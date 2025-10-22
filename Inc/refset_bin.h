#pragma once
#include <stdint.h>
#include <stdbool.h>

bool FR_LoadAndDecryptPinForName(const char *base_dir, const char *name);

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
