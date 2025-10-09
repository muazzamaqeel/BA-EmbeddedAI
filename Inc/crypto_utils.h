#pragma once
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Decrypt a single 2048-byte AES-CBC ciphertext (no padding) to float32[512].
 */
int FR_DecryptEmbedding_ToFloat(const uint8_t *cipher, size_t len_bytes,
                                float *out_f32, size_t out_f32_capacity);

/**
 * @brief Decrypt all reference embeddings once at startup.
 */
void FR_DecryptAllRefsetOnce(void);
