#pragma once
#include <stdint.h>
#include "app_postprocess.h"   // od_pp_outBuffer_t

#ifdef __cplusplus
extern "C" {
#endif

/* Extract a single embedding from an in-memory RGB888 image. */
int FR_Extract_From_RGB888(const uint8_t *rgb,
                           int img_w, int img_h,
                           float *out_emb, int out_emb_cap,
                           od_pp_outBuffer_t *out_roi);

/* Scan /UserFace on the MCU (FatFs), process images, and write /UserFace/data_embeddings.txt. */
int FR_Rebuild_UserFace_Embeddings(void);

#ifdef __cplusplus
}
#endif
