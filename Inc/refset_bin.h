#pragma once
#include <stdint.h>
#include <stdbool.h>

/* ============================================================
 *  Face Recognition Reference Set (Embeddings + PIN Decryptor)
 * ============================================================ */

/* Base directory for all embeddings and PINs */
#define FR_SD_BASE_DIR   "0:/binaries"

/* ================== Embedding Structures ================== */

/* One decrypted embedding record */
typedef struct {
    const char* name;      // Person name (UTF-8)
    const float* data;     // Pointer to decrypted float array (size = dim)
    int dim;               // Number of float elements
} EmbRec;

/* One encrypted embedding entry as stored on SD */
typedef struct {
    char*     name;        // Person name
    uint8_t*  enc;         // Encrypted data buffer
    int       enc_len;     // Encrypted length (bytes)
    int       emb_dim;     // Embedding dimension (e.g. 512)
} EncEntry;

/* List of all encrypted entries */
typedef struct {
    EncEntry* v;           // Dynamic array of EncEntry
    int n;                 // Count
} EncList;

/* ================== Globals ================== */
extern EncList g_enc;           // All encrypted embeddings
extern EmbRec* g_ref_set;       // All decrypted embeddings
extern int g_ref_set_count;     // Count of decrypted embeddings

/* Global PIN buffer (declared in app_change_pin.c) */
extern char g_current_pin[8];

/* ================== Public API ================== */

/* Load all encrypted embedding BIN files from SD (skipping *_pin.bin) */
void FR_LoadRefsetFromSD_Bin(const char* dirpath);

/* Decrypt all loaded embeddings into float arrays */
void FR_DecryptAllRefsetOnce(void);

/* Free all memory (enc + decrypted refset) */
void FR_Refset_FreeAll(void);

/* Load and decrypt a single *_pin.bin for a given name */
bool FR_LoadAndDecryptPinForName(const char *base_dir, const char *name);
