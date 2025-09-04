/* ========================= fr_helpers.h ========================= */
#pragma once

#include <stdint.h>
#include "od_pp_output_if.h"   // <-- USE THE OFFICIAL TYPE DEF FOR od_pp_outBuffer_t

/* ---------- public config (kept here for completeness) ---------- */
#ifndef FR_EMB_SIZE
#define FR_EMB_SIZE         512
#endif
#ifndef FR_IN_W
#define FR_IN_W             112
#endif
#ifndef FR_IN_H
#define FR_IN_H             112
#endif
#ifndef NN_WIDTH
#define NN_WIDTH            128
#endif
#ifndef NN_HEIGHT
#define NN_HEIGHT           128
#endif
#ifndef FR_DEFAULT_MATCH_THR
#define FR_DEFAULT_MATCH_THR 0.80f
#endif
#ifndef FR_SUBJECT_NAME
#define FR_SUBJECT_NAME "ME"
#endif
#ifndef FR_ENROLL_CONF_MIN
#define FR_ENROLL_CONF_MIN   0.70f
#endif
#ifndef FR_ENROLL_MIN_SIDE
#define FR_ENROLL_MIN_SIDE   0.28f
#endif
#ifndef FR_ENROLL_CENTER_TOL
#define FR_ENROLL_CENTER_TOL 0.60f
#endif

/* ---------- API ---------- */
void  fr_init(void);
void  fr_update_frame_snapshot(const float *src_nhwc, uint32_t bytes);

void  fr_prepare_input_for_det(const od_pp_outBuffer_t *d,
                               float *fr_in, int fr_w, int fr_h,
                               int det_in_w, int det_in_h,
                               int det_idx);

void  fr_after_inference_and_decide(const float *emb_in, int emb_len, int det_idx);

int   fr_is_enrolled(void);
int   fr_get_last_match(float *sim_out);
void  fr_set_match_threshold(float thr);
void  fr_set_enroll_gate(int ok);
void  fr_reset_enrollment(void);
void  fr_check_alias(const void *in_ptr, const void *out_ptr);

/* Progress & effective threshold (used by pp_thread_fct) */
int   fr_get_enroll_progress(int *count_out, int *target_out);
float fr_get_effective_threshold(void);
