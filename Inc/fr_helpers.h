#ifndef FR_HELPERS_H
#define FR_HELPERS_H

#include <stdint.h>
#include "app_postprocess.h"   /* od_pp_outBuffer_t, od_pp_out_t */
#include "app_config.h"        /* NN_WIDTH / NN_HEIGHT for snapshot sizing */

/* FaceRec input size & embedding size */
#define FR_IN_W 112
#define FR_IN_H 112
#define FR_EMB_SIZE 512

/* Tune your decision threshold */
#ifndef FR_MATCH_THR
#define FR_MATCH_THR 0.80f
#endif

void fr_init(void);

/* Called from nn thread: keep last detector input (NHWC FP32 in [0..1]) */
void fr_update_frame_snapshot(const float *src_nhwc, uint32_t bytes);

/* Build FR input for a single detection (bilinear crop -> [-1..1]) */
void fr_prepare_input_for_det(const od_pp_outBuffer_t *d,
                              float *fr_in, int fr_w, int fr_h,
                              int det_in_w, int det_in_h,
                              int det_idx);

/* After FR inference (INT8 emb), convert, enroll/match & print verdicts */
void fr_after_inference_and_decide(const int8_t *emb_q, int emb_len, int det_idx);

/* Safety: warn if FR input & output aliases the same address */
void fr_check_alias(const void *in_ptr, const void *out_ptr);

#endif /* FR_HELPERS_H */
