#pragma once
#include <stdint.h>
#include "app_postprocess.h"  // od_pp_outBuffer_t

int FR_Extract_From_RGB888(const uint8_t *rgb,
                           int img_w, int img_h,
                           float *out_emb, int out_emb_cap,
                           od_pp_outBuffer_t *out_roi);
