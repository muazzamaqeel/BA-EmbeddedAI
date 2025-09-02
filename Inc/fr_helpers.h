/* ========================= fr_helpers.h ========================= */
#ifndef FR_HELPERS_H
#define FR_HELPERS_H

#include <stdint.h>
#include "app_postprocess.h"
#include "app_config.h"

#ifndef ALIGN_32
#define ALIGN_32 __attribute__((aligned(32)))
#endif
#ifndef IN_PSRAM
#define IN_PSRAM __attribute__((section(".psram")))
#endif

#define FR_IN_W      112
#define FR_IN_H      112
#define FR_EMB_SIZE  128

#ifndef FR_SUBJECT_NAME
#define FR_SUBJECT_NAME "ME"
#endif

#ifndef FR_DEFAULT_MATCH_THR
#define FR_DEFAULT_MATCH_THR   0.55f
#endif
#ifndef FR_ENROLL_CONF_MIN
#define FR_ENROLL_CONF_MIN     0.75f
#endif
#ifndef FR_ENROLL_MIN_SIDE
#define FR_ENROLL_MIN_SIDE     0.22f
#endif
#ifndef FR_ENROLL_CENTER_TOL
#define FR_ENROLL_CENTER_TOL   0.25f
#endif
int fr_get_last_match(float *sim_out);
int  fr_is_enrolled(void);
void fr_init(void);
void fr_update_frame_snapshot(const float *src_nhwc, uint32_t bytes);
void fr_prepare_input_for_det(const od_pp_outBuffer_t *d,
                              float *fr_in, int fr_w, int fr_h,
                              int det_in_w, int det_in_h,
                              int det_idx);
void fr_after_inference_and_decide(const float *emb_f, int emb_len, int det_idx);
void fr_check_alias(const void *in_ptr, const void *out_ptr);

void fr_set_match_threshold(float thr);
void fr_set_enroll_gate(int ok);
void fr_reset_enrollment(void);

#endif /* FR_HELPERS_H */
