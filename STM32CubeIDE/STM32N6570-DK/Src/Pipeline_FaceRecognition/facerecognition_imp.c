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

    /* Publish ONLY detection (no FaceRec) */
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

