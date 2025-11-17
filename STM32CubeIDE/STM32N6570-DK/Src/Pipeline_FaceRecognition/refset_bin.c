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
#include "usb_embeddings.h"
#include "FreeRTOS.h"
#include "task.h"

static StaticTask_t g_refset_tcb;
static StackType_t  g_refset_stack[512];
static TaskHandle_t g_refset_task = NULL;
volatile bool g_refset_ready = false;
EncList g_enc = {0};
EmbRec* g_ref_set = NULL;
int g_ref_set_count = 0;

static const uint8_t FR_AES_KEY_16[16] = {
    0x60,0x3D,0xEB,0x10,0x15,0xCA,0x71,0xBE,
    0x2B,0x73,0xAE,0xF0,0x85,0x7D,0x77,0x81
};
static const uint8_t FR_AES_IV_16[16]  = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F
};
static void* xmalloc(size_t n)
{
    void* p = malloc(n);
    if (!p) { printf("[OOM]\r\n"); for(;;); }
    return p;
}

/* AES CBC decrypt with PKCS7 padding check */
static int aes_cbc_pkcs7_decrypt_inplace(uint8_t* buf, size_t* len)
{
    struct AES_ctx ctx;
    uint8_t iv[16];
    memcpy(iv, FR_AES_IV_16, 16);
    AES_init_ctx_iv(&ctx, FR_AES_KEY_16, iv);
    AES_CBC_decrypt_buffer(&ctx, buf, *len);

    uint8_t pad = buf[*len - 1];
    if (pad > 0 && pad <= 16 && pad <= *len)
        *len -= pad;
    else {
        printf("[AES][WARN] Bad padding byte=%u (len=%lu)\r\n", pad, (unsigned long)*len);
    }
    return 0;
}

/* Load single .bin (skip *_pin.bin) */
static bool load_one_bin(const char* path)
{
    FIL f; UINT br;
    if (f_open(&f, path, FA_READ) != FR_OK) return false;
    uint8_t hdr[12]; f_read(&f, hdr, sizeof(hdr), &br);
    if (memcmp(hdr, "FREB", 4) != 0) { f_close(&f); return false; }
    uint16_t name_len = hdr[6] | (hdr[7]<<8);
    uint16_t emb_dim  = hdr[8] | (hdr[9]<<8);
    uint16_t n_emb    = hdr[10]|(hdr[11]<<8);
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
        if (strstr(fi.fname, "_pin.bin")) continue;
        char path[128];
        snprintf(path, sizeof(path), "%s/%s", dirpath, fi.fname);
        load_one_bin(path);
    }
    f_closedir(&d);
    printf("[SD] total encrypted embeddings: %d\r\n", g_enc.n);
}

void FR_DecryptAllRefsetOnce(void)
{
    if (g_enc.n == 0) { printf("[DEC][ERR] No embeddings loaded.\r\n"); return; }
    printf("[DEC] Starting AES-CBC decryption for %d embeddings...\r\n", g_enc.n);
    g_ref_set = xmalloc(sizeof(EmbRec) * g_enc.n);
    g_ref_set_count = g_enc.n;
    for (int i=0; i<g_enc.n; ++i) {
        EncEntry *e = &g_enc.v[i];
        size_t dec_len = e->enc_len;
        uint8_t *dec_buf = xmalloc(dec_len);
        memcpy(dec_buf, e->enc, dec_len);
        aes_cbc_pkcs7_decrypt_inplace(dec_buf, &dec_len);
        if (dec_len > sizeof(float)*e->dim)
            dec_len = sizeof(float)*e->dim;
        float *fvec = (float*)dec_buf;
        for (int j=0;j<e->dim;j++){
            if (!isfinite(fvec[j]) || fabsf(fvec[j]) > 100.0f)
                fvec[j] = 0.0f;
        }
        g_ref_set[i].name = e->name;
        g_ref_set[i].dim  = e->dim;
        g_ref_set[i].data = dec_buf;
    }
    printf("[DEC] All embeddings decrypted (TinyAES backend)\r\n");
}

void FR_Refset_FreeAll(void)
{
    for (int i=0;i<g_ref_set_count;i++)
        free((void*)g_ref_set[i].data);
    free(g_ref_set);
    g_ref_set=NULL; g_ref_set_count=0;

    for (int i=0;i<g_enc.n;i++){ free(g_enc.v[i].enc); free(g_enc.v[i].name); }
    free(g_enc.v); g_enc.v=NULL; g_enc.n=0;
}

bool FR_LoadAndDecryptPinForName(const char *base_dir, const char *name)
{
    if (!USB_SD_EnsureMounted()) return false;
    char lname[64]; strncpy(lname, name, sizeof(lname)-1);
    lname[sizeof(lname)-1]='\0';
    for(char*p=lname;*p;p++) *p=tolower(*p);
    char path[128];
    snprintf(path,sizeof(path),"%s/%s_pin.bin",base_dir,lname);
    printf("[PIN] Opening: %s\r\n", path);
    FIL f; FRESULT r=f_open(&f,path,FA_READ);
    if(r!=FR_OK){ printf("[PIN] File not found (%d)\r\n",r); return false; }
    uint8_t hdr[8]; UINT br;
    f_read(&f,hdr,sizeof(hdr),&br);
    if(memcmp(hdr,"FPIN",4)!=0){ printf("[PIN] Invalid header\r\n"); f_close(&f); return false; }
    uint16_t name_len=hdr[6]|(hdr[7]<<8);
    char tmpname[64]={0}; f_read(&f,tmpname,name_len,&br);
    uint8_t len4[4]; f_read(&f,len4,4,&br);
    uint32_t enc_len=len4[0]|(len4[1]<<8)|(len4[2]<<16)|(len4[3]<<24);
    uint8_t *enc=xmalloc(enc_len+1);
    f_read(&f,enc,enc_len,&br);
    f_close(&f);
    aes_cbc_pkcs7_decrypt_inplace(enc,(size_t*)&enc_len);
    enc[enc_len]='\0';
    printf("[PIN] Decrypted PIN for %s: '%s'\r\n", name, (char*)enc);
    memset(g_current_pin,0,sizeof(g_current_pin));
    strncpy(g_current_pin,(char*)enc,sizeof(g_current_pin)-1);
    free(enc);
    return true;
}

static void RefsetLoader_Task(void *arg)
{
    (void)arg;
    printf("[REFSET] Task start: mount + load + decrypt...\r\n");

    if (!USB_SD_EnsureMounted()) {
        printf("[REFSET][ERR] SD not mounted, aborting load.\r\n");
        vTaskDelete(NULL);
    }

    FR_LoadRefsetFromSD_Bin("0:/binaries");
    FR_DecryptAllRefsetOnce();

    printf("[REFSET] Ready: %d embeddings in RAM\r\n", g_ref_set_count);
    g_refset_ready = true;

    vTaskDelete(NULL);
}

void FR_WaitRefsetReady(void)
{
    while (!g_refset_ready) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void FR_StartRefsetLoader(void)
{
    if (g_refset_task == NULL) {
        g_refset_task = xTaskCreateStatic(
            RefsetLoader_Task,
            "refset",
            sizeof(g_refset_stack) / sizeof(StackType_t),
            NULL,
            tskIDLE_PRIORITY + 2,
            g_refset_stack,
            &g_refset_tcb
        );
        configASSERT(g_refset_task != NULL);
    }
}
