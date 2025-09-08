// facedetection_imp.c
#include <math.h>
#include <string.h>
#include <assert.h>

#include "facedetection_imp.h"
#include "app_shared.h"

#include "npu_guard.h"
#include "fr_helpers.h"
#include "ll_aton_runtime.h"
#include "network.h"
#include "app_config.h"
#include "app_cam.h"
#include "cmw_camera.h"
#include "facedetection_imp_bridge.h"
// (only if this file uses DCACHE_*; if not, you can skip)
#include "cache_utils.h"

#ifndef CMW_MODE_CONTINUOUS
#define CMW_MODE_CONTINUOUS DCMIPP_MODE_CONTINUOUS
#endif

/* ============================== */
/* nn_thread_fct — INTERNAL-IO    */
/* - camera RGB/BGR888 -> f32     */
/* - input stats (throttled)      */
/* - proper cache maintenance     */
/* ============================== */
void nn_thread_fct(void *arg)
{
  /* toggles */
  #define NN_LOG_INPUT_STATS       1
  #define NN_ONE_SHOT_SYNTH_TEST   0

  /* input/layout */
  #define INPUT_IS_BGR 0
  #define PACK_AS_NCHW 0

  /* Get model input buffer (internal, not user-allocated) */
  const LL_Buffer_InfoTypeDef *nn_in_info  = Detector_In_Info();

  float    *aton_in     = (float *)LL_Buffer_addr_start(&nn_in_info[0]);   /* FP32 destination */
  uint32_t  aton_in_len = LL_Buffer_len(&nn_in_info[0]);                   /* bytes, expect 196,608 */

  const int PIX = (NN_WIDTH * NN_HEIGHT);
  const int CH  = 3;
  assert(aton_in != NULL);
  assert(aton_in_len == (uint32_t)(PIX * CH * sizeof(float)));
  assert(NN_BPP == 3); /* RGB888/BGR888 */

  /* Start camera capture into our RGB888 byte buffers */
  uint8_t *nn_pipe_dst = bqueue_get_free(&nn_input_queue, 0);
  assert(nn_pipe_dst);
  CAM_NNPipe_Start(nn_pipe_dst, CMW_MODE_CONTINUOUS);

  uint32_t nn_period[2]; nn_period[1] = HAL_GetTick();
  uint32_t last_inp_print = 0;

  /* Preprocess: map [0..255] -> [0..1] (BlazeFace/YOLO* expect [0..1]) */
  const float scale = 1.0f / 255.0f;
  const float bias  = 0.0f;

  while (1)
  {
    /* Wait a captured frame (RGB888 bytes: NN_WIDTH*NN_HEIGHT*3) */
    uint8_t *capture_buffer = bqueue_get_ready(&nn_input_queue);
    assert(capture_buffer);

    /* IMPORTANT: DMA wrote the frame -> invalidate before CPU reads */
    DCACHE_Invalidate(capture_buffer, NN_WIDTH * NN_HEIGHT * NN_BPP);

    /* Reserve an output token to sync with pp thread */
    (void)bqueue_get_free(&nn_output_queue, 1);

    /* compute NN period */
    nn_period[0] = nn_period[1];
    nn_period[1] = HAL_GetTick();
    uint32_t nn_period_ms = nn_period[1] - nn_period[0];

    /* -------- RGB/BGR888 -> float32 -------- */
    {
      const uint8_t *src = capture_buffer;

    #if NN_LOG_INPUT_STATS
      float mn =  1e30f, mx = -1e30f, sum = 0.f; int cnt = 0;
    #endif

    #if PACK_AS_NCHW
      float *dstR = aton_in + 0 * PIX;
      float *dstG = aton_in + 1 * PIX;
      float *dstB = aton_in + 2 * PIX;
      for (int i = 0; i < PIX; ++i)
      {
        float r = (float)(*src++), g = (float)(*src++), b = (float)(*src++);
    #if INPUT_IS_BGR
        float t = r; r = b; b = t;
    #endif
        r = r * scale + bias; g = g * scale + bias; b = b * scale + bias;
        dstR[i] = r; dstG[i] = g; dstB[i] = b;

    #if NN_LOG_INPUT_STATS
        if ((HAL_GetTick() - last_inp_print) >= 1000U && ((i & 0x3F) == 0)) {
          mn = fminf(mn, r); mx = fmaxf(mx, r); sum += r; cnt++;
          mn = fminf(mn, g); mx = fmaxf(mx, g); sum += g; cnt++;
          mn = fminf(mn, b); mx = fmaxf(mx, b); sum += b; cnt++;
        }
    #endif
      }
    #else
      float *dst = aton_in;
      for (int i = 0; i < PIX; ++i)
      {
        float r = (float)(*src++), g = (float)(*src++), b = (float)(*src++);
    #if INPUT_IS_BGR
        float t = r; r = b; b = t;
    #endif
        r = r * scale + bias; g = g * scale + bias; b = b * scale + bias;
        *dst++ = r; *dst++ = g; *dst++ = b;

    #if NN_LOG_INPUT_STATS
        if ((HAL_GetTick() - last_inp_print) >= 1000U && ((i & 0x3F) == 0)) {
          mn = fminf(mn, r); mx = fmaxf(mx, r); sum += r; cnt++;
          mn = fminf(mn, g); mx = fmaxf(mx, g); sum += g; cnt++;
          mn = fminf(mn, b); mx = fmaxf(mx, b); sum += b; cnt++;
        }
    #endif
      }
    #endif /* PACK_AS_NCHW */

      /* cache clean: CPU wrote input, NPU will read it */
      DCACHE_Clean(aton_in, aton_in_len);

      /* publish detector input snapshot to FR module (float NHWC [0..1]) */
      fr_update_frame_snapshot(aton_in, aton_in_len);

    #if NN_LOG_INPUT_STATS
      if ((HAL_GetTick() - last_inp_print) >= 1000U) {
        float mean = (cnt > 0) ? (sum / (float)cnt) : 0.f;
        printf("[NN] input stats: min=%.3f max=%.3f mean=%.3f  [%s, %s]\r\n",
               mn, mx, mean, INPUT_IS_BGR ? "BGR" : "RGB", PACK_AS_NCHW ? "NCHW" : "NHWC");
        last_inp_print = HAL_GetTick();
      }
    #endif
    }

    /* -------- prepare outputs for NPU write -------- */
    {
      const LL_Buffer_InfoTypeDef *nn_out_info = Detector_Out_Info();
      for (int i = 0; i < NN_OUT_NB; ++i) {
        void *oaddr = LL_Buffer_addr_start(&nn_out_info[i]);
        size_t olen = (size_t)LL_Buffer_len(&nn_out_info[i]);
    #if defined(USE_DCACHE)
        dcache_align_range(&oaddr, &olen);
        SCB_CleanInvalidateDCache_by_Addr(oaddr, (int)olen);
    #else
        (void)oaddr; (void)olen;
    #endif
      }
    }

    /* inference (serialize on NPU) */
    uint32_t ts = HAL_GetTick();
    Detector_Run();                  // Detector_Run() handles NPU lock/unlock
    uint32_t inf_ms = HAL_GetTick() - ts;
    printf("[TIM] NN  infer took %lums\r\n", (unsigned long)inf_ms);

    /* Return camera buffer and wake postprocess */
    bqueue_put_free(&nn_input_queue);
    bqueue_put_ready(&nn_output_queue);

    /* publish stats to display (pp thread will fill boxes & signal update) */
    int ret = xSemaphoreTake(disp.lock, portMAX_DELAY);  assert(ret == pdTRUE);
    disp.info.inf_ms = inf_ms;
    disp.info.nn_period_ms = nn_period_ms;
    ret = xSemaphoreGive(disp.lock);                     assert(ret == pdTRUE);
  }
}
