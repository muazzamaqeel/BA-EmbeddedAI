#include "refset_bin.h"
#include "fatfs.h"
#include "stm32n6570_discovery_sd.h"
#include "aes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include "app_change_pin.h"
#include "usb_embeddings.h"   // ✅ for USB_SD_EnsureMounted()

/* ================= AES parameters ================= */
static const uint8_t FR_AES_KEY_16[16] = {
    0x60,0x3D,0xEB,0x10,0x15,0xCA,0x71,0xBE,
    0x2B,0x73,0xAE,0xF0,0x85,0x7D,0x77,0x81
};
static const uint8_t FR_AES_IV_16[16]  = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F
};

/* ================= Global containers ================= */
EncList g_enc = {0};        // encrypted embeddings
EmbRec* g_ref_set = NULL;   // decrypted embeddings
int g_ref_set_count = 0;

/* ================= Helpers ================= */
static void* xmalloc(size_t n)
{
    void* p = malloc(n);
    if (!p) { printf("[OOM]\r\n"); for(;;); }
    return p;
}

/* --- AES CBC decrypt + PKCS7 remove --- */
static int aes_cbc_pkcs7_decrypt_inplace(uint8_t* buf, size_t* len)
{
    struct AES_ctx ctx;
    uint8_t iv[16];
    memcpy(iv, FR_AES_IV_16, 16);
    AES_init_ctx_iv(&ctx, FR_AES_KEY_16, iv);
    AES_CBC_decrypt_buffer(&ctx, buf, *len);

    uint8_t pad = buf[*len - 1];
    if (pad > 0 && pad <= 16)
    {
        *len -= pad;
    }
    else
    {
        printf("[AES][WARN] No valid padding (last byte=%d)\r\n", pad);
    }
    return 0;
}

/* --- load single .bin (face embeddings) --- */
static bool load_one_bin(const char* path)
{
    FIL f;
    FRESULT r = f_open(&f, path, FA_READ);
    if (r != FR_OK) {
        printf("[BIN] open fail %s (err %d)\r\n", path, r);
        return false;
    }

    uint8_t hdr[12]; UINT br;
    f_read(&f, hdr, sizeof(hdr), &br);
    if (memcmp(hdr, "FREB", 4) != 0) {
        printf("[BIN] Invalid header in %s\r\n", path);
        f_close(&f);
        return false;
    }

    uint16_t name_len = hdr[6] | (hdr[7] << 8);
    uint16_t emb_dim  = hdr[8] | (hdr[9] << 8);
    uint16_t n_emb    = hdr[10]| (hdr[11]<<8);

    char* name = xmalloc(name_len+1);
    f_read(&f, name, name_len, &br);
    name[name_len] = '\0';

    for (int i=0; i<n_emb; i++) {
        uint8_t len4[4]; f_read(&f, len4, 4, &br);
        uint32_t enc_len = len4[0] | (len4[1]<<8) | (len4[2]<<16) | (len4[3]<<24);
        uint8_t* enc = xmalloc(enc_len);
        f_read(&f, enc, enc_len, &br);

        g_enc.v = realloc(g_enc.v, (g_enc.n+1)*sizeof(EncEntry));
        g_enc.v[g_enc.n++] = (EncEntry){ name, enc, (int)enc_len, (int)emb_dim };
    }

    printf("[BIN] %s loaded (%d×%d)\r\n", name, n_emb, emb_dim);
    f_close(&f);
    return true;
}

/* --- directory scanner for embeddings (skips *_pin.bin) --- */
void FR_LoadRefsetFromSD_Bin(const char* dirpath)
{
    if (!USB_SD_EnsureMounted()) return;

    DIR d; FILINFO fi;
    if (f_opendir(&d, dirpath) != FR_OK) {
        printf("[SD] open dir failed: %s\r\n", dirpath);
        return;
    }

    while (f_readdir(&d, &fi) == FR_OK && fi.fname[0]) {
        if (fi.fattrib & AM_DIR) continue;
        if (!strstr(fi.fname, ".bin")) continue;

        // Skip PIN files
        size_t len = strlen(fi.fname);
        if (len > 8 && strcmp(fi.fname + (len - 8), "_pin.bin") == 0)
            continue;

        char path[128];
        snprintf(path, sizeof(path), "%s/%s", dirpath, fi.fname);
        load_one_bin(path);
    }

    f_closedir(&d);
    printf("[SD] total encrypted embeddings: %d\r\n", g_enc.n);
}

