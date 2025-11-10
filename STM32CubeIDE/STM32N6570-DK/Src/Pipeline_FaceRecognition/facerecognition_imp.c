// Facerecognition postprocess / FR thread
//
// Adds deep diagnostics to find accuracy regressions:
//  - Detects FR input quantization mismatch (U8 vs I8).
//  - Prints FR input stats (global + per-channel), ROI/crop/margin details,
//    and % of bilinear taps that were clamped to image borders.
//  - Prints FR output stats (int8 min/max/mean, dequant L2 norm, NaN check).
//  - Audits reference set at startup and prints Top-K matches per frame.
//  - Tracks embedding self-consistency over time.
//  - Uses per-channel crop normalization toward mid-gray for U8 paths.
//  - Optional gamma lift for dim crops.
//  - Hysteresis-based ID lock to avoid flicker.
//  - Option to flip FR input color order (BGR vs RGB).
//
// Build-time knobs (override with -D):
//   FR_INPUT_IS_U8=0/1      (0 = INT8 symmetric input; 1 = U8 zp=127 input)
//   FR_INPUT_IS_BGR=0/1     (swap R<->B for FR only; detector stays RGB)
//   FR_TOPK=3
//   FR_CROP_MARGIN=0.20f
//   FR_MATCH_THR=0.50f      (legacy single-threshold; used as THR_ON default)
//   FR_THR_ON=0.50f         (lock if candidate >= THR_ON)
//   FR_THR_OFF=0.40f        (keep lock while score >= THR_OFF)
//   FR_LOCK_FORGET_FRAMES=15
//   FR_PERIOD_MS=250
//   FR_ENABLE_BRIGHTEN=1
//   FR_BRIGHTEN_TARGET=0.50f      (≈127 in U8)
//   FR_BRIGHTEN_GAIN_MIN=0.80f
//   FR_BRIGHTEN_GAIN_MAX=1.50f
//   FR_ENABLE_GAMMA=1
//   FR_GAMMA=0.92f                (lift mids if dim)
//   FR_GAMMA_THRESH=0.45f         (apply gamma if meanRGB < thresh)
//
// NOTE: FR output is INT8 (scale≈1/128, zp=0). That part stays unchanged.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <assert.h>

/* STM32 / RTOS */
#include "stm32n6xx_hal.h"
#include "FreeRTOS.h"
#include "semphr.h"

/* App & runtime */
#include "app_config.h"               // POSTPROCESS_TYPE, etc.
#include "network.h"                  // NN_* macros (NN_OUT_NB, NN_WIDTH/HEIGHT, etc.)
#include "cache_utils.h"              // DCACHE_* helpers
#include "npu_guard.h"                // NPU_Lock / TAG_FR
#include "ll_aton_runtime.h"          // LL_Buffer_*, HAL bindings
#include "app_shared.h"               // display_t, bqueue_t, od types (via postproc), prototypes
#include "app_postprocess.h"          // od_pp_out_t / od_pp_outBuffer_t, app_postprocess_*()
#include "fr_helpers.h"               // helpers (frame snapshot, etc.)
#include "face_recognition.h"         // FR_IN_W/H, FR_EMB_SIZE, FR_IN_IS_UINT8
#include "facerecognition_imp.h"      // pp_thread_fct declaration
#include "app_ui_pin.h"
#include "app_ui_pin_face_rec.h"
#include "crypto_utils.h"
#include "refset_bin.h"   // new SD + AES loader
#include "app_change_pin.h"

/* If the generated header that declares these isn't included by your BSP,
 * keep these forward decls to avoid implicit-decl warnings. */
const LL_Buffer_InfoTypeDef *LL_ATON_Input_Buffers_Info_face_recognition(void);
const LL_Buffer_InfoTypeDef *LL_ATON_Output_Buffers_Info_face_recognition(void);

#ifndef MIN
#define MIN(a,b) (( (a) < (b) ) ? (a) : (b))
#endif

/* ====== Build-time knobs (override via -D at compile) ====== */
/* Default to the model-generated quant choice unless caller overrides. */
#ifndef FR_INPUT_IS_U8
#define FR_INPUT_IS_U8  FR_IN_IS_UINT8  /* 0 = I8 symmetric input; 1 = U8 zp=127 input */
#endif
#ifndef FR_INPUT_IS_BGR
#define FR_INPUT_IS_BGR 0              /* 0=RGB (default), 1=BGR swap for FR only */
#endif

/* Quantization params for U8 input (match TFLite: scale=1/128, zp=127) */
#ifndef FR_IN_INV_SCALE
#define FR_IN_INV_SCALE  128.0f   /* 1 / 0.0078125f */
#endif
#ifndef FR_IN_ZP
#define FR_IN_ZP         127.0f
#endif

/* Optional: per-channel brightness normalization for dim scenes (two-pass). */
#ifndef FR_ENABLE_BRIGHTEN
#define FR_ENABLE_BRIGHTEN       1
#endif
#ifndef FR_BRIGHTEN_TARGET
#define FR_BRIGHTEN_TARGET       0.50f   /* aim mid-gray in [0..1] (≈127 for U8) */
#endif
#ifndef FR_BRIGHTEN_GAIN_MIN
#define FR_BRIGHTEN_GAIN_MIN     0.80f
#endif
#ifndef FR_BRIGHTEN_GAIN_MAX
#define FR_BRIGHTEN_GAIN_MAX     1.60f
#endif

#ifndef FR_ENABLE_GAMMA
#define FR_ENABLE_GAMMA          1
#endif
#ifndef FR_GAMMA
#define FR_GAMMA                 0.92f   /* <1 brightens mids */
#endif
#ifndef FR_GAMMA_THRESH
#define FR_GAMMA_THRESH          0.45f
#endif

#ifndef FR_TOPK
#define FR_TOPK  3
#endif

#ifndef FR_CROP_MARGIN
#define FR_CROP_MARGIN  0.20f
#endif

