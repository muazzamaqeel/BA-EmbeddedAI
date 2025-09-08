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

  /* Log outputs layout once */
  {
    for (int i = 0; i < NN_OUT_NB; ++i) {
      const char *nm = nn_out_info[i].name ? nn_out_info[i].name : "(null)";
      unsigned long blen = (unsigned long)LL_Buffer_len(&nn_out_info[i]);
      printf("[NN] out[%d] %-24s len=%lu bytes (%lu floats)\n",
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

  /* FaceRec I/O discovery (once) */
  const LL_Buffer_InfoTypeDef *fr_in_info  = LL_ATON_Input_Buffers_Info_face_recognition();
  const LL_Buffer_InfoTypeDef *fr_out_info = LL_ATON_Output_Buffers_Info_face_recognition();
  float   *fr_in      = (float *)LL_Buffer_addr_start(&fr_in_info[0]);
  void    *fr_out     =          LL_Buffer_addr_start(&fr_out_info[0]);
  uint32_t fr_in_len  = LL_Buffer_len(&fr_in_info[0]);
  uint32_t fr_out_len = LL_Buffer_len(&fr_out_info[0]);
  const int need_in_bytes = FR_IN_W * FR_IN_H * 3 * (int)sizeof(float);

  if (!fr_in || !fr_out || fr_in_len < (uint32_t)need_in_bytes || fr_out_len < sizeof(float)) {
    printf("[FR][ERR] IO invalid: in=%p (%lu) out=%p (%lu)\n",
           (void*)fr_in, (unsigned long)fr_in_len, fr_out, (unsigned long)fr_out_len);
  }

  /* Detector float input (NHWC [0..1]) for cropping */
  const LL_Buffer_InfoTypeDef *det_in_info = LL_ATON_Input_Buffers_Info_Default();
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

  /* knobs */
  const float     ACCEPT_CONF_MIN    = 0.70f;
  const float     ACCEPT_SIDE_MIN    = 0.28f;
  const float     CENTER_TOL         = 0.60f;
  const float     SCORE_SIDE_REF     = 0.35f;
  const float     STABLE_IOU_MIN     = 0.30f;
  const int       HOLD_MISS_FRAMES   = 6;
  const float     EMA_ALPHA          = 0.70f;

  /* FR run throttling & decision threshold */
  uint32_t last_fr_ms = 0;
  const uint32_t FR_PERIOD_MS = 250;      /* ~4 Hz */
  const float    MATCH_THR    = 0.55f;    /* cosine threshold for "known" */

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
        printf("[PP] best_det idx=%d conf=%.2f side=%.3f r_center=%.3f\n", best_i, best.conf, side, r_center);
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
        float ax0 = primary.roi.x_center - primary.roi.width*0.5f;
        float ay0 = primary.roi.y_center - primary.roi.height*0.5f;
        float ax1 = primary.roi.x_center + primary.roi.width*0.5f;
        float ay1 = primary.roi.y_center + primary.roi.height*0.5f;
        float bx0 = best.x_center - best.width*0.5f;
        float by0 = best.y_center - best.height*0.5f;
        float bx1 = best.x_center + best.width*0.5f;
        float by1 = best.y_center + best.height*0.5f;
        float ix0 = fmaxf(ax0, bx0), iy0 = fmaxf(ay0, by0);
        float ix1 = fminf(ax1, bx1), iy1 = fminf(ay1, by1);
        float iw = fmaxf(0.f, ix1 - ix0), ih = fmaxf(0.f, iy1 - iy0);
        float inter = iw * ih;
        float aarea = fmaxf(0.f, ax1-ax0) * fmaxf(0.f, ay1-ay0);
        float barea = fmaxf(0.f, bx1-bx0) * fmaxf(0.f, by1-by0);
        float uni = aarea + barea - inter + 1e-6f;
        float ov = inter / uni;

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

      /* Prepare FR input from detector float frame */
      crop_to_facerec_input_from_detector_input(
        det_in, NN_WIDTH, NN_HEIGHT,
        &primary.roi,
        fr_in, FR_IN_W, FR_IN_H);

      /* Cache: CPU -> NPU */
      DCACHE_Clean(fr_in, need_in_bytes);

      /* Run FaceRec */
      NPU_Lock(TAG_FR);
      uint32_t t0 = HAL_GetTick();
      FaceRec_Run_NoLock();
      uint32_t dt = HAL_GetTick() - t0;
      NPU_Unlock(TAG_FR);

      /* Cache: NPU -> CPU */
      DCACHE_Invalidate(fr_out, fr_out_len);

      /* Compare to reference set */
      const float *probe = (const float*)fr_out;
      int probe_dim = (int)(fr_out_len / sizeof(float));
      if (probe_dim > FR_EMB_SIZE) probe_dim = FR_EMB_SIZE;

      int best_i = -1;
      float best_s = -2.0f;

      for (int i = 0; i < g_ref_set_count; ++i) {
        int dim = (probe_dim < g_ref_set[i].dim) ? probe_dim : g_ref_set[i].dim;
        if (dim <= 0) continue;
        float s = cosine_sim(probe, g_ref_set[i].data, dim);
        if (s > best_s) { best_s = s; best_i = i; }
      }

      const char *name = "Unknown";
      if (best_i >= 0 && g_ref_set[best_i].name && g_ref_set[best_i].name[0] && best_s >= MATCH_THR) {
        name = g_ref_set[best_i].name;
      }

      /* Update overlay text */
      snprintf((char*)g_fr_overlay_label, sizeof(g_fr_overlay_label), "%s  %.2f", name, best_s);
      printf("[FR] match: %s  cos=%.3f  (dt=%lums)\n", name, best_s, (unsigned long)dt);
    }

    /* Publish ONLY detection (overlay text handled by Display via g_fr_overlay_label) */
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
