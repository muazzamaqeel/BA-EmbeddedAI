/* embeddings_extractor.c
 *
 * Offline extractor: given an RGB888 image buffer in memory, run BlazeFace +
 * FaceRec to produce one embedding vector. No camera needed.
 *
 * Public API:
 *   int FR_Extract_From_RGB888(const uint8_t *rgb, int img_w, int img_h,
 *                              float *out_emb, int out_emb_cap,
 *                              od_pp_outBuffer_t *out_roi);
 *
 * Return:
 *   0  -> success, embedding written
 *   +1 -> no face found
 *   <0 -> error
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "stm32n6xx_hal.h"

#include "network.h"
#include "app_config.h"
#include "app_postprocess.h"
#include "cache_utils.h"
#include "ll_aton_runtime.h"
#include "facedetection_imp.h"
#include "facerecognition_imp.h"
#include "face_recognition.h"
#include "facedetection_imp_bridge.h"
#include "fr_helpers.h"

/* Network-specific buffer info helpers (generated names) */
extern const LL_Buffer_InfoTypeDef *LL_ATON_Input_Buffers_Info_face_recognition(void);
extern const LL_Buffer_InfoTypeDef *LL_ATON_Output_Buffers_Info_face_recognition(void);

/* If your FaceRec Run wrapper is declared elsewhere, keep this extern: */
void FaceRec_Run_NoLock(void);


/* -------- helpers -------- */

static inline int clampi(int v, int lo, int hi) {
  return (v < lo) ? lo : (v > hi) ? hi : v;
}

/* Bilinear resize: RGB888 src -> NHWC float dst in [0..1] (size: dst_w x dst_h x 3) */
static void resize_rgb888_to_float_nhwc(const uint8_t *src, int sw, int sh,
                                        float *dst, int dw, int dh)
{
  const float sx_scale = (float)sw / (float)dw;
  const float sy_scale = (float)sh / (float)dh;
  const float inv255 = 1.0f / 255.0f;

  for (int y = 0; y < dh; ++y) {
    float fy = (y + 0.5f) * sy_scale - 0.5f;
    int y0 = (int)floorf(fy);
    int y1 = y0 + 1;
    float wy = fy - (float)y0;
    y0 = clampi(y0, 0, sh - 1);
    y1 = clampi(y1, 0, sh - 1);

    for (int x = 0; x < dw; ++x) {
      float fx = (x + 0.5f) * sx_scale - 0.5f;
      int x0 = (int)floorf(fx);
      int x1 = x0 + 1;
      float wx = fx - (float)x0;
      x0 = clampi(x0, 0, sw - 1);
      x1 = clampi(x1, 0, sw - 1);

      /* 4 source pixels (each is RGB888) */
      const uint8_t *p00 = src + ((y0 * sw + x0) * 3);
      const uint8_t *p01 = src + ((y0 * sw + x1) * 3);
      const uint8_t *p10 = src + ((y1 * sw + x0) * 3);
      const uint8_t *p11 = src + ((y1 * sw + x1) * 3);

      float w00 = (1.f - wx) * (1.f - wy);
      float w01 =        wx  * (1.f - wy);
      float w10 = (1.f - wx) *        wy;
      float w11 =        wx  *        wy;

      int di = (y * dw + x) * 3;

      /* R,G,B order (adjust if your detector expects BGR) */
      for (int c = 0; c < 3; ++c) {
        float v = (float)p00[c] * w00 +
                  (float)p01[c] * w01 +
                  (float)p10[c] * w10 +
                  (float)p11[c] * w11;
        dst[di + c] = v * inv255;     /* [0..255] -> [0..1] */
      }
    }
  }
}

