/*
 * crypto_utils.c  – AES-CBC decryptor for encrypted face embeddings
 * Compatible with gen_refset_tflite.py encryption script (AES-128-CBC, PKCS7)
 *
 * Platform : STM32N6570-DK (Cortex-M55)
 * Backend  : TinyAES (software, always available)
 */

#include "crypto_utils.h"
#include "aes.h"          // from Lib/crypto/tinyAES
#include <string.h>
#include <stdio.h>
#include <math.h>

/* --------------------------------------------------------------------------
 * AES-128 CBC configuration — must match Python encryption script
 * -------------------------------------------------------------------------- */
static const uint8_t FR_AES_KEY_16[16] = {
    0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE,
    0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81
};

static const uint8_t FR_AES_IV_16[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

/* --------------------------------------------------------------------------
 * Structure definition (from embeddings_table_combined.c)
 * -------------------------------------------------------------------------- */
typedef struct {
    const char* name;
    const uint8_t* data;
    int dim;
} EmbRec;

extern EmbRec *g_ref_set;
extern int g_ref_set_count;

/* --------------------------------------------------------------------------
 * AES-CBC decrypt (TinyAES)
 * -------------------------------------------------------------------------- */
static int fr_aes_cbc_decrypt(uint8_t *buf, size_t len)
{
    struct AES_ctx ctx;
    uint8_t iv[16];
    memcpy(iv, FR_AES_IV_16, sizeof(iv));

    if ((len % 16) != 0) {
        printf("[AES][ERR] len not multiple of 16\r\n");
        return -1;
    }

    AES_init_ctx_iv(&ctx, FR_AES_KEY_16, iv);
    AES_CBC_decrypt_buffer(&ctx, buf, len);

    return 0;
}

/* --------------------------------------------------------------------------
 * Single embedding decrypt → float array
 * -------------------------------------------------------------------------- */
int FR_DecryptEmbedding_ToFloat(const uint8_t *cipher, size_t len_bytes,
                                float *out_f32, size_t out_f32_capacity)
{
    if (!cipher || !out_f32 || (len_bytes % 16) != 0)
        return -1;

    if (len_bytes > 2048)
        return -2;

    uint8_t buf[2048];
    memcpy(buf, cipher, len_bytes);

    int ret = fr_aes_cbc_decrypt(buf, len_bytes);
    if (ret != 0)
        return -3;

    size_t n = len_bytes / sizeof(float);
    if (n > out_f32_capacity)
        n = out_f32_capacity;

    memcpy(out_f32, buf, n * sizeof(float));
    return (int)n;
}

/* --------------------------------------------------------------------------
 * Bulk decrypt for all reference embeddings
 * -------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------
 * Bulk decrypt + normalize all reference embeddings
 * -------------------------------------------------------------------------- */
void FR_DecryptAllRefsetOnce(void)
{
    printf("[DEC] Starting software AES-CBC decryption for %d embeddings...\r\n",
           g_ref_set_count);

    static uint8_t __attribute__((aligned(4))) temp[2048];

    for (int i = 0; i < g_ref_set_count; ++i)
    {
        EmbRec *r = &g_ref_set[i];
        size_t len = r->dim * sizeof(float);
        if (len > sizeof(temp)) {
            printf("[DEC][ERR] %s too large (%u bytes)\r\n", r->name, (unsigned)len);
            continue;
        }

        /* Copy encrypted data */
        memcpy(temp, r->data, len);

        /* AES-CBC decrypt */
        int ret = fr_aes_cbc_decrypt(temp, len);
        if (ret != 0) {
            printf("[DEC][ERR] %s decrypt failed (%d)\r\n", r->name, ret);
            continue;
        }

        /* Optional: endian correction (Python is little-endian, STM32 too)
           — only enable if decrypted values look wrong (e.g., 1e+37) */
#if 0
        for (size_t j = 0; j < len; j += 4) {
            uint8_t t0 = temp[j]; temp[j] = temp[j+3]; temp[j+3] = t0;
            uint8_t t1 = temp[j+1]; temp[j+1] = temp[j+2]; temp[j+2] = t1;
        }
#endif

        float *vec = (float*)temp;

        /* Optional: dequantize if your refset was INT8 scaled by 128 */
#ifdef FR_INPUT_IS_U8
        for (int j = 0; j < r->dim; ++j)
            vec[j] *= (1.0f / 128.0f);
#endif

        /* Normalize the embedding to unit length (L2 normalization) */
        float norm = 0.0f;
        for (int j = 0; j < r->dim; ++j)
            norm += vec[j] * vec[j];
        norm = sqrtf(norm) + 1e-9f;  // prevent divide by zero
        for (int j = 0; j < r->dim; ++j)
            vec[j] /= norm;

        /* Copy normalized float array back into reference set (if mutable) */
        memcpy((void*)r->data, vec, len);

        /* Debug: print first few floats for verification */
        printf("[DEC][%s] first 4 floats: %.6f %.6f %.6f %.6f\r\n",
               r->name, vec[0], vec[1], vec[2], vec[3]);
        printf("[DEC][%s] norm=%.6f\r\n", r->name, norm);
    }

    printf("[DEC] All embeddings decrypted + normalized (TinyAES backend)\r\n");
}
