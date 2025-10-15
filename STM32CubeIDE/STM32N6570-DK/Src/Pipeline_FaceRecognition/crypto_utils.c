/*
 * crypto_utils.c – AES-CBC decryptor for encrypted face embeddings
 * Compatible with gen_refset_tflite.py encryption script (AES-128-CBC, PKCS7)
 *
 * Platform : STM32N6570-DK (Cortex-M55)
 * Backend  : TinyAES (software, always available)
 */

#include "crypto_utils.h"
#include "aes.h"
#include "refset_bin.h"     // ✅ shared structs: g_enc, g_ref_set, etc.
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>         // ✅ for malloc(), free()

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
 * AES-CBC decrypt (TinyAES) with PKCS#7 unpadding
 * -------------------------------------------------------------------------- */
static int fr_aes_cbc_decrypt(uint8_t *buf, size_t *len)
{
    struct AES_ctx ctx;
    uint8_t iv[16];
    memcpy(iv, FR_AES_IV_16, sizeof(iv));

    if ((*len % 16) != 0)
    {
        printf("[AES][ERR] len not multiple of 16\r\n");
        return -1;
    }

    AES_init_ctx_iv(&ctx, FR_AES_KEY_16, iv);
    AES_CBC_decrypt_buffer(&ctx, buf, *len);

    /* ===== Remove PKCS#7 padding ===== */
    uint8_t pad = buf[*len - 1];
    if (pad > 0 && pad <= 16)
    {
        bool valid = true;
        for (int i = 0; i < pad; i++)
        {
            if (buf[*len - 1 - i] != pad)
            {
                valid = false;
                break;
            }
        }

        if (valid)
        {
            *len -= pad;
            printf("[AES] Removed PKCS7 padding = %u bytes\r\n", pad);
        }
        else
        {
            printf("[AES][WARN] Invalid padding bytes detected (pad=%u)\r\n", pad);
        }
    }
    else
    {
        printf("[AES][WARN] No valid padding (last byte=%u)\r\n", pad);
    }

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

    size_t actual_len = len_bytes;
    int ret = fr_aes_cbc_decrypt(buf, &actual_len);
    if (ret != 0)
        return -3;

    size_t n = actual_len / sizeof(float);
    if (n > out_f32_capacity)
        n = out_f32_capacity;

    memcpy(out_f32, buf, n * sizeof(float));
    return (int)n;
}

/* --------------------------------------------------------------------------
 * Bulk decrypt + normalize all reference embeddings
 * -------------------------------------------------------------------------- */
void FR_DecryptAllRefsetOnce(void)
{
    printf("[DEC] Starting software AES-CBC decryption for %d embeddings...\r\n",
           g_enc.n);

    if (g_enc.n <= 0) {
        printf("[DEC][WARN] No encrypted embeddings found.\r\n");
        return;
    }

    /* Allocate ref set container */
    g_ref_set = malloc(sizeof(EmbRec) * g_enc.n);
    g_ref_set_count = g_enc.n;

    static uint8_t __attribute__((aligned(4))) temp[2048];

    for (int i = 0; i < g_enc.n; ++i)
    {
        EncEntry *src = &g_enc.v[i];
        size_t len = src->enc_len;

        if (len > sizeof(temp)) {
            printf("[DEC][ERR] %s too large (%u bytes)\r\n", src->name, (unsigned)len);
            continue;
        }

        memcpy(temp, src->enc, len);
        int ret = fr_aes_cbc_decrypt(temp, &len);
        if (ret != 0) {
            printf("[DEC][ERR] %s decrypt failed (%d)\r\n", src->name, ret);
            continue;
        }

        float *vec = (float*)temp;
        size_t n = len / sizeof(float);

        /* Normalize to unit length */
        float norm = 0.0f;
        for (size_t j = 0; j < n; ++j)
            norm += vec[j] * vec[j];
        norm = sqrtf(norm) + 1e-9f;
        for (size_t j = 0; j < n; ++j)
            vec[j] /= norm;

        float *stored = malloc(n * sizeof(float));
        memcpy(stored, vec, n * sizeof(float));

        g_ref_set[i].name = src->name;
        g_ref_set[i].data = (const float*)stored;   // ✅ fixed const-cast
        g_ref_set[i].dim  = (int)n;

        printf("[DEC][%s] norm=%.6f\r\n", src->name, norm);
    }

    printf("[DEC] All embeddings decrypted + normalized (TinyAES backend)\r\n");
}