/* Crop + resize a normalized ROI from a NHWC [0..1] frame to FR input [-1..1] */
static void crop_normROI_to_FR_input(const float *src_nhwc, int src_w, int src_h,
                                     const od_pp_outBuffer_t *roi,
                                     float *dst_nhwc, int dst_w, int dst_h)
{
  /* Make square with a small margin (same policy as your runtime) */
  float cx = roi->x_center * src_w;
  float cy = roi->y_center * src_h;
  float bw = roi->width  * src_w;
  float bh = roi->height * src_h;
  float side = fmaxf(bw, bh) * 1.10f;  /* +10% */
  float x0 = cx - side * 0.5f;
  float y0 = cy - side * 0.5f;
  float x1 = cx + side * 0.5f;
  float y1 = cy + side * 0.5f;

  const float sx_scale = (x1 - x0) / (float)dst_w;
  const float sy_scale = (y1 - y0) / (float)dst_h;

  for (int y = 0; y < dst_h; ++y) {
    float sy = y0 + (y + 0.5f) * sy_scale;
    int sy0 = (int)floorf(sy);
    int sy1 = sy0 + 1;
    float wy = sy - (float)sy0;
    sy0 = clampi(sy0, 0, src_h - 1);
    sy1 = clampi(sy1, 0, src_h - 1);

    for (int x = 0; x < dst_w; ++x) {
      float sx = x0 + (x + 0.5f) * sx_scale;
      int sx0 = (int)floorf(sx);
      int sx1 = sx0 + 1;
      float wx = sx - (float)sx0;
      sx0 = clampi(sx0, 0, src_w - 1);
      sx1 = clampi(sx1, 0, src_w - 1);

      int idx00 = (sy0 * src_w + sx0) * 3;
      int idx01 = (sy0 * src_w + sx1) * 3;
      int idx10 = (sy1 * src_w + sx0) * 3;
      int idx11 = (sy1 * src_w + sx1) * 3;

      float w00 = (1.f - wx) * (1.f - wy);
      float w01 =        wx  * (1.f - wy);
      float w10 = (1.f - wx) *        wy;
      float w11 =        wx  *        wy;

      int didx = (y * dst_w + x) * 3;
      for (int c = 0; c < 3; ++c) {
        float v = src_nhwc[idx00 + c] * w00 +
                  src_nhwc[idx01 + c] * w01 +
                  src_nhwc[idx10 + c] * w10 +
                  src_nhwc[idx11 + c] * w11;
        dst_nhwc[didx + c] = v * 2.f - 1.f;  /* [0..1] -> [-1..1] */
      }
    }
  }
}

/* Select one best detection (same scoring as runtime) */
static int select_best_face(const od_pp_out_t *pp, od_pp_outBuffer_t *best_out)
{
  if (!pp || pp->nb_detect == 0) return 0;

  const float ACCEPT_CONF_MIN = 0.70f;
  const float ACCEPT_SIDE_MIN = 0.28f;
  const float CENTER_TOL      = 0.60f;
  const float SCORE_SIDE_REF  = 0.35f;

  float best_score = -1.f;
  int best_i = -1;
  for (uint32_t i = 0; i < pp->nb_detect; ++i) {
    const od_pp_outBuffer_t *d = &pp->pOutBuff[i];
    float side = fmaxf(d->width, d->height);
    if (d->conf < ACCEPT_CONF_MIN || side < ACCEPT_SIDE_MIN) continue;

    float dx = fabsf(d->x_center - 0.5f);
    float dy = fabsf(d->y_center - 0.5f);
    float r_center = sqrtf(dx*dx + dy*dy);
    float s_size   = fminf(1.f, side / SCORE_SIDE_REF);
    float s_center = 1.f - fminf(1.f, r_center / CENTER_TOL);
    float score = d->conf * (0.25f + 0.75f * s_size) * s_center;

    if (score > best_score) { best_score = score; best_i = (int)i; *best_out = *d; }
  }
  return (best_i >= 0);
}

/* -------- main API -------- */

