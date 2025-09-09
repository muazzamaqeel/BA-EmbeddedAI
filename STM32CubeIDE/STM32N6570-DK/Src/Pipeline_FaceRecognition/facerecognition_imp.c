// Facerecognition postprocess / FR thread

/* C stdlib */
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
#include "fr_helpers.h"               // fr_prepare_input_for_det(), helpers
#include "face_recognition.h"         // FR_IN_W/H, FR_EMB_SIZE, fr_* APIs
#include "facerecognition_imp.h"      // pp_thread_fct declaration


#ifndef MIN
#define MIN(a,b) (( (a) < (b) ) ? (a) : (b))
#endif

const LL_Buffer_InfoTypeDef *Detector_In_Info(void);


/* Globals defined in app.c that we use here */
extern bqueue_t nn_output_queue;
extern display_t disp;
extern volatile char g_fr_overlay_label[32];

const LL_Buffer_InfoTypeDef *LL_ATON_Input_Buffers_Info_face_recognition(void);
const LL_Buffer_InfoTypeDef *LL_ATON_Output_Buffers_Info_face_recognition(void);

void FaceRec_Run_NoLock(void);
const LL_Buffer_InfoTypeDef *Detector_Out_Info(void);

/* ---- Reference set compiled in Generated/embeddings_table.c ---- */
typedef struct { const char* name; const float* data; int dim; } EmbRec;
extern const EmbRec g_ref_set[];
extern const int    g_ref_set_count;

/* ---------------- helpers added for FR ---------------- */

static inline int clampi(int v, int lo, int hi){ return (v<lo)?lo:((v>hi)?hi:v); }

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

/* Crop detector input (float NHWC [0..1], size NN_WIDTH x NN_HEIGHT x 3)
 * to FaceRec input (float NHWC [-1..1], size FR_IN_W x FR_IN_H x 3) using a
 * square box around the ROI with a small margin. */