/* --- decrypt all embeddings --- */
void FR_DecryptAllRefsetOnce(void)
{
    if (g_enc.n == 0) {
        printf("[DEC][ERR] No embeddings loaded.\r\n");
        return;
    }

    printf("[DEC] Starting software AES-CBC decryption for %d embeddings...\r\n", g_enc.n);

    g_ref_set = xmalloc(sizeof(EmbRec) * g_enc.n);
    g_ref_set_count = g_enc.n;

    for (int i=0; i<g_enc.n; ++i) {
        EncEntry *e = &g_enc.v[i];
        size_t dec_len = e->enc_len;

        uint8_t *dec_buf = xmalloc(dec_len);
        memcpy(dec_buf, e->enc, dec_len);

        aes_cbc_pkcs7_decrypt_inplace(dec_buf, &dec_len);

        // validate expected embedding size (512 floats)
        if (dec_len > sizeof(float) * e->emb_dim) {
            printf("[DEC][WARN] %s padded to %d bytes → valid %d\r\n",
                   e->name, e->enc_len, (int)dec_len);
            dec_len = sizeof(float) * e->emb_dim;
        }

        float *fvec = (float *)dec_buf;
        for (int j = 0; j < e->emb_dim; ++j) {
            if (!isfinite(fvec[j])) fvec[j] = 0.0f;
        }

        g_ref_set[i].name = e->name;
        g_ref_set[i].dim  = e->emb_dim;
        g_ref_set[i].data = dec_buf;
    }

    printf("[DEC] All embeddings decrypted + normalized (TinyAES backend)\r\n");
}

/* --- free memory --- */
void FR_Refset_FreeAll(void)
{
    for (int i=0; i<g_ref_set_count; i++)
        free((void*)g_ref_set[i].data);

    free(g_ref_set); g_ref_set = NULL; g_ref_set_count = 0;

    for (int i=0; i<g_enc.n; i++) {
        free(g_enc.v[i].enc);
        free(g_enc.v[i].name);
    }

    free(g_enc.v);
    g_enc.v = NULL;
    g_enc.n = 0;
}

/* ============================================================
 *  PIN BIN loader and decryptor
 * ============================================================ */
bool FR_LoadAndDecryptPinForName(const char *base_dir, const char *name)
{
    if (!base_dir || !name) {
        printf("[PIN] Invalid args (base_dir or name is NULL)\r\n");
        return false;
    }

    if (!USB_SD_EnsureMounted()) {
        printf("[PIN] SD not mounted\r\n");
        return false;
    }

    char lname[64];
    strncpy(lname, name, sizeof(lname)-1);
    lname[sizeof(lname)-1] = '\0';
    for (char *p = lname; *p; ++p) *p = (char)tolower((unsigned char)*p);

    char path[128];
    snprintf(path, sizeof(path), "%s/%s_pin.bin", base_dir, lname);
    printf("[PIN] Opening: %s\r\n", path);

    FIL f;
    FRESULT r = f_open(&f, path, FA_READ);
    if (r != FR_OK) {
        printf("[PIN] File not found or open fail (%d): %s\r\n", r, path);
        return false;
    }

    uint8_t hdr[8]; UINT br;
    f_read(&f, hdr, sizeof(hdr), &br);
    if (memcmp(hdr, "FPIN", 4) != 0) {
        printf("[PIN] Invalid header in %s\r\n", path);
        f_close(&f);
        return false;
    }

    uint16_t ver      = hdr[4] | (hdr[5] << 8);
    uint16_t name_len = hdr[6] | (hdr[7] << 8);
    (void)ver;

    char file_name[64] = {0};
    f_read(&f, file_name, name_len, &br);
    file_name[name_len] = '\0';

    uint8_t len4[4];
    f_read(&f, len4, 4, &br);
    uint32_t enc_len = len4[0] | (len4[1]<<8) | (len4[2]<<16) | (len4[3]<<24);

    uint8_t *enc_buf = xmalloc(enc_len + 1);
    f_read(&f, enc_buf, enc_len, &br);
    f_close(&f);

    printf("[PIN] Loaded %s (enc_len=%lu)\r\n", path, (unsigned long)enc_len);

    aes_cbc_pkcs7_decrypt_inplace(enc_buf, (size_t *)&enc_len);
    enc_buf[enc_len] = '\0';

    printf("[PIN] Decrypted PIN for %s: '%s'\r\n", name, (char*)enc_buf);

    memset(g_current_pin, 0, sizeof(g_current_pin));
    strncpy(g_current_pin, (char*)enc_buf, sizeof(g_current_pin)-1);

    free(enc_buf);
    return true;
}