#ifndef FR_MATCH_THR
#define FR_MATCH_THR    0.50f
#endif
#ifndef FR_THR_ON
#define FR_THR_ON       FR_MATCH_THR
#endif
#ifndef FR_THR_OFF
#define FR_THR_OFF      0.40f
#endif
#ifndef FR_LOCK_FORGET_FRAMES
#define FR_LOCK_FORGET_FRAMES  15
#endif

#ifndef FR_PERIOD_MS
#define FR_PERIOD_MS    250U
#endif

#ifndef FR_ACCEPT_CONF_MIN
#define FR_ACCEPT_CONF_MIN   0.70f
#endif
#ifndef FR_ACCEPT_SIDE_MIN
#define FR_ACCEPT_SIDE_MIN   0.28f
#endif
#ifndef FR_CENTER_TOL
#define FR_CENTER_TOL        0.60f
#endif
#ifndef FR_SCORE_SIDE_REF
#define FR_SCORE_SIDE_REF    0.35f
#endif
#ifndef FR_STABLE_IOU_MIN
#define FR_STABLE_IOU_MIN    0.30f
#endif
#ifndef FR_HOLD_MISS_FRAMES
#define FR_HOLD_MISS_FRAMES  6
#endif
#ifndef FR_EMA_ALPHA
#define FR_EMA_ALPHA         0.70f
#endif

/* Debug cadence (ms) for verbose prints */
#ifndef DBG_EVERY_MS
#define DBG_EVERY_MS  2000U
#endif

/* Reference set quick audit prints */
#ifndef DBG_AUDIT_REFSET
#define DBG_AUDIT_REFSET  1
#endif

/* Additional per-frame prints (set to 0 to reduce noise) */
#ifndef DBG_PRINT_TOPK
#define DBG_PRINT_TOPK  1
#endif
#ifndef DBG_PRINT_IN_STATS
#define DBG_PRINT_IN_STATS  1
#endif
#ifndef DBG_PRINT_OUT_STATS
#define DBG_PRINT_OUT_STATS 1
#endif
#ifndef DBG_PRINT_ROI
#define DBG_PRINT_ROI  1
#endif

/* Prototypes provided elsewhere */
const LL_Buffer_InfoTypeDef *Detector_In_Info(void);
const LL_Buffer_InfoTypeDef *Detector_Out_Info(void);
void FaceRec_Run_NoLock(void);

/* Globals defined in app.c that we use here */
extern bqueue_t nn_output_queue;
extern display_t disp;
extern volatile char g_fr_overlay_label[32];


/* ---- Reference set now handled dynamically via refset_bin.h ---- */
// typedef struct { const char* name; const float* data; int dim; } EmbRec;
// extern EmbRec* g_ref_set;
// extern int     g_ref_set_count;
// extern void    FR_BuildCombinedRefset(void);



/* ---------------- helpers ---------------- */

static inline int clampi(int v, int lo, int hi){ return (v<lo)?lo:((v>hi)?hi:v); }
static inline float clampf(float v, float lo, float hi){ return (v<lo)?lo:((v>hi)?hi:v); }

static float cosine_sim(const float *a, const float *b, int n)
{
  /* robust FP accumulation */
  double ab = 0.0, aa = 0.0, bb = 0.0;
  for (int i = 0; i < n; ++i) {
    double x = a[i], y = b[i];
    ab += x * y;  aa += x * x;  bb += y * y;
  }
  if (aa <= 0.0 || bb <= 0.0) return 0.0f;
  double d = ab / (sqrt(aa) * sqrt(bb));
  if (d >  1.0) d =  1.0;
  if (d < -1.0) d = -1.0;
  return (float)d;
}

static inline float iou_norm_boxes(const od_pp_outBuffer_t *a, const od_pp_outBuffer_t *b)
{
  float ax0 = a->x_center - a->width*0.5f,  ay0 = a->y_center - a->height*0.5f;
  float ax1 = a->x_center + a->width*0.5f,  ay1 = a->y_center + a->height*0.5f;
  float bx0 = b->x_center - b->width*0.5f,  by0 = b->y_center - b->height*0.5f;
  float bx1 = b->x_center + b->width*0.5f,  by1 = b->y_center + b->height*0.5f;
  float ix0 = fmaxf(ax0, bx0), iy0 = fmaxf(ay0, by0);
  float ix1 = fminf(ax1, bx1), iy1 = fminf(ay1, by1);
  float iw = fmaxf(0.f, ix1 - ix0), ih = fmaxf(0.f, iy1 - iy0);
  float inter = iw * ih;
  float aarea = fmaxf(0.f, ax1-ax0) * fmaxf(0.f, ay1-ay0);
  float barea = fmaxf(0.f, bx1-bx0) * fmaxf(0.f, by1-by0);
  float uni = aarea + barea - inter + 1e-6f;
  return inter / uni;
}

/* Debug metrics for cropping */
typedef struct {
  float x0, y0, x1, y1;     /* crop window in pixels */
  float side, roi_w, roi_h; /* side after margin, raw roi dims */
  int   taps, taps_clamped; /* how many bilinear taps hit image border */
  float mean_r, mean_g, mean_b; /* pre-quant means in [0..1] */
} CropDbg;