static void crop_to_facerec_input_from_detector_input(
    const float *src_nhwc, int src_w, int src_h,
    const od_pp_outBuffer_t *roi,
    float *dst_nhwc, int dst_w, int dst_h)
{
  /* Make square crop with +10% margin */
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

  for (int y=0; y<dst_h; ++y) {
    float sy = y0 + (y + 0.5f) * sy_scale;
    int sy0 = (int)floorf(sy);
    int sy1 = sy0 + 1;
    float wy = sy - (float)sy0;
    sy0 = clampi(sy0, 0, src_h-1);
    sy1 = clampi(sy1, 0, src_h-1);

    for (int x=0; x<dst_w; ++x) {
      float sx = x0 + (x + 0.5f) * sx_scale;
      int sx0 = (int)floorf(sx);
      int sx1 = sx0 + 1;
      float wx = sx - (float)sx0;
      sx0 = clampi(sx0, 0, src_w-1);
      sx1 = clampi(sx1, 0, src_w-1);

      int idx00 = (sy0*src_w + sx0)*3;
      int idx01 = (sy0*src_w + sx1)*3;
      int idx10 = (sy1*src_w + sx0)*3;
      int idx11 = (sy1*src_w + sx1)*3;

      float w00 = (1.f-wx)*(1.f-wy);
      float w01 = (     wx)*(1.f-wy);
      float w10 = (1.f-wx)*(     wy);
      float w11 = (     wx)*(     wy);

      int didx = (y*dst_w + x)*3;
      for (int c=0;c<3;c++){
        float v = src_nhwc[idx00+c]*w00 +
                  src_nhwc[idx01+c]*w01 +
                  src_nhwc[idx10+c]*w10 +
                  src_nhwc[idx11+c]*w11;
        /* Detector input is [0..1]; FaceRec expects [-1..1] */
        dst_nhwc[didx+c] = v*2.f - 1.f;
      }
    }
  }
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





/* Crop detector float32 frame (RGB, NHWC, [0..1]) using ROI
 * → FaceRec 160×160 uint8 input (quantized [-1..1] → u8 [0..255]).
 */
static void crop_to_facerec_input_u8_from_detector_f32(
    const float *src_nhwc,          /* full detector input frame, NHWC, floats in [0..1] */
    int src_w, int src_h,           /* detector input size (NN_WIDTH x NN_HEIGHT) */
    const od_pp_outBuffer_t *roi,   /* normalized ROI: x_center,y_center,width,height in [0..1] */
    uint8_t *dst,                   /* destination buffer (dst_w*dst_h*3) */
    int dst_w, int dst_h)
{
    /* square crop with +10% margin */
    const float cx = roi->x_center * (float)src_w;
    const float cy = roi->y_center * (float)src_h;
    const float bw = roi->width  * (float)src_w;
    const float bh = roi->height * (float)src_h;
    const float side = fmaxf(bw, bh) * 1.10f;   /* +10% */
    const float x0 = cx - side * 0.5f;
    const float y0 = cy - side * 0.5f;
    const float x1 = cx + side * 0.5f;
    const float y1 = cy + side * 0.5f;

    const float sx_scale = (x1 - x0) / (float)dst_w;
    const float sy_scale = (y1 - y0) / (float)dst_h;

    /* quantization params for [-1,1] → u8 */
    const float q_mul = 127.5f;   /* = 1 / (2/255) */
    const float q_zp  = 128.0f;

    for (int y = 0; y < dst_h; ++y) {
        float sy = y0 + (y + 0.5f) * sy_scale;
        int sy0 = (int)floorf(sy);
        int sy1 = sy0 + 1;
        float wy = sy - (float)sy0;

        if (sy0 < 0) sy0 = 0; else if (sy0 >= src_h) sy0 = src_h - 1;
        if (sy1 < 0) sy1 = 0; else if (sy1 >= src_h) sy1 = src_h - 1;

        for (int x = 0; x < dst_w; ++x) {
            float sx = x0 + (x + 0.5f) * sx_scale;
            int sx0 = (int)floorf(sx);
            int sx1 = sx0 + 1;
            float wx = sx - (float)sx0;

            if (sx0 < 0) sx0 = 0; else if (sx0 >= src_w) sx0 = src_w - 1;
            if (sx1 < 0) sx1 = 0; else if (sx1 >= src_w) sx1 = src_w - 1;

            const int idx00 = (sy0 * src_w + sx0) * 3;
            const int idx01 = (sy0 * src_w + sx1) * 3;
            const int idx10 = (sy1 * src_w + sx0) * 3;
            const int idx11 = (sy1 * src_w + sx1) * 3;

            const float w00 = (1.0f - wx) * (1.0f - wy);
            const float w01 = (       wx) * (1.0f - wy);
            const float w10 = (1.0f - wx) * (       wy);
            const float w11 = (       wx) * (       wy);

            /* bilinear in RGB, input is [0..1] */
            float r = src_nhwc[idx00+0]*w00 + src_nhwc[idx01+0]*w01 +
                      src_nhwc[idx10+0]*w10 + src_nhwc[idx11+0]*w11;
            float g = src_nhwc[idx00+1]*w00 + src_nhwc[idx01+1]*w01 +
                      src_nhwc[idx10+1]*w10 + src_nhwc[idx11+1]*w11;
            float b = src_nhwc[idx00+2]*w00 + src_nhwc[idx01+2]*w01 +
                      src_nhwc[idx10+2]*w10 + src_nhwc[idx11+2]*w11;

            /* map [0..1] → [-1,1] */
            float rq = r * 2.0f - 1.0f;
            float gq = g * 2.0f - 1.0f;
            float bq = b * 2.0f - 1.0f;

            /* quantize: round(x*127.5 + 128), clamp to [0,255] */
            const int d = (y * dst_w + x) * 3;

            int qr = (int)lrintf(rq * q_mul + q_zp);
            int qg = (int)lrintf(gq * q_mul + q_zp);
            int qb = (int)lrintf(bq * q_mul + q_zp);

            if (qr < 0) qr = 0; else if (qr > 255) qr = 255;
            if (qg < 0) qg = 0; else if (qg > 255) qg = 255;
            if (qb < 0) qb = 0; else if (qb > 255) qb = 255;

            dst[d+0] = (uint8_t)qr;
            dst[d+1] = (uint8_t)qg;
            dst[d+2] = (uint8_t)qb;
        }
    }
}


/* Crop detector float32 frame (RGB, NHWC, [0..1]) using ROI
 * → FaceRec 160×160 uint8 input (quantized [-1..1] → u8 [0..255]).
 * margin: e.g. 0.20f means +20% side expansion.
 */
static void crop_to_facerec_input_u8_from_detector_f32_margin(
    const float *src_nhwc,          /* full detector input frame, NHWC, floats in [0..1] */
    int src_w, int src_h,           /* detector input size (NN_WIDTH x NN_HEIGHT) */
    const od_pp_outBuffer_t *roi,   /* normalized ROI: x_center,y_center,width,height in [0..1] */
    float margin,
    uint8_t *dst,                   /* destination buffer (dst_w*dst_h*3) */
    int dst_w, int dst_h)
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

    /* quantization params for [-1,1] → u8 bits that map to int8 on the NPU */
    const float q_mul = 127.5f;   /* = 1 / (2/255) */
    const float q_zp  = 128.0f;

    for (int y = 0; y < dst_h; ++y) {
        float sy = y0 + (y + 0.5f) * sy_scale;
        int sy0 = (int)floorf(sy);
        int sy1 = sy0 + 1;
        float wy = sy - (float)sy0;

        if (sy0 < 0) sy0 = 0; else if (sy0 >= src_h) sy0 = src_h - 1;
        if (sy1 < 0) sy1 = 0; else if (sy1 >= src_h) sy1 = src_h - 1;

        for (int x = 0; x < dst_w; ++x) {
            float sx = x0 + (x + 0.5f) * sx_scale;
            int sx0 = (int)floorf(sx);
            int sx1 = sx0 + 1;
            float wx = sx - (float)sx0;

            if (sx0 < 0) sx0 = 0; else if (sx0 >= src_w) sx0 = src_w - 1;
            if (sx1 < 0) sx1 = 0; else if (sx1 >= src_w) sx1 = src_w - 1;

            const int idx00 = (sy0 * src_w + sx0) * 3;
            const int idx01 = (sy0 * src_w + sx1) * 3;
            const int idx10 = (sy1 * src_w + sx0) * 3;
            const int idx11 = (sy1 * src_w + sx1) * 3;

            const float w00 = (1.0f - wx) * (1.0f - wy);
            const float w01 = (       wx) * (1.0f - wy);
            const float w10 = (1.0f - wx) * (       wy);
            const float w11 = (       wx) * (       wy);

            /* bilinear in RGB, input is [0..1] */
            float r = src_nhwc[idx00+0]*w00 + src_nhwc[idx01+0]*w01 +
                      src_nhwc[idx10+0]*w10 + src_nhwc[idx11+0]*w11;
            float g = src_nhwc[idx00+1]*w00 + src_nhwc[idx01+1]*w01 +
                      src_nhwc[idx10+1]*w10 + src_nhwc[idx11+1]*w11;
            float b = src_nhwc[idx00+2]*w00 + src_nhwc[idx01+2]*w01 +
                      src_nhwc[idx10+2]*w10 + src_nhwc[idx11+2]*w11;

            /* map [0..1] → [-1,1] */
            float rq = r * 2.0f - 1.0f;
            float gq = g * 2.0f - 1.0f;
            float bq = b * 2.0f - 1.0f;

            /* quantize: round(x*127.5 + 128), clamp to [0,255] */
            const int d = (y * dst_w + x) * 3;

            int qr = (int)lrintf(rq * q_mul + q_zp);
            int qg = (int)lrintf(gq * q_mul + q_zp);
            int qb = (int)lrintf(bq * q_mul + q_zp);

            if (qr < 0) qr = 0; else if (qr > 255) qr = 255;
            if (qg < 0) qg = 0; else if (qg > 255) qg = 255;
            if (qb < 0) qb = 0; else if (qb > 255) qb = 255;

            dst[d+0] = (uint8_t)qr;
            dst[d+1] = (uint8_t)qg;
            dst[d+2] = (uint8_t)qb;
        }
    }
}



