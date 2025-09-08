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
#elif POSTPROCESS_TYPE == POSTPROCESS_OD_ST_YOLOX_UF
  st_yolox_pp_static_param_t pp_params;
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

  /* ---------- primary face stabilizer ---------- */
  typedef struct {
    od_pp_outBuffer_t roi;
    int   have;
    int   visible_count;
    int   missing_count;
    uint32_t first_seen_ms;
    uint32_t last_seen_ms;
  } primary_t;

  static primary_t primary = {0};

  static uint32_t last_blob_print     = 0;
  static uint32_t last_countdown_log  = 0;
  static uint32_t last_fr_ms          = 0;
  static uint32_t last_reason_log     = 0;

  /* One-shot enrollment countdown latch */
  static int      enroll_countdown_done = 0;

  /* 3-of-5 voting window: 255=empty, 1=match, 0=non-match */
  static uint8_t  vote_win[5] = {255,255,255,255,255};
  static int      vote_idx = 0, vote_filled = 0;

  /* knobs */
  const float     SCORE_SIDE_REF     = 0.35f;   /* prefer larger faces */
  const float     CENTER_TOL         = 0.60f;   /* radial tolerance when picking best */
  const float     ACCEPT_CONF_MIN    = 0.70f;   /* stricter gating */
  const float     ACCEPT_SIDE_MIN    = 0.28f;   /* ignore tiny faces */
  const float     STABLE_IOU_MIN     = 0.30f;   /* keep same primary when IoU ≥ this */
  const int       HOLD_MISS_FRAMES   = 6;       /* keep box for a few missed frames */
  const float     EMA_ALPHA          = 0.70f;   /* smoothing of cx,cy,w,h */
  const uint32_t  ENROLL_DWELL_MS    = 10000;   /* 10 s */
  const uint32_t  FR_THROTTLE_MS     = 250;     /* run FR at most every 250ms */
  const float     VOTE_MARGIN        = 0.05f;   /* deadband margin around thr_eff */
  const float     FAST_ACCEPT_BOOST  = 0.07f;   /* 2x strong matches -> instant accept */

  /* Enrollment anchor */
  static od_pp_outBuffer_t enroll_anchor;
  static int anchor_set = 0;

  while (1)
  {
    (void)bqueue_get_ready(&nn_output_queue);

    /* Map & invalidate NPU outputs for CPU read */
    for (int i = 0; i < NN_OUT_NB; i++) {
      pp_input[i] = LL_Buffer_addr_start(&nn_out_info[i]);
      pp_len[i]   = LL_Buffer_len(&nn_out_info[i]);
      DCACHE_Invalidate(pp_input[i], pp_len[i]);
    }

    /* Throttled raw-blob stats (optional) */
    {
      uint32_t now_stats = HAL_GetTick();
      if ((now_stats - last_blob_print) >= 600U) {
        for (int j = 0; j < NN_OUT_NB; ++j) {
          int nf = (int)(pp_len[j] / (uint32_t)sizeof(float));
          const float *p = (const float*)pp_input[j];
          float maxv = -1e30f, minv = 1e30f, sum = 0.f; int cnt = 0, nz = 0, nan_cnt = 0;
          for (int k = 0; k < nf; k++) {
            float v = p[k];
            if (!(v == v)) { nan_cnt++; continue; }
            if (v > maxv) maxv = v;
            if (v < minv) minv = v;
            sum += v; cnt++;
            if (v != 0.f) nz++;
          }
          if (nan_cnt == 0 && cnt > 0) {
            float mean = sum / (float)cnt;
            printf("[NN] out%d nf=%d  min=% .5f  max=% .5f  mean=% .5f  nz=%d\n",
                   j, nf, minv, maxv, mean, nz);
          }
        }
        last_blob_print = now_stats;
      }
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
        primary.first_seen_ms = now;
        primary.last_seen_ms  = now;
      } else {
        /* IoU check vs previous primary */
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
          /* Smooth towards the new box */
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
            primary.first_seen_ms = now;
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
          g_fr_overlay_label[0] = '\0';
        }
      }
    }

    /* -------- ENROLL dwell (10s) + FR throttle -------- */
    int enrolled   = fr_is_enrolled();
    int can_run_fr = 0;

    /* Crowd guard: never enroll or recognize when multiple faces visible */
    int crowd = (pp_output.nb_detect > 1);

    if (!enroll_countdown_done) {
      if (!enrolled && primary.have) {
        if (!crowd) {
          uint32_t dwell = now - primary.first_seen_ms;

          /* 1 Hz countdown during dwell */
          if (now - last_countdown_log >= 1000U) {
            int remain_s = (int)((ENROLL_DWELL_MS > dwell) ? ((ENROLL_DWELL_MS - dwell + 999)/1000) : 0);
            printf("[ENROLL] Keep your face steady… %ds\n", remain_s);
            last_countdown_log = now;
          }

          int remain_s = (int)((ENROLL_DWELL_MS > dwell) ? ((ENROLL_DWELL_MS - dwell + 999)/1000) : 0);
          snprintf((char*)g_fr_overlay_label, sizeof(g_fr_overlay_label),
                   (remain_s > 0) ? "%s (%ds)" : "%s",
                   FR_SUBJECT_NAME, remain_s);

          if (dwell >= ENROLL_DWELL_MS) {
            enroll_countdown_done = 1;
            enroll_anchor = primary.roi;
            anchor_set = 1;

            printf("[ENROLL] dwell complete — OPEN gate & anchor set\n");
            fr_set_enroll_gate(1);          /* start collecting samples now */
            can_run_fr = 1;                 /* run FR immediately */
          } else {
            fr_set_enroll_gate(0);
          }
        } else {
          fr_set_enroll_gate(0);
          primary.first_seen_ms = now;      /* reset dwell when crowd appears */
          if ((now - last_reason_log) >= 1000U) {
            printf("[FR][SKIP] crowd detected during enrollment dwell\n");
            last_reason_log = now;
          }
        }
      } else if (enrolled && primary.have && !crowd) {
        enroll_countdown_done = 1;
        fr_set_enroll_gate(0);
        if (now - last_fr_ms >= FR_THROTTLE_MS) can_run_fr = 1;
      } else {
        fr_set_enroll_gate(0);
      }
    } else {
      /* Countdown DONE; keep enrolling only if same anchored face is present and alone */
      if (!enrolled) {
        if (primary.have && anchor_set &&
            iou_norm_boxes(&primary.roi, &enroll_anchor) >= 0.60f &&
            !crowd) {
          fr_set_enroll_gate(1);
        } else {
          fr_set_enroll_gate(0);
          if ((now - last_reason_log) >= 1000U && !primary.have) {
            printf("[FR][SKIP] enroll phase: primary lost or anchor mismatch (IoU<0.60) or crowd\n");
            last_reason_log = now;
          }
        }
      } else {
        fr_set_enroll_gate(0);
      }

      if (primary.have && !crowd) {
        if (now - last_fr_ms >= FR_THROTTLE_MS) {
          can_run_fr = 1;
        } else if ((now - last_reason_log) >= 1000U) {
          printf("[FR][SKIP] throttled (%lums < %lums)\n",
                 (unsigned long)(now - last_fr_ms), (unsigned long)FR_THROTTLE_MS);
          last_reason_log = now;
        }
      } else if ((now - last_reason_log) >= 1000U) {
        if (crowd) printf("[FR][SKIP] crowd\n");
        else       printf("[FR][SKIP] no stable primary\n");
        last_reason_log = now;
      }
    }

    /* -------- Run FaceRec once on the stabilized primary (if allowed) -------- */
    if (primary.have && can_run_fr) {
      /* FR IO */
      const LL_Buffer_InfoTypeDef *fr_in_info  = LL_ATON_Input_Buffers_Info_face_recognition();
      float    *fr_in     = (float*)   LL_Buffer_addr_start(&fr_in_info[0]);
      uint32_t  fr_in_len =            LL_Buffer_len(&fr_in_info[0]);

      const LL_Buffer_InfoTypeDef *fr_out_info = LL_ATON_Output_Buffers_Info_face_recognition();
      void     *fr_out_ptr = LL_Buffer_addr_start(&fr_out_info[0]);
      uint32_t  fr_out_len = LL_Buffer_len(&fr_out_info[0]);

      fr_check_alias(fr_in, fr_out_ptr);

      /* ROI quick log (normalized + px) */
      {
        float sideN = fmaxf(primary.roi.width, primary.roi.height);
        float dx = fabsf(primary.roi.x_center - 0.5f);
        float dy = fabsf(primary.roi.y_center - 0.5f);
        float r_center = sqrtf(dx*dx + dy*dy);
        float side_px = sideN * (float)NN_WIDTH;
        printf("[FR-IN][ROI] det#0 conf=%.2f  cx=%.3f cy=%.3f  w=%.3f h=%.3f  sideN=%.3f  r_center=%.3f  (px side=%.1f)\n",
               primary.roi.conf, primary.roi.x_center, primary.roi.y_center,
               primary.roi.width, primary.roi.height, sideN, r_center, side_px);
      }

      /* Build input from the last NN frame snapshot */
      fr_prepare_input_for_det(&primary.roi, fr_in, FR_IN_W, FR_IN_H, NN_WIDTH, NN_HEIGHT, 0);

      /* Cache: clean input (CPU->NPU) and clean+invalidate output BEFORE run */
      DCACHE_Clean(fr_in, fr_in_len);
#if defined(USE_DCACHE)
      {
        void *oaddr = LL_Buffer_addr_start(&fr_out_info[0]);
        size_t olen = LL_Buffer_len(&fr_out_info[0]);
        dcache_align_range(&oaddr, &olen);
        SCB_CleanInvalidateDCache_by_Addr(oaddr, (int)olen);
      }
#endif

      /* Serialize FR on NPU */
      uint32_t t0 = HAL_GetTick();
      NPU_Lock(TAG_FR);
      FaceRec_Run_NoLock();
      NPU_Unlock(TAG_FR);
      uint32_t fr_ms = HAL_GetTick() - t0;

      /* Invalidate output AFTER NPU writes */
#if defined(USE_DCACHE)
      {
        void *oaddr = LL_Buffer_addr_start(&fr_out_info[0]);
        size_t olen = LL_Buffer_len(&fr_out_info[0]);
        dcache_align_range(&oaddr, &olen);
        SCB_InvalidateDCache_by_Addr(oaddr, (int)olen);
      }
#else
      DCACHE_Invalidate(fr_out_ptr, fr_out_len);
#endif

      /* Determine usable length defensively */
      int n_floats = (int)(fr_out_len / (uint32_t)sizeof(float));
      if (n_floats < 16 || n_floats > (FR_EMB_SIZE*4)) {
        n_floats = FR_EMB_SIZE;
        printf("[FR][WARN] suspicious out_len=%lu bytes — using n=%d\n",
               (unsigned long)fr_out_len, n_floats);
      }
      if (n_floats > FR_EMB_SIZE) n_floats = FR_EMB_SIZE;

      /* Copy to shadow buffer so later DMAs can’t clobber while we read */
      static float fr_out_shadow[FR_EMB_SIZE];
      memcpy(fr_out_shadow, fr_out_ptr, (size_t)n_floats * sizeof(float));

      /* Hand off to FR decision (with corruption checks inside) */
      fr_after_inference_and_decide(fr_out_shadow, n_floats, 0);
      printf("[TIM] FR infer took %lums\n", (unsigned long)fr_ms);

      /* --- Voting with fast-accept path --- */
      {
        static float last_sim = -1.f;

        float sim = -1.f;
        (void)fr_get_last_match(&sim);
        float thr_eff = fr_get_effective_threshold();

        /* If we have two consecutive strong matches, accept immediately */
        if (sim >= thr_eff + FAST_ACCEPT_BOOST && last_sim >= thr_eff + FAST_ACCEPT_BOOST) {
          snprintf((char*)g_fr_overlay_label, sizeof(g_fr_overlay_label), "%s", FR_SUBJECT_NAME);
          printf("[FR][VOTE] fast-accept (sim=%.3f, prev=%.3f, thr=%.3f)\n", sim, last_sim, thr_eff);
          /* reset window */
          for (int i = 0; i < 5; i++) { vote_win[i] = 255; }
          vote_idx = 0;
          vote_filled = 0;
        } else {
          /* Deadband voting around threshold */
          int vote;
          if      (sim >= (thr_eff + VOTE_MARGIN)) vote = 1;
          else if (sim <= (thr_eff - VOTE_MARGIN)) vote = 0;
          else {
            int prev = (vote_filled > 0) ? vote_win[(vote_idx+4)%5] : 255;
            vote = (prev == 0 || prev == 1) ? prev : 255;
          }

          vote_win[vote_idx] = (uint8_t)vote;
          vote_idx = (vote_idx+1)%5; if (vote_filled<5) vote_filled++;

          int ones=0, zeros=0;
          for (int i=0;i<vote_filled;i++){ if (vote_win[i]==1) ones++; else if (vote_win[i]==0) zeros++; }

          if (vote_filled >= 3) {
            if (ones >= 3) {
              snprintf((char*)g_fr_overlay_label, sizeof(g_fr_overlay_label), "%s", FR_SUBJECT_NAME);
              printf("[FR][VOTE] result=MATCH  (ones=%d zeros=%d)\n", ones, zeros);
            } else if (zeros >= 3) {
              snprintf((char*)g_fr_overlay_label, sizeof(g_fr_overlay_label), "Unknown");
              printf("[FR][VOTE] result=UNKNOWN  (ones=%d zeros=%d)\n", ones, zeros);
            } else {
              printf("[FR][VOTE] hold  (ones=%d zeros=%d)\n", ones, zeros);
            }
          } else {
            printf("[FR][VOTE] warming-up %d/5 (vote=%d sim=%.3f thr=%.3f)\n",
                   vote_filled, vote, sim, thr_eff);
          }
        }
        last_sim = sim;
      }

      last_fr_ms = now;
    } else {
      /* If we skipped FR due to crowd, avoid misleading label */
      if (pp_output.nb_detect > 1) {
        if (enroll_countdown_done || fr_is_enrolled()) {
          snprintf((char*)g_fr_overlay_label, sizeof(g_fr_overlay_label), "Unknown");
        }
      }
    }

    /* -------- Publish exactly ONE box to display (or none) -------- */
    int lock = xSemaphoreTake(disp.lock, portMAX_DELAY);  assert(lock == pdTRUE);

    if (primary.have) {
      disp.info.nb_detect = 1;
      disp.info.detects[0] = primary.roi;
    } else {
      disp.info.nb_detect = 0;
    }

    /* Force tracker overlay OFF so only one box is drawn */
    disp.info.tracking_enabled = 0;
#ifdef TRACKER_MODULE
    disp.info.tboxes_valid_nb = 0;
#endif
    disp.info.pp_ms = nn_pp[1] - nn_pp[0];

    lock = xSemaphoreGive(disp.lock);                     assert(lock == pdTRUE);

    bqueue_put_free(&nn_output_queue);
    xSemaphoreGive(disp.update);
  }
}