/* ---- Bilinear sampler (RGB from NHWC [0..1]) ---- */
static inline void bilinear_rgb_sample(
    const float *src_nhwc, int src_w, int src_h,
    float fx, float fy,
    float *r, float *g, float *b,
    int *hit_border)
{
  int sx0 = (int)floorf(fx);
  int sy0 = (int)floorf(fy);
  int sx1 = sx0 + 1;
  int sy1 = sy0 + 1;

  int csx0 = sx0, csx1 = sx1, csy0 = sy0, csy1 = sy1;
  if (csx0 < 0) csx0 = 0; else if (csx0 >= src_w) csx0 = src_w - 1;
  if (csx1 < 0) csx1 = 0; else if (csx1 >= src_w) csx1 = src_w - 1;
  if (csy0 < 0) csy0 = 0; else if (csy0 >= src_h) csy0 = src_h - 1;
  if (csy1 < 0) csy1 = 0; else if (csy1 >= src_h) csy1 = src_h - 1;

  float wx = fx - (float)sx0;
  float wy = fy - (float)sy0;

  const int idx00 = (csy0 * src_w + csx0) * 3;
  const int idx01 = (csy0 * src_w + csx1) * 3;
  const int idx10 = (csy1 * src_w + csx0) * 3;
  const int idx11 = (csy1 * src_w + csx1) * 3;

  const float w00 = (1.0f - wx) * (1.0f - wy);
  const float w01 = (       wx) * (1.0f - wy);
  const float w10 = (1.0f - wx) * (       wy);
  const float w11 = (       wx) * (       wy);

  *r = src_nhwc[idx00+0]*w00 + src_nhwc[idx01+0]*w01 +
       src_nhwc[idx10+0]*w10 + src_nhwc[idx11+0]*w11;
  *g = src_nhwc[idx00+1]*w00 + src_nhwc[idx01+1]*w01 +
       src_nhwc[idx10+1]*w10 + src_nhwc[idx11+1]*w11;
  *b = src_nhwc[idx00+2]*w00 + src_nhwc[idx01+2]*w01 +
       src_nhwc[idx10+2]*w10 + src_nhwc[idx11+2]*w11;

  if (hit_border) {
    int clamped = (csx0 != sx0) || (csx1 != sx1) || (csy0 != sy0) || (csy1 != sy1);
    *hit_border = clamped;
  }
}

/* Crop detector float32 frame (RGB, NHWC, [0..1]) using ROI
 * → FaceRec 160×160 quantized input (INT8 sym or U8 asym), with margin.
 * Matches TFLite U8 input quantization: scale=1/128, zp=127 when FR_INPUT_IS_U8=1.
 * Optional per-channel brightness normalization and gamma lift.
 */
static void crop_to_facerec_input_quant_from_detector_f32(
    const float *src_nhwc,  int src_w, int src_h,
    const od_pp_outBuffer_t *roi,
    float margin,           /* e.g. 0.20f = +20% */
#if FR_INPUT_IS_U8
    uint8_t *dst,
#else
    int8_t  *dst,
#endif
    int dst_w, int dst_h,
    CropDbg *dbg /* optional */)
{
  /* square crop with configurable margin */
  const float cx = roi->x_center * (float)src_w;
  const float cy = roi->y_center * (float)src_h;
  const float bw = roi->width  * (float)src_w;
  const float bh = roi->height * (float)src_h;
  const float side0 = fmaxf(bw, bh);
  const float side  = side0 * (1.0f + margin);
  const float x0 = cx - side * 0.5f;
  const float y0 = cy - side * 0.5f;
  const float x1 = cx + side * 0.5f;
  const float y1 = cy + side * 0.5f;

  const float sx_scale = (x1 - x0) / (float)dst_w;
  const float sy_scale = (y1 - y0) / (float)dst_h;

  int taps = 0, clamped = 0;
  double acc_r = 0.0, acc_g = 0.0, acc_b = 0.0;

  float gain_r = 1.0f, gain_g = 1.0f, gain_b = 1.0f;
  float apply_gamma = 0.0f;

#if FR_ENABLE_BRIGHTEN
  /* ----- Pass 1: estimate per-channel brightness in the crop ----- */
  for (int y = 0; y < dst_h; ++y) {
    float sy = y0 + (y + 0.5f) * sy_scale;
    for (int x = 0; x < dst_w; ++x) {
      float sx = x0 + (x + 0.5f) * sx_scale;
      float r,g,b; int hit=0;
      bilinear_rgb_sample(src_nhwc, src_w, src_h, sx, sy, &r, &g, &b, &hit);
      acc_r += r; acc_g += g; acc_b += b;
      taps++; if (hit) clamped++;
    }
  }
  double denom = (double)(dst_w * dst_h);
  float mean_r = (float)(acc_r / denom);
  float mean_g = (float)(acc_g / denom);
  float mean_b = (float)(acc_b / denom);
  float mean_rgb = (mean_r + mean_g + mean_b) * (1.0f/3.0f);

  /* Per-channel gains toward target */
  if (mean_r > 1e-6f) gain_r = clampf(FR_BRIGHTEN_TARGET / mean_r, FR_BRIGHTEN_GAIN_MIN, FR_BRIGHTEN_GAIN_MAX);
  if (mean_g > 1e-6f) gain_g = clampf(FR_BRIGHTEN_TARGET / mean_g, FR_BRIGHTEN_GAIN_MIN, FR_BRIGHTEN_GAIN_MAX);
  if (mean_b > 1e-6f) gain_b = clampf(FR_BRIGHTEN_TARGET / mean_b, FR_BRIGHTEN_GAIN_MIN, FR_BRIGHTEN_GAIN_MAX);

  /* Decide whether to lift mids with gamma */
#if FR_ENABLE_GAMMA
  if (mean_rgb < FR_GAMMA_THRESH) apply_gamma = FR_GAMMA;
#endif

  /* reset accumulators if dbg requested (so they reflect post-gain means) */
  if (dbg) { acc_r = acc_g = acc_b = 0.0; taps = 0; clamped = 0; }
#endif /* FR_ENABLE_BRIGHTEN */

  /* ----- Pass 2: apply gains/gamma (if enabled), map, quantize ----- */
  for (int y = 0; y < dst_h; ++y) {
    float sy = y0 + (y + 0.5f) * sy_scale;
    for (int x = 0; x < dst_w; ++x) {
      float sx = x0 + (x + 0.5f) * sx_scale;
      float r,g,b; int hit=0;
      bilinear_rgb_sample(src_nhwc, src_w, src_h, sx, sy, &r, &g, &b, &hit);

#if FR_ENABLE_BRIGHTEN
      r = clampf(r*gain_r, 0.0f, 1.0f);
      g = clampf(g*gain_g, 0.0f, 1.0f);
      b = clampf(b*gain_b, 0.0f, 1.0f);
  #if FR_ENABLE_GAMMA
      if (apply_gamma > 0.0f && apply_gamma != 1.0f) {
        r = powf(r, apply_gamma);
        g = powf(g, apply_gamma);
        b = powf(b, apply_gamma);
      }
  #endif
#endif

      if (dbg) { acc_r += r; acc_g += g; acc_b += b; taps++; if (hit) clamped++; }

#if FR_INPUT_IS_BGR
      /* swap channels for FR only if requested */
      float tmp = r; r = b; b = tmp;
#endif

      /* map [0..1] → [-1,1] */
      float rq = r * 2.0f - 1.0f;
      float gq = g * 2.0f - 1.0f;
      float bq = b * 2.0f - 1.0f;

      const int d = (y * dst_w + x) * 3;
#if FR_INPUT_IS_U8
      int qr = (int)lrintf(rq * FR_IN_INV_SCALE + FR_IN_ZP);
      int qg = (int)lrintf(gq * FR_IN_INV_SCALE + FR_IN_ZP);
      int qb = (int)lrintf(bq * FR_IN_INV_SCALE + FR_IN_ZP);
      if (qr < 0)   qr = 0;   else if (qr > 255) qr = 255;
      if (qg < 0)   qg = 0;   else if (qg > 255) qg = 255;
      if (qb < 0)   qb = 0;   else if (qb > 255) qb = 255;
      ((uint8_t*)dst)[d+0] = (uint8_t)qr;
      ((uint8_t*)dst)[d+1] = (uint8_t)qg;
      ((uint8_t*)dst)[d+2] = (uint8_t)qb;
#else
      int qr = (int)lrintf(rq * 127.0f);
      int qg = (int)lrintf(gq * 127.0f);
      int qb = (int)lrintf(bq * 127.0f);
      if (qr < -128) qr = -128; else if (qr > 127) qr = 127;
      if (qg < -128) qg = -128; else if (qg > 127) qg = 127;
      if (qb < -128) qb = -128; else if (qb > 127) qb = 127;
      ((int8_t*)dst)[d+0] = (int8_t)qr;
      ((int8_t*)dst)[d+1] = (int8_t)qg;
      ((int8_t*)dst)[d+2] = (int8_t)qb;
#endif
    }
  }

  if (dbg) {
    dbg->x0 = x0; dbg->y0 = y0; dbg->x1 = x1; dbg->y1 = y1;
    dbg->side = side; dbg->roi_w = bw; dbg->roi_h = bh;
    dbg->taps = taps; dbg->taps_clamped = clamped;
    double denom2 = (double)(dst_w * dst_h);
    dbg->mean_r = (float)(acc_r / denom2);
    dbg->mean_g = (float)(acc_g / denom2);
    dbg->mean_b = (float)(acc_b / denom2);
  }
}

