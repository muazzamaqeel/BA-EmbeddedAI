/* ========================= fr_helpers.h ========================= */
#pragma once

#include <stdint.h>
#include "od_pp_output_if.h"   // official type for od_pp_outBuffer_t
#include "fr_model_cfg.h"      // <-- NEW: always pull FR_IN_W/FR_IN_H/FR_EMB_SIZE from generated header

/* ---------- public config (tunable thresholds, not sizes) ---------- */
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