int FR_Extract_From_RGB888(const uint8_t *rgb,
                           int img_w, int img_h,
                           float *out_emb, int out_emb_cap,
                           od_pp_outBuffer_t *out_roi)
{
  if (!rgb || img_w <= 0 || img_h <= 0 || !out_emb || out_emb_cap <= 0) return -1;

  /* 1) DETECTOR INPUT (internal buffer) */
  const LL_Buffer_InfoTypeDef *det_in = Detector_In_Info();
  float    *det_in_f32  = (float*)  LL_Buffer_addr_start(&det_in[0]);   /* NHWC floats */
  uint32_t  det_in_len  =           LL_Buffer_len(&det_in[0]);          /* bytes */
  const int PIX = NN_WIDTH * NN_HEIGHT;
  assert(det_in_len >= (uint32_t)(PIX * 3 * sizeof(float)));

  /* Resize+normalize the source image into detector input */
  resize_rgb888_to_float_nhwc(rgb, img_w, img_h, det_in_f32, NN_WIDTH, NN_HEIGHT);

  /* Cache clean before NPU reads */
  DCACHE_Clean(det_in_f32, det_in_len);

  /* Prepare outputs for NPU write (clean+invalidate) */
  {
    const LL_Buffer_InfoTypeDef *det_out = Detector_Out_Info();
    for (int i = 0; i < NN_OUT_NB; ++i) {
      void *oaddr = LL_Buffer_addr_start(&det_out[i]);
      size_t olen = (size_t)LL_Buffer_len(&det_out[i]);
#if defined(USE_DCACHE)
      dcache_align_range(&oaddr, &olen);
      SCB_CleanInvalidateDCache_by_Addr(oaddr, (int)olen);
#else
      (void)oaddr; (void)olen;
#endif
    }
  }

  /* 2) RUN DETECTOR */
  Detector_Run();  /* NPU lock/unlock handled inside */

  /* 3) POSTPROCESS */
#if POSTPROCESS_TYPE == POSTPROCESS_CUSTOM
  static bool pp_inited = false;
  if (!pp_inited) { app_postprocess_init(NULL); pp_inited = true; }
#else
  static bool pp_inited = false;
  static yolov8_pp_static_param_t pp_params; /* adjust to your POSTPROCESS_* variant */
  if (!pp_inited) { app_postprocess_init(&pp_params); pp_inited = true; }
#endif

  const LL_Buffer_InfoTypeDef *det_out = Detector_Out_Info();
  void *pp_input[NN_OUT_NB];
  uint32_t pp_len[NN_OUT_NB];
  for (int i = 0; i < NN_OUT_NB; ++i) {
    pp_input[i] = LL_Buffer_addr_start(&det_out[i]);
    pp_len[i]   = LL_Buffer_len(&det_out[i]);
    DCACHE_Invalidate(pp_input[i], pp_len[i]);
  }

  od_pp_out_t pp_out = {0};
#if POSTPROCESS_TYPE == POSTPROCESS_CUSTOM
  int r = app_postprocess_run((void**)pp_input, NN_OUT_NB, &pp_out, NULL);
#else
  int r = app_postprocess_run((void**)pp_input, NN_OUT_NB, &pp_out, &pp_params);
#endif
  if (r != 0) return -2;

  if (pp_out.nb_detect == 0) {
    if (out_roi) memset(out_roi, 0, sizeof(*out_roi));
    return +1;  /* no face */
  }

  od_pp_outBuffer_t best = {0};
  if (!select_best_face(&pp_out, &best)) {
    if (out_roi) memset(out_roi, 0, sizeof(*out_roi));
    return +1;  /* faces exist but none passed gating */
  }
  if (out_roi) *out_roi = best;

  /* 4) PREPARE FR INPUT (internal or user buffer) */
  const LL_Buffer_InfoTypeDef *fr_in_info  = LL_ATON_Input_Buffers_Info_face_recognition();
  float    *fr_in      = (float*) LL_Buffer_addr_start(&fr_in_info[0]);  /* NHWC floats */
  uint32_t  fr_in_len  =          LL_Buffer_len(&fr_in_info[0]);

  assert(fr_in_len >= (uint32_t)(FR_IN_W * FR_IN_H * 3 * sizeof(float)));

  /* Crop the face from the SAME detector input frame (det_in_f32 in [0..1]) */
  crop_normROI_to_FR_input(det_in_f32, NN_WIDTH, NN_HEIGHT, &best,
                           fr_in, FR_IN_W, FR_IN_H);

  /* Clean FR input before NPU */
  DCACHE_Clean(fr_in, fr_in_len);

  /* Prepare FR output */
  const LL_Buffer_InfoTypeDef *fr_out_info = LL_ATON_Output_Buffers_Info_face_recognition();
  void     *fr_out_ptr = LL_Buffer_addr_start(&fr_out_info[0]);
  uint32_t  fr_out_len = LL_Buffer_len(&fr_out_info[0]);

#if defined(USE_DCACHE)
  {
    void *oaddr = fr_out_ptr;
    size_t olen = fr_out_len;
    dcache_align_range(&oaddr, &olen);
    SCB_CleanInvalidateDCache_by_Addr(oaddr, (int)olen);
  }
#endif

  /* 5) RUN FACEREC */
  FaceRec_Run_NoLock();  /* serialize externally if needed */

  /* Invalidate FR output after NPU writes */
#if defined(USE_DCACHE)
  {
    void *oaddr = fr_out_ptr;
    size_t olen = fr_out_len;
    dcache_align_range(&oaddr, &olen);
    SCB_InvalidateDCache_by_Addr(oaddr, (int)olen);
  }
#else
  DCACHE_Invalidate(fr_out_ptr, fr_out_len);
#endif

  /* 6) COPY EMBEDDING OUT (clamp size defensively) */
  int n_f32 = (int)(fr_out_len / (uint32_t)sizeof(float));
  if (n_f32 > FR_EMB_SIZE) n_f32 = FR_EMB_SIZE;
  if (n_f32 > out_emb_cap) n_f32 = out_emb_cap;
  if (n_f32 <= 0) return -3;

  memcpy(out_emb, fr_out_ptr, (size_t)n_f32 * sizeof(float));
  return 0;
}