/* Quick utility */
static float l2_norm(const float *x, int n)
{
  double s=0.0; for (int i=0;i<n;i++){ double v=x[i]; s+=v*v; }
  return (float)sqrt(s);
}

/* Hysteresis lock for identity */
typedef struct {
  char  name[32];
  float score;
  int   bad_frames;
  int   locked;
} id_lock_t;

void pp_thread_fct(void *arg)
{
#if POSTPROCESS_TYPE == POSTPROCESS_OD_YOLO_V2_UF
  yolov2_pp_static_param_t pp_params;
#elif POSTPROCESS_TYPE == POSTPROCESS_OD_YOLO_V5_UU
  yolov5_pp_static_param_t pp_params;
#elif POSTPROCESS_TYPE == POSTPROCESS_OD_YOLO_V8_UF || POSTPROCESS_TYPE == POSTPROCESS_OD_YOLO_V8_UI
  yolov8_pp_static_param_t pp_params;
#elif POSTPROCESS_TYPE == POSTPROCESS_CUSTOM
  /* BlazeFace: no static params */
#else
# error "PostProcessing type not supported"
#endif

  const LL_Buffer_InfoTypeDef *nn_out_info = Detector_Out_Info();

  /* Log detector outputs layout once (sanity check) */
  {
    for (int i = 0; i < NN_OUT_NB; ++i) {
      const char *nm = nn_out_info[i].name ? nn_out_info[i].name : "(null)";
      unsigned long blen = (unsigned long)LL_Buffer_len(&nn_out_info[i]);
      printf("[NN] out[%d] %-24s len=%lu bytes (%lu floats)\r\n",
             i, nm, blen, blen / (unsigned long)sizeof(float));
    }
  }



  /* ---- Load encrypted reference set ---- */
  FR_LoadRefsetFromSD_Bin("0:faces");
  printf("[FR] Encrypted reference sets found: %d\r\n", g_ref_set_count);

  /* Wait for AES clocks to settle (or cmox init) */
  vTaskDelay(pdMS_TO_TICKS(2000));

  printf("[FR] Decrypting embeddings...\r\n");
  FR_DecryptAllRefsetOnce();
  printf("[FR] Decryption complete — %d embeddings ready\r\n", g_ref_set_count);

  /* ---- Quick debug of first entry (check SD parser output) ---- */
  if (g_ref_set_count > 0)
  {
      printf("[FR][DBG] First entry: %s, dim=%d, first bytes: ",
             g_ref_set[0].name, g_ref_set[0].dim);
      for (int i = 0; i < 8 && i < g_ref_set[0].dim; ++i)
    	  printf("%.4f ", g_ref_set[0].data[i]);
      printf("\r\n");
  }

  /* ---- Delayed decryption ----
   * Wait until hardware AES clocks are stable (2 s after boot)
   * before starting software/hardware AES routines.
   */
  vTaskDelay(pdMS_TO_TICKS(2000));  // 2 s delay after system start

  printf("[DEC] cmox initialized (delayed)\r\n");
  FR_DecryptAllRefsetOnce();   // 🔒 safe now
  printf("[DEC] Decryption completed successfully.\r\n");

  void *pp_input[NN_OUT_NB];
  uint32_t pp_len[NN_OUT_NB];
  od_pp_out_t pp_output;
  uint32_t nn_pp[2];
  int ret;

#if POSTPROCESS_TYPE == POSTPROCESS_CUSTOM
  app_postprocess_init(NULL);
#else
  app_postprocess_init(&pp_params);
#endif

  /* -------- FaceRec I/O discovery (INT8 pipeline) -------- */
  const LL_Buffer_InfoTypeDef *fr_in_info  = LL_ATON_Input_Buffers_Info_face_recognition();
  const LL_Buffer_InfoTypeDef *fr_out_info = LL_ATON_Output_Buffers_Info_face_recognition();

#if FR_INPUT_IS_U8
  uint8_t *fr_in = (uint8_t *)LL_Buffer_addr_start(&fr_in_info[0]);    /* U8 input */
  const char *mode_str = "U8(zp=127)";
#else
  int8_t  *fr_in = (int8_t  *)LL_Buffer_addr_start(&fr_in_info[0]);    /* I8 input */
  const char *mode_str = "I8(zp=0)";
#endif
  int8_t  *fr_out       = (int8_t  *)LL_Buffer_addr_start(&fr_out_info[0]); /* I8 output */
  uint32_t fr_in_len    = LL_Buffer_len(&fr_in_info[0]);
  uint32_t fr_out_len   = LL_Buffer_len(&fr_out_info[0]);

  const uint32_t need_in_bytes = (uint32_t)(FR_IN_W * FR_IN_H * 3);  /* e.g., 160*160*3 = 76800 */

  if (!fr_in || !fr_out || fr_in_len < need_in_bytes || fr_out_len < FR_EMB_SIZE) {
    printf("[FR][ERR] IO invalid: in=%p (%lu) out=%p (%lu) need_in=%lu need_out=%u\r\n",
           (void*)fr_in, (unsigned long)fr_in_len,
           (void*)fr_out, (unsigned long)fr_out_len,
           (unsigned long)need_in_bytes, (unsigned)FR_EMB_SIZE);
  } else {
    printf("[FR][IO] IN=%p (%lu) OUT=%p (%lu)  (expected in=%lu out=%u)  [mode=%s]%s\r\n",
           (void*)fr_in, (unsigned long)fr_in_len,
           (void*)fr_out, (unsigned long)fr_out_len,
           (unsigned long)need_in_bytes, (unsigned)FR_EMB_SIZE, mode_str,
           FR_INPUT_IS_BGR ? " [FR uses BGR]" : "");
  }

  /* Audit reference set once */
#if DBG_AUDIT_REFSET
  {
    printf("[REFSET] entries=%d\r\n", g_ref_set_count);
    int bad=0;
    for (int i=0;i<g_ref_set_count;i++){
      float n = l2_norm(g_ref_set[i].data, g_ref_set[i].dim);
      printf("  - #%d name=%s dim=%d L2=%.3f\r\n", i,
             (g_ref_set[i].name?g_ref_set[i].name:"?"),
             g_ref_set[i].dim, n);
      if (!(n>0.0f && isfinite(n))) bad++;
    }
    if (bad) printf("[REFSET][WARN] %d invalid L2 norms (zero/NaN/Inf)\r\n", bad);
  }
#endif

  /* Simple stabilizer for a single primary box */
  typedef struct {
    od_pp_outBuffer_t roi;
    int   have;
    int   visible_count;
    int   missing_count;
    uint32_t last_seen_ms;
  } primary_t;
  static primary_t primary = {0};

  /* selection/stabilization knobs (local consts for clarity) */
  const float     ACCEPT_CONF_MIN  = FR_ACCEPT_CONF_MIN;
  const float     ACCEPT_SIDE_MIN  = FR_ACCEPT_SIDE_MIN;
  const float     CENTER_TOL       = FR_CENTER_TOL;
  const float     SCORE_SIDE_REF   = FR_SCORE_SIDE_REF;
  const float     STABLE_IOU_MIN   = FR_STABLE_IOU_MIN;
  const int       HOLD_MISS_FRAMES = FR_HOLD_MISS_FRAMES;
  const float     EMA_ALPHA        = FR_EMA_ALPHA;

  /* FR throttling & thresholds */
  uint32_t last_fr_ms = 0;
  const uint32_t FR_PERIOD = FR_PERIOD_MS;
  const float    THR_ON    = FR_THR_ON;
  const float    THR_OFF   = FR_THR_OFF;

  /* Debug cadence */
  uint32_t last_dbg_ms = 0;

  /* Previous embedding for self-consistency */
  float prev_emb[FR_EMB_SIZE]; int have_prev = 0;

  /* Identity lock */
  static id_lock_t lock = {{0}, 0.0f, 0, 0};
  /* ---- Anti-retrigger (PIN gating) ---- */
  static volatile bool g_pin_session_active = false;
  static uint32_t      g_last_unlock_ms     = 0;
  static int           g_stable_count       = 0;
  static char          last_name[32]        = {0};

  while (1)
  {
    (void)bqueue_get_ready(&nn_output_queue);

    /* Map & invalidate NPU outputs for CPU read */
    for (int i = 0; i < NN_OUT_NB; i++) {
      pp_input[i] = LL_Buffer_addr_start(&nn_out_info[i]);
      pp_len[i]   = LL_Buffer_len(&nn_out_info[i]);
      DCACHE_Invalidate(pp_input[i], pp_len[i]);
    }

    /* Run detection postprocess */
    pp_output.pOutBuff  = NULL;
    pp_output.nb_detect = 0;
    nn_pp[0] = HAL_GetTick();
#if POSTPROCESS_TYPE == POSTPROCESS_CUSTOM
    ret = app_postprocess_run((void **)pp_input, NN_OUT_NB, &pp_output, NULL);
#else
    ret = app_postprocess_run((void **)pp_input, NN_OUT_NB, &pp_output, &pp_params);
#endif
    assert(ret == 0);
    nn_pp[1] = HAL_GetTick();

    uint32_t now = nn_pp[1];

    /* -------- choose ONE best detection & stabilize -------- */
    od_pp_outBuffer_t best;
    int have_best = 0;
    if (pp_output.nb_detect > 0) {
      float best_score = -1.f;
      int best_i = -1;
      for (uint32_t i = 0; i < pp_output.nb_detect; ++i) {
        const od_pp_outBuffer_t *d = &pp_output.pOutBuff[i];
        float side = fmaxf(d->width, d->height);
        if (d->conf < ACCEPT_CONF_MIN || side < ACCEPT_SIDE_MIN) continue;

        float dx = fabsf(d->x_center - 0.5f);
        float dy = fabsf(d->y_center - 0.5f);
        float r_center = sqrtf(dx*dx + dy*dy);
        float s_size   = fminf(1.f, side / SCORE_SIDE_REF);
        float s_center = 1.f - fminf(1.f, r_center / CENTER_TOL);
        float score = d->conf * (0.25f + 0.75f * s_size) * s_center;

        if (score > best_score) { best_score = score; best = *d; have_best = 1; best_i = (int)i; }
      }
      if (have_best) {
#if DBG_PRINT_ROI
        float side = fmaxf(best.width, best.height);
        float dx = fabsf(best.x_center - 0.5f);
        float dy = fabsf(best.y_center - 0.5f);
        float r_center = sqrtf(dx*dx + dy*dy);
        printf("[PP] best_det idx=%d conf=%.2f side=%.3f r_center=%.3f\r\n", best_i, best.conf, side, r_center);
#endif
      }
    }

    if (have_best) {
      if (!primary.have) {
        primary.roi = best;
        primary.have = 1;
        primary.visible_count = 1;
        primary.missing_count = 0;
        primary.last_seen_ms  = now;
      } else {
        /* IoU vs previous → smooth if “same” */
        float ov = iou_norm_boxes(&primary.roi, &best);
        if (ov >= STABLE_IOU_MIN) {
          primary.roi.x_center = EMA_ALPHA*primary.roi.x_center + (1.f-EMA_ALPHA)*best.x_center;
          primary.roi.y_center = EMA_ALPHA*primary.roi.y_center + (1.f-EMA_ALPHA)*best.y_center;
          primary.roi.width    = EMA_ALPHA*primary.roi.width    + (1.f-EMA_ALPHA)*best.width;
          primary.roi.height   = EMA_ALPHA*primary.roi.height   + (1.f-EMA_ALPHA)*best.height;
          primary.roi.conf     = best.conf;
          primary.roi.class_index = best.class_index;
          primary.visible_count++;
          primary.missing_count = 0;
          primary.last_seen_ms  = now;
        } else {
          if (primary.missing_count >= HOLD_MISS_FRAMES) {
            primary.roi = best;
            primary.visible_count = 1;
            primary.missing_count = 0;
            primary.last_seen_ms  = now;
          } else {
            primary.missing_count++;
          }
        }
      }
    } else {
      if (primary.have) {
        primary.missing_count++;
        if (primary.missing_count > HOLD_MISS_FRAMES) {
          memset(&primary, 0, sizeof(primary));
          g_fr_overlay_label[0] = '\0';  /* clear overlay */
        }
      }
    }

    /* ---------- FaceRec match (throttled) ---------- */
    if (primary.have && fr_in && fr_out && (now - last_fr_ms >= FR_PERIOD)) {
      last_fr_ms = now;

      /* Always take a fresh, race-free detector snapshot **inside** the loop */
      const float *det_in = NULL;
      uint32_t det_in_len = 0;
      fr_get_frame_snapshot((const void **)&det_in, &det_in_len);
      if (!det_in || det_in_len < (NN_WIDTH * NN_HEIGHT * 3 * sizeof(float))) {
        printf("[FR][ERR] snapshot invalid (ptr=%p len=%lu)\r\n",
               (void*)det_in, (unsigned long)det_in_len);
        /* publish empty detection & move on */
        bqueue_put_free(&nn_output_queue);
        xSemaphoreGive(disp.update);
        continue;
      }

      CropDbg cdbg;
      /* Prepare FR input: detector float [0..1] -> FR quant (I8 or U8) with margin */
      crop_to_facerec_input_quant_from_detector_f32(
        det_in, NN_WIDTH, NN_HEIGHT,
        &primary.roi,
        FR_CROP_MARGIN,
        fr_in, FR_IN_W, FR_IN_H,
        &cdbg);

      /* Cache: CPU -> NPU */
      DCACHE_Clean(fr_in, need_in_bytes);

      /* Optional periodic debug on crop + input stats */
      if ((now - last_dbg_ms) >= DBG_EVERY_MS) {
#if DBG_PRINT_ROI
        printf("[FR][crop] px=[%.1f,%.1f]-[%.1f,%.1f] side=%.1f roiWH=%.1fx%.1f clamped=%d/%d (%.1f%%) preQ_meanRGB=%.3f/%.3f/%.3f\r\n",
               cdbg.x0, cdbg.y0, cdbg.x1, cdbg.y1, cdbg.side,
               cdbg.roi_w, cdbg.roi_h,
               cdbg.taps_clamped, cdbg.taps,
               (cdbg.taps>0)?(100.f*(float)cdbg.taps_clamped/(float)cdbg.taps):0.f,
               cdbg.mean_r, cdbg.mean_g, cdbg.mean_b);
#endif
#if DBG_PRINT_IN_STATS
        /* Quick stats on quantized FR input to sanity-check quantization mode */
        const int cnt = FR_IN_W * FR_IN_H * 3;
#if FR_INPUT_IS_U8
        uint8_t mn = 255, mx = 0; long sum = 0;
        double sumR=0, sumG=0, sumB=0;
        for (int i=0;i<cnt;i+=3){
          uint8_t r=((uint8_t*)fr_in)[i+0], g=((uint8_t*)fr_in)[i+1], b=((uint8_t*)fr_in)[i+2];
          if (r<mn) mn=r; if (r>mx) mx=r; sum+=r; sumR+=r; sumG+=g; sumB+=b;
          if (g<mn) mn=g; if (g>mx) mx=g; sum+=g;
          if (b<mn) mn=b; if (b>mx) mx=b; sum+=b;
        }
        float mean = (float)sum / (float)cnt;
        float mr=(float)(sumR/(double)(cnt/3)), mg=(float)(sumG/(double)(cnt/3)), mb=(float)(sumB/(double)(cnt/3));
        printf("[FR][in] U8 stats: min=%u max=%u mean=%.1f  perC=%.1f/%.1f/%.1f (expect ~127 center)\r\n",
               (unsigned)mn, (unsigned)mx, mean, mr, mg, mb);
#else
        int8_t mn = 127, mx = -128; long sum = 0;
        double sumR=0, sumG=0, sumB=0;
        for (int i=0;i<cnt;i+=3){
          int8_t r=((int8_t*)fr_in)[i+0], g=((int8_t*)fr_in)[i+1], b=((int8_t*)fr_in)[i+2];
          if (r<mn) mn=r; if (r>mx) mx=r; sum+=r; sumR+=r; sumG+=g; sumB+=b;
          if (g<mn) mn=g; if (g>mx) mx=g; sum+=g;
          if (b<mn) mn=b; if (b>mx) mx=b; sum+=b;
        }
        float mean = (float)sum / (float)cnt;
        float mr=(float)(sumR/(double)(cnt/3)), mg=(float)(sumG/(double)(cnt/3)), mb=(float)(sumB/(double)(cnt/3));
        printf("[FR][in] I8 stats: min=%d max=%d mean=%.1f  perC=%.1f/%.1f/%.1f (expect ~0 center)\r\n",
               (int)mn, (int)mx, mean, mr, mg, mb);
#endif /* FR_INPUT_IS_U8 */
#endif /* DBG_PRINT_IN_STATS */
        last_dbg_ms = now;
      }

      /* Run FaceRec (serialized on NPU) */
      NPU_Lock(TAG_FR);
      uint32_t t0 = HAL_GetTick();
      FaceRec_Run_NoLock();
      uint32_t dt = HAL_GetTick() - t0;
      NPU_Unlock(TAG_FR);

      /* Cache: NPU -> CPU */
      DCACHE_Invalidate(fr_out, fr_out_len);

      /* Dequantize int8 embeddings → float; scale=1/128, zp=0 */
      float probe_f[FR_EMB_SIZE];
      const int probe_dim = (int)MIN(FR_EMB_SIZE, (int)(fr_out_len)); /* bytes==dims here */
      int out_min = 127, out_max = -128; long out_sum = 0;
      for (int i = 0; i < probe_dim; ++i) {
        int8_t q = fr_out[i];
        if (q < out_min) out_min = q;
        if (q > out_max) out_max = q;
        out_sum += q;
        probe_f[i] = (float)q * 0.0078125f;  /* 1/128 */
      }
      /* Debug: dump first 10 embedding floats */
      for (int i = 0; i < 10 && i < probe_dim; i++) {
        printf("[FR][vec] probe[%d] = %.4f\r\n", i, probe_f[i]);
      }

#if DBG_PRINT_OUT_STATS
      {
        float out_mean = (float)out_sum / (float)probe_dim;
        float nrm = l2_norm(probe_f, probe_dim);
        int nan_cnt=0; for(int i=0;i<probe_dim;i++){ if(!isfinite(probe_f[i])) nan_cnt++; }
        printf("[FR][out] I8 stats: min=%d max=%d mean=%.1f  L2=%.3f  NaN=%d\r\n",
               out_min, out_max, out_mean, nrm, nan_cnt);
      }
#endif

      /* ----- Compare to reference set (Top-K and per-name Top3-mean) ----- */
      int   top_idx[8];   float top_val[8];
      const int K = (FR_TOPK <= 8 ? FR_TOPK : 8);
      for (int k=0;k<K;k++){ top_idx[k] = -1; top_val[k] = -2.0f; }

      /* Compute best per-name using mean of top-3 refs for that name */
      float best_name_score = -2.0f;
      const char *best_name_ptr = NULL;

      for (int i = 0; i < g_ref_set_count; ++i) {
        const char *nm = g_ref_set[i].name ? g_ref_set[i].name : "";
        if (!nm[0]) continue;

        /* collect top-3 scores for this name */
        float t1=-2.f, t2=-2.f, t3=-2.f;
        for (int j = 0; j < g_ref_set_count; ++j) {
          if (!g_ref_set[j].name) continue;
          if (strcmp(g_ref_set[j].name, nm) != 0) continue;
          int dim = (probe_dim < g_ref_set[j].dim) ? probe_dim : g_ref_set[j].dim;
          if (dim <= 0) continue;
          float s = cosine_sim(probe_f, g_ref_set[j].data, dim);

          /* maintain global Top-K of individual refs for debug */
          for (int k=0;k<K;k++){
            if (s > top_val[k]) {
              for (int m=K-1; m>k; --m) { top_val[m]=top_val[m-1]; top_idx[m]=top_idx[m-1]; }
              top_val[k] = s; top_idx[k] = j; break;
            }
          }

          /* in-name top3 */
          if (s > t1){ t3=t2; t2=t1; t1=s; }
          else if (s > t2){ t3=t2; t2=s; }
          else if (s > t3){ t3=s; }
        }

        float mean_top3;
        if (t2 < -1.0f) {
          /* fewer than 2 examples: use max only */
          mean_top3 = t1;
        } else if (t3 < -1.0f) {
          mean_top3 = (t1 + t2) * 0.5f;
        } else {
          mean_top3 = (t1 + t2 + t3) / 3.0f;
        }

        if (mean_top3 > best_name_score) {
          best_name_score = mean_top3;
          best_name_ptr   = nm;
        }
      }

      /* Candidate (by per-name Top3-mean) */
      const char *cand_name = (best_name_ptr && best_name_score > -1.0f) ? best_name_ptr : "Unknown";
      float       cand_s    = (best_name_score > -2.0f) ? best_name_score : 0.0f;

      /* Self-consistency with previous embedding */
      if (have_prev) {
        float sself = cosine_sim(probe_f, prev_emb, probe_dim);
        printf("[FR][self] cos(prev)=%.3f\r\n", sself);
      }
      memcpy(prev_emb, probe_f, probe_dim*sizeof(float)); have_prev = 1;

      /* ----- Hysteresis lock to avoid flicker ----- */
      const char *final_name = "Unknown";
      float final_s = cand_s;

      if (!lock.locked) {
        if (cand_s >= THR_ON && cand_name && cand_name[0]) {
          snprintf(lock.name, sizeof(lock.name), "%s", cand_name);
          lock.score = cand_s;
          lock.bad_frames = 0;
          lock.locked = 1;
        }
        final_name = lock.locked ? lock.name : "Unknown";
        final_s    = lock.locked ? cand_s   : cand_s;
      } else {
        if (cand_name && strcmp(cand_name, lock.name) == 0) {
          /* same identity candidate */
          if (cand_s >= THR_OFF) {
            lock.score = cand_s;
            lock.bad_frames = 0;
          } else {
            lock.bad_frames++;
            if (lock.bad_frames > FR_LOCK_FORGET_FRAMES) {
              lock.locked = 0;
              lock.name[0] = '\0';
            }
          }
        } else {
          /* different candidate */
          if (cand_s >= THR_ON && cand_s > lock.score + 0.05f) {
            snprintf(lock.name, sizeof(lock.name), "%s", cand_name);
            lock.score = cand_s;
            lock.bad_frames = 0;
            lock.locked = 1;
          } else {
            lock.bad_frames++;
            if (lock.bad_frames > FR_LOCK_FORGET_FRAMES) {
              lock.locked = 0;
              lock.name[0] = '\0';
            }
          }
        }
        final_name = lock.locked ? lock.name : ((cand_s >= THR_ON && cand_name)?cand_name:"Unknown");
        final_s    = lock.locked ? lock.score: cand_s;
      }

      /* Log result + Top-K */
      printf("[FR] match: %s  cos=%.3f  (dt=%lums)\r\n", final_name, final_s, (unsigned long)dt);

      /* ---- PIN unlock gating ---- */
      const uint32_t now_ms = HAL_GetTick();
      const uint32_t PIN_COOLDOWN_MS = 3000;      // 3-second cooldown
      const int      STABLE_FRAMES_REQUIRED = 3;  // Require 3 consecutive stable frames

      // Track consecutive frames with same name and above threshold
      if (strcmp(final_name, last_name) == 0 && final_s >= FR_THR_ON) {
          g_stable_count++;
          printf("[FR][Stable] %s %d/%d\r\n", final_name, g_stable_count, STABLE_FRAMES_REQUIRED);
      } else {
          g_stable_count = 0;
          strncpy(last_name, final_name, sizeof(last_name));
      }

      // Trigger only if all conditions are met
      if (!g_pin_session_active &&
          strcmp(final_name, "Unknown") != 0 &&
          final_s >= FR_THR_ON &&
          g_stable_count >= STABLE_FRAMES_REQUIRED &&
          (now_ms - g_last_unlock_ms) > PIN_COOLDOWN_MS)
      {
          g_pin_session_active = true;
          g_stable_count = 0;
          g_last_unlock_ms = now_ms;

          printf("[UI] Launching PIN screen for %s...\r\n", final_name);

          if (!FR_LoadAndDecryptPinForName("0:pin", final_name)) {
              printf("[UI][ERR] No PIN file for %s, skipping PIN screen.\r\n", final_name);
          } else {
              printf("[UI] Loaded decrypted PIN for %s → %s\r\n", final_name, g_current_pin);
              UI_FR_PinScreen_Show();
              UI_FR_PinScreen_WaitForOK();
          }

          printf("[UI] PIN screen done, resuming pipeline...\r\n");

          g_pin_session_active = false;
          g_last_unlock_ms = HAL_GetTick();  // Reset cooldown timer
      }



#if DBG_PRINT_TOPK
      if (K > 0) {
        printf("[FR][Top%d] ", K);
        for (int k=0;k<K;k++){
          if (top_idx[k] >= 0) {
            const char* nm = (g_ref_set[top_idx[k]].name && g_ref_set[top_idx[k]].name[0]) ? g_ref_set[top_idx[k]].name : "?";
            printf("%s:%.2f%s", nm, top_val[k], (k+1<K && top_idx[k+1]>=0) ? " | " : "\r\n");
          }
        }
      }
#endif

      /* Update overlay */
      snprintf((char*)g_fr_overlay_label, sizeof(g_fr_overlay_label), "%s  %.2f", final_name, final_s);
    }

    /* -------- publish detection only (overlay text is global) -------- */
    BaseType_t lock_sem = xSemaphoreTake(disp.lock, portMAX_DELAY);  assert(lock_sem == pdTRUE);

    if (primary.have) {
      disp.info.nb_detect = 1;
      disp.info.detects[0] = primary.roi;
    } else {
      disp.info.nb_detect = 0;
    }

    /* Ensure tracker overlay OFF so only one box is drawn */
    disp.info.tracking_enabled = 0;
#ifdef TRACKER_MODULE
    disp.info.tboxes_valid_nb = 0;
#endif
    disp.info.pp_ms = nn_pp[1] - nn_pp[0];

    lock_sem = xSemaphoreGive(disp.lock);  assert(lock_sem == pdTRUE);

    bqueue_put_free(&nn_output_queue);
    xSemaphoreGive(disp.update);
  }
}
