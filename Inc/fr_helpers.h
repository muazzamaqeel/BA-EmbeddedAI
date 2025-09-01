/* ========================= fr_helpers.h ========================= */
#ifndef FR_HELPERS_H
#define FR_HELPERS_H

#include <stdint.h>
#include "app_postprocess.h"   /* od_pp_outBuffer_t, od_pp_out_t */
#include "app_config.h"        /* NN_WIDTH / NN_HEIGHT for snapshot sizing */

/* ---- FaceRec input & embedding sizes ---- */
#define FR_IN_W      112
#define FR_IN_H      112
#define FR_EMB_SIZE  512

/* ---- Subject name for logs (override before including this header if desired) ---- */
#ifndef FR_SUBJECT_NAME
#define FR_SUBJECT_NAME "ME"
#endif

/* ---- Matching & enrollment defaults (can be changed at runtime) ----
 * You can tweak these at compile time, but prefer the runtime setters below.
 */
#ifndef FR_DEFAULT_MATCH_THR
#define FR_DEFAULT_MATCH_THR   0.55f   /* safer start than 0.80 for int8 embeddings */
#endif

#ifndef FR_ENROLL_CONF_MIN
#define FR_ENROLL_CONF_MIN     0.75f   /* detector confidence required to enroll */
#endif

#ifndef FR_ENROLL_MIN_SIDE
#define FR_ENROLL_MIN_SIDE     0.22f   /* min of max(w,h) (normalized to detector input) */
#endif

#ifndef FR_ENROLL_CENTER_TOL
#define FR_ENROLL_CENTER_TOL   0.25f   /* |cx-0.5| and |cy-0.5| must be <= this */
#endif

/* ---------- API ---------- */

/* Init FaceRec helper (clears snapshot, resets enrollment, prints config) */
void fr_init(void);

/* Called from NN thread: keep last detector input (NHWC FP32 in [0..1]) */
void fr_update_frame_snapshot(const float *src_nhwc, uint32_t bytes);

/* Build FR input for a single detection (bilinear crop; converts [0..1] -> [-1..1]) */
void fr_prepare_input_for_det(const od_pp_outBuffer_t *d,
                              float *fr_in, int fr_w, int fr_h,
                              int det_in_w, int det_in_h,
                              int det_idx);

/* After FR inference (INT8 embedding), convert, (optionally enroll), match & print verdicts */
void fr_after_inference_and_decide(const int8_t *emb_q, int emb_len, int det_idx);

/* Safety: warn if FR input & output alias the same address */
void fr_check_alias(const void *in_ptr, const void *out_ptr);

/* ---------- Runtime configuration hooks ---------- */

/* Change the cosine-similarity match threshold at runtime */
void fr_set_match_threshold(float thr);

/* For the current detection, set whether auto-enrollment is allowed (0/1).
 * The pp thread should call this per detection BEFORE fr_prepare_input_for_det(),
 * typically based on detector conf/size/centering.
 */
void fr_set_enroll_gate(int ok);

/* Reset enrollment (clears samples & centroid; starts collecting again) */
void fr_reset_enrollment(void);

#endif /* FR_HELPERS_H */
