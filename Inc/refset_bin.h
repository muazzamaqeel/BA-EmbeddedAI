#pragma once
#include <stdint.h>
#include <stdbool.h>

extern volatile bool g_refset_ready;
void FR_StartRefsetLoader(void);
void FR_WaitRefsetReady(void);

/* Base directory for all embeddings and PINs */
#define FR_SD_BASE_DIR   "0:/binaries"

/* ================== PIN Decryptor ================== */
bool FR_LoadAndDecryptPinForName(const char *base_dir, const char *name);

/* ================== Embedding Structures ================== */
typedef struct {
    const char* name;
    const float* data;
    int dim;
} EmbRec;

typedef struct {
    char*     name;
    uint8_t*  enc;
    int       enc_len;
    int       dim;
} EncEntry;

typedef struct {
    EncEntry* v;
    int n;
} EncList;

/* Global containers (shared between SD loader and decryptor) */
extern EncList g_enc;
extern EmbRec* g_ref_set;
extern int g_ref_set_count;

/* Public API */
void FR_LoadRefsetFromSD_Bin(const char* dirpath);
void FR_Refset_FreeAll(void);
void FR_DecryptAllRefsetOnce(void);
