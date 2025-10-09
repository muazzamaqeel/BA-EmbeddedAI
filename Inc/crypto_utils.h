#pragma once
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Decrypt a single 2048-byte AES-CBC ciphertext (no padding) to float32[512].
 * 
 * @param cipher Pointer to input ciphertext bytes.
 * @param len_bytes Length of ciphertext in bytes (must be multiple of 16).
 * @param out_f32 Output buffer for decrypted float32 values.
 * @param out_f32_capacity Maximum number of floats that can fit in out_f32.
 * @return int Number of floats written on success, negative on error.
 */
int FR_DecryptEmbedding_ToFloat(const uint8_t *cipher, size_t len_bytes,
                                float *out_f32, size_t out_f32_capacity);

/**
 * @brief Decrypt all reference embeddings once at startup.
 * 
 * Uses globals from embeddings_table_combined.c:
 *   - EmbRec *g_ref_set
 *   - int g_ref_set_count
 */
void FR_DecryptAllRefsetOnce(void);