/* Crop detector float32 frame (RGB, NHWC, [0..1]) using ROI
 * → FaceRec 160×160 quantized input (INT8 sym or U8 asym), with margin.
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
    int dst_w, int dst_h)
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

#if FR_INPUT_IS_U8
    /* U8 asymmetric: [-1,1] → round(x*(255/2) + 128) = round(x*127.5 + 128) */
    const float q_mul = 127.5f;
    const float q_zp  = 128.0f;
#else
    /* INT8 symmetric: [-1,1] → round(x*127) (clamp to [-128,127]) */
    const float q_mul = 127.0f;
#endif

#if !FR_INPUT_IS_U8
    int8_t *dst_i8 = dst;
#else
    uint8_t *dst_u8 = dst;
#endif

    for (int y = 0; y < dst_h; ++y) {
        float sy = y0 + (y + 0.5f) * sy_scale;
        int sy0 = (int)floorf(sy);
        int sy1 = sy0 + 1;
        float wy = sy - (float)sy0;
        if (sy0 < 0) sy0 = 0; else if (sy0 >= src_h) sy0 = src_h - 1;
        if (sy1 < 0) sy1 = 0; else if (sy1 >= src_h) sy1 = src_h - 1;

        for (int x = 0; x < dst_w; ++x) {
            float sx = x0 + (x + 0.5f) * sx_scale;
            int sx0 = (int)floorf(sx);
            int sx1 = sx0 + 1;
            float wx = sx - (float)sx0;
            if (sx0 < 0) sx0 = 0; else if (sx0 >= src_w) sx0 = src_w - 1;
            if (sx1 < 0) sx1 = 0; else if (sx1 >= src_w) sx1 = src_w - 1;

            const int idx00 = (sy0 * src_w + sx0) * 3;
            const int idx01 = (sy0 * src_w + sx1) * 3;
            const int idx10 = (sy1 * src_w + sx0) * 3;
            const int idx11 = (sy1 * src_w + sx1) * 3;

            const float w00 = (1.0f - wx) * (1.0f - wy);
            const float w01 = (       wx) * (1.0f - wy);
            const float w10 = (1.0f - wx) * (       wy);
            const float w11 = (       wx) * (       wy);

            /* bilinear in RGB, input is [0..1] */
            float r = src_nhwc[idx00+0]*w00 + src_nhwc[idx01+0]*w01 +
                      src_nhwc[idx10+0]*w10 + src_nhwc[idx11+0]*w11;
            float g = src_nhwc[idx00+1]*w00 + src_nhwc[idx01+1]*w01 +
                      src_nhwc[idx10+1]*w10 + src_nhwc[idx11+1]*w11;
            float b = src_nhwc[idx00+2]*w00 + src_nhwc[idx01+2]*w01 +
                      src_nhwc[idx10+2]*w10 + src_nhwc[idx11+2]*w11;

            /* map [0..1] → [-1,1] */
            float rq = r * 2.0f - 1.0f;
            float gq = g * 2.0f - 1.0f;
            float bq = b * 2.0f - 1.0f;

            const int d = (y * dst_w + x) * 3;

#if FR_INPUT_IS_U8
            int qr = (int)lrintf(rq * q_mul + q_zp);
            int qg = (int)lrintf(gq * q_mul + q_zp);
            int qb = (int)lrintf(bq * q_mul + q_zp);
            if (qr < 0) qr = 0; else if (qr > 255) qr = 255;
            if (qg < 0) qg = 0; else if (qg > 255) qg = 255;
            if (qb < 0) qb = 0; else if (qb > 255) qb = 255;
            dst_u8[d+0] = (uint8_t)qr;
            dst_u8[d+1] = (uint8_t)qg;
            dst_u8[d+2] = (uint8_t)qb;
#else
            int qr = (int)lrintf(rq * q_mul);
            int qg = (int)lrintf(gq * q_mul);
            int qb = (int)lrintf(bq * q_mul);
            if (qr < -128) qr = -128; else if (qr > 127) qr = 127;
            if (qg < -128) qg = -128; else if (qg > 127) qg = 127;
            if (qb < -128) qb = -128; else if (qb > 127) qb = 127;
            dst_i8[d+0] = (int8_t)qr;
            dst_i8[d+1] = (int8_t)qg;
            dst_i8[d+2] = (int8_t)qb;
#endif
        }
    }
}




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

  /* ==== Tuning knobs ==== */
  const float     ACCEPT_CONF_MIN    = 0.70f;
  const float     ACCEPT_SIDE_MIN    = 0.28f;
  const float     CENTER_TOL         = 0.60f;
  const float     SCORE_SIDE_REF     = 0.35f;
  const float     STABLE_IOU_MIN     = 0.30f;
  const int       HOLD_MISS_FRAMES   = 6;
  const float     EMA_ALPHA          = 0.70f;

  const uint32_t  FR_PERIOD_MS       = 250;        /* ~4 Hz */
  const float     FR_MATCH_THR       = 0.50f;      /* <<= slightly easier while tuning */
  const float     FR_CROP_MARGIN     = 0.20f;      /* <<= +20% box expansion helps alignment */
  const int       FR_TOPK            = 3;          /* print top-3 scores for diagnosis */

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

  uint8_t *fr_in   = (uint8_t *)LL_Buffer_addr_start(&fr_in_info[0]);  /* u8 input */
  int8_t  *fr_out  = (int8_t  *)LL_Buffer_addr_start(&fr_out_info[0]); /* i8 output */
  uint32_t fr_in_len  = LL_Buffer_len(&fr_in_info[0]);
  uint32_t fr_out_len = LL_Buffer_len(&fr_out_info[0]);

  const uint32_t need_in_bytes = (uint32_t)(FR_IN_W * FR_IN_H * 3);  /* 160*160*3 = 76800 */

  if (!fr_in || !fr_out || fr_in_len < need_in_bytes || fr_out_len < FR_EMB_SIZE) {
    printf("[FR][ERR] IO invalid: in=%p (%lu) out=%p (%lu) need_in=%lu need_out=%u\r\n",
           (void*)fr_in, (unsigned long)fr_in_len,
           (void*)fr_out, (unsigned long)fr_out_len,
           (unsigned long)need_in_bytes, (unsigned)FR_EMB_SIZE);
  } else {
    printf("[FR][IO] IN=%p (%lu) OUT=%p (%lu)  (expected in=%lu out=%u)\r\n",
           (void*)fr_in, (unsigned long)fr_in_len,
           (void*)fr_out, (unsigned long)fr_out_len,
           (unsigned long)need_in_bytes, (unsigned)FR_EMB_SIZE);
  }

  /* Detector float input (NHWC [0..1]) for cropping */
  const LL_Buffer_InfoTypeDef *det_in_info = Detector_In_Info();
  const float *det_in = (const float*)LL_Buffer_addr_start(&det_in_info[0]);
  const uint32_t det_in_len = LL_Buffer_len(&det_in_info[0]);
  (void)det_in_len; /* expected NN_WIDTH*NN_HEIGHT*3*sizeof(float) */

  /* Simple stabilizer for a single primary box */
  typedef struct {
    od_pp_outBuffer_t roi;
    int   have;
    int   visible_count;
    int   missing_count;
    uint32_t last_seen_ms;
  } primary_t;
  static primary_t primary = {0};

  /* reference set */
  extern const EmbRec g_ref_set[];
  extern const int    g_ref_set_count;

  uint32_t last_fr_ms = 0;

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
        float side = fmaxf(best.width, best.height);
        float dx = fabsf(best.x_center - 0.5f);
        float dy = fabsf(best.y_center - 0.5f);
        float r_center = sqrtf(dx*dx + dy*dy);
        printf("[PP] best_det idx=%d conf=%.2f side=%.3f r_center=%.3f\r\n", best_i, best.conf, side, r_center);
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
    if (primary.have && det_in && fr_in && fr_out && (now - last_fr_ms >= FR_PERIOD_MS)) {
      last_fr_ms = now;

      /* Prepare FR input: detector float [0..1] -> FR u8 [0..255] 160x160x3, with margin */
      crop_to_facerec_input_u8_from_detector_f32_margin(
        det_in, NN_WIDTH, NN_HEIGHT,
        &primary.roi,
        FR_CROP_MARGIN,
        fr_in, FR_IN_W, FR_IN_H);

      /* Cache: CPU -> NPU */
      DCACHE_Clean(fr_in, need_in_bytes);

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
      const int probe_dim = (int)MIN(FR_EMB_SIZE, (int)(fr_out_len)); /* fr_out_len == 512 bytes */
      for (int i = 0; i < probe_dim; ++i) {
        probe_f[i] = (float)fr_out[i] * 0.0078125f;  /* 1/128 */
      }

      /* Compare to reference set — keep Top-K for debug */
      int   best_i = -1;
      float best_s = -2.0f;

      /* simple Top-K heap (K small) */
      int   top_idx[8];   float top_val[8];
      const int K = (FR_TOPK <= 8 ? FR_TOPK : 8);
      for (int k=0;k<K;k++){ top_idx[k] = -1; top_val[k] = -2.0f; }

      for (int i = 0; i < g_ref_set_count; ++i) {
        int dim = (probe_dim < g_ref_set[i].dim) ? probe_dim : g_ref_set[i].dim;
        if (dim <= 0) continue;
        float s = cosine_sim(probe_f, g_ref_set[i].data, dim);

        if (s > best_s) { best_s = s; best_i = i; }

        /* insert into small Top-K */
        for (int k=0;k<K;k++){
          if (s > top_val[k]) {
            for (int m=K-1; m>k; --m) { top_val[m]=top_val[m-1]; top_idx[m]=top_idx[m-1]; }
            top_val[k] = s; top_idx[k] = i; break;
          }
        }
      }

      const char *name = "Unknown";
      if (best_i >= 0 && g_ref_set[best_i].name && g_ref_set[best_i].name[0] && best_s >= FR_MATCH_THR) {
        name = g_ref_set[best_i].name;
      }

      /* Log Top-K to understand “almost matches” */
      printf("[FR] match: %s  cos=%.3f  (dt=%lums)\r\n", name, best_s, (unsigned long)dt);
      if (K > 0) {
        printf("[FR][Top%d] ", K);
        for (int k=0;k<K;k++){
          if (top_idx[k] >= 0) {
            const char* nm = (g_ref_set[top_idx[k]].name && g_ref_set[top_idx[k]].name[0]) ? g_ref_set[top_idx[k]].name : "?";
            printf("%s:%.2f%s", nm, top_val[k], (k+1<K && top_idx[k+1]>=0) ? " | " : "\r\n");
          }
        }
      }

      /* Update overlay */
      snprintf((char*)g_fr_overlay_label, sizeof(g_fr_overlay_label), "%s  %.2f", name, best_s);
    }

    /* -------- publish detection only (overlay text is global) -------- */
    int lock = xSemaphoreTake(disp.lock, portMAX_DELAY);  assert(lock == pdTRUE);

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

    lock = xSemaphoreGive(disp.lock);  assert(lock == pdTRUE);

    bqueue_put_free(&nn_output_queue);
    xSemaphoreGive(disp.update);
  }
}
