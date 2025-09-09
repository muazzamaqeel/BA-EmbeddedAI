#define MY_NAME "Muazzam"
#define FR_SUBJECT_NAME MY_NAME   // <— add this so logs say “Muazzam”, not “ME”
#include "npu_guard.h"
#include "fr_helpers.h"
#include "app_shared.h"
#include "facedetection_imp.h"
#include "cache_utils.h"
#include "facedetection_imp_bridge.h"
#include "facerecognition_imp.h"

/* FaceRec dedicated user IO (non-aliased, PSRAM) */


/**
 ******************************************************************************
 * @file    app.c
 * @author  GPM Application Team
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "app.h"
#include "network.h"   // <-- REQUIRED for the macros & default buffers layout
#include "stm32n6570_discovery_xspi.h"
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "app_cam.h"
#include "app_config.h"
#include "app_postprocess.h"
#include "isp_api.h"
#include "ll_aton_runtime.h"
#include "ll_aton_rt_user_api.h"

#include "cmw_camera.h"
#include "scrl.h"
#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6xx_hal.h"
#ifdef STM32N6570_DK_REV
#include "stm32n6570_discovery.h"
#else
#include "stm32n6xx_nucleo.h"
#endif
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#ifdef TRACKER_MODULE
#include "tracker.h"
#endif
#include "utils.h"
#include "face_recognition.h"
/* FaceRec dedicated thread */
static StaticTask_t fr_thread;
static StackType_t fr_thread_stack[2 * configMINIMAL_STACK_SIZE];
TaskHandle_t g_fr_task = NULL;

/* was: static float  g_fr_in_user [FR_IN_W * FR_IN_H * 3] ALIGN_32; */
static uint8_t g_fr_in_user [FR_IN_W * FR_IN_H * 3] ALIGN_32;

/* was: static float  g_fr_out_user[FR_EMB_SIZE]           ALIGN_32; */
static int8_t  g_fr_out_user[FR_EMB_SIZE]           ALIGN_32;

// Wrappers so other files don't need to see NN_Instance_Default
LL_ATON_DECLARE_NAMED_NN_INSTANCE_AND_INTERFACE(Default);
// Declaration only
LL_ATON_DECLARE_NAMED_NN_INSTANCE_AND_INTERFACE(face_recognition);

void FaceRec_Run_NoLock(void) {
    LL_ATON_RT_Main(&NN_Instance_face_recognition);
}


// ---- NOW add the wrappers ----
const LL_Buffer_InfoTypeDef *Detector_In_Info(void)  { return LL_ATON_Input_Buffers_Info_Default(); }
const LL_Buffer_InfoTypeDef *Detector_Out_Info(void) { return LL_ATON_Output_Buffers_Info_Default(); }
void Detector_Run(void) {
    NPU_Lock(TAG_NN);
    LL_ATON_RT_Main(&NN_Instance_Default);
    NPU_Unlock(TAG_NN);
}


#define FREERTOS_PRIORITY(p) ((UBaseType_t)((int)tskIDLE_PRIORITY + configMAX_PRIORITIES / 2 + (p)))

#define CACHE_OP(__op__) do { \
  if (is_cache_enable()) { \
    __op__; \
  } \
} while (0)

#define ALIGN_VALUE(_v_,_a_) (((_v_) + (_a_) - 1) & ~((_a_) - 1))

#define NN_OUT_MAX_NB 4
#if NN_OUT_NB > NN_OUT_MAX_NB
#error "max output buffer reached"
#endif

/* define default 0 value for NN_OUTx_SIZE for [1:NN_OUT_MAX_NB[ */
#ifndef NN_OUT1_SIZE
#define NN_OUT1_SIZE 0
#endif
#ifndef NN_OUT2_SIZE
#define NN_OUT2_SIZE 0
#endif
#ifndef NN_OUT3_SIZE
#define NN_OUT3_SIZE 0
#endif
#define NN_OUT0_SIZE_ALIGN ALIGN_VALUE(NN_OUT0_SIZE, 32)
#define NN_OUT1_SIZE_ALIGN ALIGN_VALUE(NN_OUT1_SIZE, 32)
#define NN_OUT2_SIZE_ALIGN ALIGN_VALUE(NN_OUT2_SIZE, 32)
#define NN_OUT3_SIZE_ALIGN ALIGN_VALUE(NN_OUT3_SIZE, 32)
#define NN_OUT_BUFFER_SIZE (NN_OUT0_SIZE_ALIGN + NN_OUT1_SIZE_ALIGN + NN_OUT2_SIZE_ALIGN + NN_OUT3_SIZE_ALIGN)

#define LCD_FG_WIDTH LCD_BG_WIDTH
#define LCD_FG_HEIGHT LCD_BG_HEIGHT

#define NUMBER_COLORS 10
#define BQUEUE_MAX_BUFFERS 2
#define CPU_LOAD_HISTORY_DEPTH 8

#define DISPLAY_BUFFER_NB (DISPLAY_DELAY + 2)

/* Align so we are sure nn_output_buffers[0] and nn_output_buffers[1] are aligned on 32 bytes */
#define NN_BUFFER_OUT_SIZE_ALIGN ALIGN_VALUE(NN_BUFFER_OUT_SIZE, 32)

#define UTIL_LCD_COLOR_TRANSPARENT 0

#ifdef STM32N6570_DK_REV
#define LCD_FONT Font20
#define BUTTON_TOGGLE_TRACKING BUTTON_USER1
#else
#define LCD_FONT Font12
#define BUTTON_TOGGLE_TRACKING BUTTON_USER
#endif
//static float g_last_frame[NN_WIDTH * NN_HEIGHT * 3] ALIGN_32 IN_PSRAM;
/* Global handle so FaceRec can pause NN during extraction */
TaskHandle_t g_nn_task = NULL;

static void fr_thread_fct(void *arg)
{
  (void)arg;
  while (1) {
    // Wait until someone tells this thread to run
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    // [Lock NPU because detector may also run]
    NPU_Lock(TAG_FR);

    // Run Face Recognition on current face crop
    FaceRec_Run_NoLock();

    // Postprocess: embeddings are now in g_fr_out_user
    printf("[FR] Embeddings ready. First val=%.3f\n", g_fr_out_user[0]);

    NPU_Unlock(TAG_FR);
  }
}

// --- XSPI1 HyperRAM: enable memory-mapped window @ 0x9000_0000 .. 0x90FF_FFFF
/* app.c — disable duplicate HyperRAM mapping function */
#if 0
static int FR_HyperRAM_EnableMMAP(void)
{
  int32_t st = BSP_XSPI_RAM_Init(0);
  if (st != BSP_ERROR_NONE) { printf("XSPI1 HyperRAM init failed: %ld\r\n", (long)st); return -1; }
  st = BSP_XSPI_RAM_EnableMemoryMappedMode(0);
  if (st != BSP_ERROR_NONE) { printf("XSPI1 HyperRAM MMAP enable failed: %ld\r\n", (long)st); return -2; }
  printf("XSPI1 HyperRAM mapped @ 0x90000000..0x90FFFFFF\r\n");
  return 0;
}
#endif


volatile char g_fr_overlay_label[32] = "";  /* text shown in the box */



/* Globals */
DECLARE_CLASSES_TABLE;
/* Lcd Background area */
static Rectangle_TypeDef lcd_bg_area = {
  .X0 = 0,
  .Y0 = 0,
  .XSize = LCD_BG_WIDTH,
  .YSize = LCD_BG_HEIGHT,
};
/* Lcd Foreground area */
static Rectangle_TypeDef lcd_fg_area = {
  .X0 = 0,
  .Y0 = 0,
  .XSize = LCD_FG_WIDTH,
  .YSize = LCD_FG_HEIGHT,
};
static const uint32_t colors[NUMBER_COLORS] = {
    UTIL_LCD_COLOR_GREEN,
    UTIL_LCD_COLOR_RED,
    UTIL_LCD_COLOR_CYAN,
    UTIL_LCD_COLOR_MAGENTA,
    UTIL_LCD_COLOR_YELLOW,
    UTIL_LCD_COLOR_GRAY,
    UTIL_LCD_COLOR_BLACK,
    UTIL_LCD_COLOR_BROWN,
    UTIL_LCD_COLOR_BLUE,
    UTIL_LCD_COLOR_ORANGE
};
/* Lcd Background Buffer */
static uint8_t lcd_bg_buffer[DISPLAY_BUFFER_NB][LCD_BG_WIDTH * LCD_BG_HEIGHT * 2] ALIGN_32 IN_PSRAM;
static int lcd_bg_buffer_disp_idx = 1;
static int lcd_bg_buffer_capt_idx = 0;
/* Lcd Foreground Buffer */
static uint8_t lcd_fg_buffer[2][LCD_FG_WIDTH * LCD_FG_HEIGHT* 2] ALIGN_32 IN_PSRAM;
static int lcd_fg_buffer_rd_idx;
display_t disp;
static cpuload_info_t cpu_load;
/* screen buffer */
static uint8_t screen_buffer[LCD_BG_WIDTH * LCD_BG_HEIGHT * 2] ALIGN_32 IN_PSRAM;


/* nn input buffers */
/* Camera NN pipe delivers RGB888: 128*128*3 = 49,152 bytes */
static uint8_t nn_input_buffers[2][NN_WIDTH * NN_HEIGHT * NN_BPP] ALIGN_32 IN_PSRAM;


bqueue_t nn_input_queue;

 /* nn output buffers */
static const uint32_t nn_out_len_user[NN_OUT_MAX_NB] = {
  NN_OUT0_SIZE, NN_OUT1_SIZE, NN_OUT2_SIZE, NN_OUT3_SIZE
};
static uint8_t nn_output_buffers[2][NN_OUT_BUFFER_SIZE] ALIGN_32;
bqueue_t nn_output_queue;


 /* rtos */
static StaticTask_t nn_thread;
static StackType_t nn_thread_stack[2 * configMINIMAL_STACK_SIZE];
static StaticTask_t pp_thread;
static StackType_t pp_thread_stack[2 *configMINIMAL_STACK_SIZE];
static StaticTask_t dp_thread;
static StackType_t dp_thread_stack[2 *configMINIMAL_STACK_SIZE];
static StaticTask_t isp_thread;
static StackType_t isp_thread_stack[2 *configMINIMAL_STACK_SIZE];
static SemaphoreHandle_t isp_sem;
static StaticSemaphore_t isp_sem_buffer;

/* tracking state */
#ifdef TRACKER_MODULE
static trk_tbox_t tboxes[2 * AI_OD_PP_MAX_BOXES_LIMIT];
static trk_dbox_t dboxes[AI_OD_PP_MAX_BOXES_LIMIT];
static trk_ctx_t trk_ctx;
#endif



static int is_cache_enable()
{
#if defined(USE_DCACHE)
  return 1;
#else
  return 0;
#endif
}

static void cpuload_init(cpuload_info_t *cpu_load)
{
  memset(cpu_load, 0, sizeof(cpuload_info_t));
}

static void cpuload_update(cpuload_info_t *cpu_load)
{
  int i;

  cpu_load->history[1] = cpu_load->history[0];
  cpu_load->history[0].total = portGET_RUN_TIME_COUNTER_VALUE();
  cpu_load->history[0].thread = cpu_load->history[0].total - ulTaskGetIdleRunTimeCounter();
  cpu_load->history[0].tick = HAL_GetTick();

  if (cpu_load->history[1].tick - cpu_load->history[2].tick < 1000)
    return ;

  for (i = 0; i < CPU_LOAD_HISTORY_DEPTH - 2; i++)
    cpu_load->history[CPU_LOAD_HISTORY_DEPTH - 1 - i] = cpu_load->history[CPU_LOAD_HISTORY_DEPTH - 1 - i - 1];
}

static void cpuload_get_info(cpuload_info_t *cpu_load, float *cpu_load_last, float *cpu_load_last_second,
                             float *cpu_load_last_five_seconds)
{
  if (cpu_load_last)
    *cpu_load_last = 100.0 * (cpu_load->history[0].thread - cpu_load->history[1].thread) /
                     (cpu_load->history[0].total - cpu_load->history[1].total);
  if (cpu_load_last_second)
    *cpu_load_last_second = 100.0 * (cpu_load->history[2].thread - cpu_load->history[3].thread) /
                     (cpu_load->history[2].total - cpu_load->history[3].total);
  if (cpu_load_last_five_seconds)
    *cpu_load_last_five_seconds = 100.0 * (cpu_load->history[2].thread - cpu_load->history[7].thread) /
                     (cpu_load->history[2].total - cpu_load->history[7].total);
}

static int bqueue_init(bqueue_t *bq, int buffer_nb, uint8_t **buffers)
{
  int i;

  if (buffer_nb > BQUEUE_MAX_BUFFERS)
    return -1;

  bq->free = xSemaphoreCreateCountingStatic(buffer_nb, buffer_nb, &bq->free_buffer);
  if (!bq->free)
    goto free_sem_error;
  bq->ready = xSemaphoreCreateCountingStatic(buffer_nb, 0, &bq->ready_buffer);
  if (!bq->ready)
    goto ready_sem_error;

  bq->buffer_nb = buffer_nb;
  for (i = 0; i < buffer_nb; i++) {
    assert(buffers[i]);
    bq->buffers[i] = buffers[i];
  }
  bq->free_idx = 0;
  bq->ready_idx = 0;

  return 0;

ready_sem_error:
  vSemaphoreDelete(bq->free);
free_sem_error:
  return -1;
}

uint8_t *bqueue_get_free(bqueue_t *bq, int is_blocking)
{
  uint8_t *res;
  int ret;

  ret = xSemaphoreTake(bq->free, is_blocking ? portMAX_DELAY : 0);
  if (ret == pdFALSE)
    return NULL;

  res = bq->buffers[bq->free_idx];
  bq->free_idx = (bq->free_idx + 1) % bq->buffer_nb;

  return res;
}

void bqueue_put_free(bqueue_t *bq)
{
  int ret;

  ret = xSemaphoreGive(bq->free);
  assert(ret == pdTRUE);
}

uint8_t *bqueue_get_ready(bqueue_t *bq)
{
  uint8_t *res;
  int ret;

  ret = xSemaphoreTake(bq->ready, portMAX_DELAY);
  assert(ret == pdTRUE);

  res = bq->buffers[bq->ready_idx];
  bq->ready_idx = (bq->ready_idx + 1) % bq->buffer_nb;

  return res;
}

void bqueue_put_ready(bqueue_t *bq)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  int ret;

  if (xPortIsInsideInterrupt()) {
    ret = xSemaphoreGiveFromISR(bq->ready, &xHigherPriorityTaskWoken);
    assert(ret == pdTRUE);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  } else {
    ret = xSemaphoreGive(bq->ready);
    assert(ret == pdTRUE);
  }
}

static void reload_bg_layer(int next_disp_idx)
{
  int ret;

  ret = SCRL_SetAddress_NoReload(lcd_bg_buffer[next_disp_idx], SCRL_LAYER_0);
  assert(ret == 0);
  ret = SCRL_ReloadLayer(SCRL_LAYER_0);
  assert(ret == 0);

  ret = SRCL_Update();
  assert(ret == 0);
}

static void app_main_pipe_frame_event()
{
  //printf("P1 frame\n");
  int next_disp_idx = (lcd_bg_buffer_disp_idx + 1) % DISPLAY_BUFFER_NB;
  int next_capt_idx = (lcd_bg_buffer_capt_idx + 1) % DISPLAY_BUFFER_NB;
  int ret;

  ret = HAL_DCMIPP_PIPE_SetMemoryAddress(CMW_CAMERA_GetDCMIPPHandle(), DCMIPP_PIPE1,
                                         DCMIPP_MEMORY_ADDRESS_0, (uint32_t) lcd_bg_buffer[next_capt_idx]);
  assert(ret == HAL_OK);

  reload_bg_layer(next_disp_idx);
  lcd_bg_buffer_disp_idx = next_disp_idx;
  lcd_bg_buffer_capt_idx = next_capt_idx;
}

static void app_ancillary_pipe_frame_event()
{
  //printf("P2 frame\n");
  uint8_t *next_buffer;
  int ret;

  next_buffer = bqueue_get_free(&nn_input_queue, 0);
  if (next_buffer) {
    ret = HAL_DCMIPP_PIPE_SetMemoryAddress(CMW_CAMERA_GetDCMIPPHandle(), DCMIPP_PIPE2,
                                           DCMIPP_MEMORY_ADDRESS_0, (uint32_t) next_buffer);
    assert(ret == HAL_OK);
    bqueue_put_ready(&nn_input_queue);
  }
}

static void app_main_pipe_vsync_event()
{
  //printf("VSYNC\n");
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  int ret;

  ret = xSemaphoreGiveFromISR(isp_sem, &xHigherPriorityTaskWoken);
  if (ret == pdTRUE)
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static int clamp_point(int *x, int *y)
{
  int xi = *x;
  int yi = *y;

  if (*x < 0)
    *x = 0;
  if (*y < 0)
    *y = 0;
  if (*x >= lcd_bg_area.XSize)
    *x = lcd_bg_area.XSize - 1;
  if (*y >= lcd_bg_area.YSize)
    *y = lcd_bg_area.YSize - 1;

  return (xi != *x) || (yi != *y);
}

static void convert_length(float32_t wi, float32_t hi, int *wo, int *ho)
{
  *wo = (int) (lcd_bg_area.XSize * wi);
  *ho = (int) (lcd_bg_area.YSize * hi);
}

static void convert_point(float32_t xi, float32_t yi, int *xo, int *yo)
{
  *xo = (int) (lcd_bg_area.XSize * xi);
  *yo = (int) (lcd_bg_area.YSize * yi);
}

static void Display_Detection(od_pp_outBuffer_t *detect)
{
  int xc, yc;
  int x0, y0;
  int x1, y1;
  int w, h;

  convert_point(detect->x_center, detect->y_center, &xc, &yc);
  convert_length(detect->width, detect->height, &w, &h);
  x0 = xc - (w + 1) / 2;
  y0 = yc - (h + 1) / 2;
  x1 = xc + (w + 1) / 2;
  y1 = yc + (h + 1) / 2;
  clamp_point(&x0, &y0);
  clamp_point(&x1, &y1);

  UTIL_LCD_DrawRect(x0, y0, x1 - x0, y1 - y0, colors[detect->class_index % NUMBER_COLORS]);
  const char *lbl = (g_fr_overlay_label[0] ? (const char*)g_fr_overlay_label
                                           : classes_table[detect->class_index]);
  UTIL_LCDEx_PrintfAt(x0 + 1, y0 + 1, LEFT_MODE, lbl);
}

static void Display_NetworkOutput_NoTracking(display_info_t *info)
{
  od_pp_outBuffer_t *rois = info->detects;
  uint32_t nb_rois = info->nb_detect;
  float cpu_load_one_second;
  int line_nb = 0;
  float nn_fps;
  int i;

  /* clear previous ui */
  UTIL_LCD_FillRect(lcd_fg_area.X0, lcd_fg_area.Y0, lcd_fg_area.XSize, lcd_fg_area.YSize, 0x00000000); /* Clear previous boxes */

  /* cpu load */
  cpuload_update(&cpu_load);
  cpuload_get_info(&cpu_load, NULL, &cpu_load_one_second, NULL);

  /* draw metrics */
  nn_fps = 1000.0 / info->nn_period_ms;
#if 1
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb),  RIGHT_MODE, "Cpu load");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb),  RIGHT_MODE, "   %.1f%%", cpu_load_one_second);
  line_nb += 2;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "Inference");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "   %ums", info->inf_ms);
  line_nb += 2;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "   FPS");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "  %.2f", nn_fps);
  line_nb += 2;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, " Objects %u", nb_rois);
  line_nb += 1;
#else
  (void) nn_fps;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb),  RIGHT_MODE, "Cpu load");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb),  RIGHT_MODE, "   %.1f%%", cpu_load_one_second);
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "nn period");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "   %ums", info->nn_period_ms);
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "Inference");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "   %ums", info->inf_ms);
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "Post process");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "   %ums", info->pp_ms);
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "Display");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "   %ums", info->disp_ms);
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, " Objects %u", nb_rois);
  line_nb += 1;
#endif

  /* Draw bounding boxes */
  for (i = 0; i < nb_rois; i++)
    Display_Detection(&rois[i]);
}

static int model_get_output_nb(const LL_Buffer_InfoTypeDef *nn_out_info)
{
  int nb = 0;

  while (nn_out_info->name) {
    nb++;
    nn_out_info++;
  }

  return nb;
}

#ifdef TRACKER_MODULE
static void Display_TrackingBox(tbox_info *tbox)
{
  int xc, yc;
  int x0, y0;
  int x1, y1;
  int w, h;

  convert_point(tbox->cx, tbox->cy, &xc, &yc);
  convert_length(tbox->w, tbox->h, &w, &h);
  x0 = xc - (w + 1) / 2;
  y0 = yc - (h + 1) / 2;
  x1 = xc + (w + 1) / 2;
  y1 = yc + (h + 1) / 2;
  clamp_point(&x0, &y0);
  clamp_point(&x1, &y1);

  UTIL_LCD_DrawRect(x0, y0, x1 - x0, y1 - y0, colors[tbox->id % NUMBER_COLORS]);
  UTIL_LCDEx_PrintfAt(x0 + 1, y0 + 1, LEFT_MODE, "%3d", tbox->id);
}

static void Display_NetworkOutput_Tracking(display_info_t *info)
{
  float cpu_load_one_second;
  int line_nb = 0;
  float nn_fps;
  int i;

  /* clear previous ui */
  UTIL_LCD_FillRect(lcd_fg_area.X0, lcd_fg_area.Y0, lcd_fg_area.XSize, lcd_fg_area.YSize, 0x00000000); /* Clear previous boxes */

  /* cpu load */
  cpuload_update(&cpu_load);
  cpuload_get_info(&cpu_load, NULL, &cpu_load_one_second, NULL);

  /* draw metrics */
  nn_fps = 1000.0 / info->nn_period_ms;
#if 1
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb),  RIGHT_MODE, "Cpu load");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb),  RIGHT_MODE, "   %.1f%%", cpu_load_one_second);
  line_nb += 2;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "Inference");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "   %ums", info->inf_ms);
  line_nb += 2;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "   FPS");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "  %.2f", nn_fps);
  line_nb += 2;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, " Objects %u", info->tboxes_valid_nb);
  line_nb += 1;
#else
  (void) nn_fps;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb),  RIGHT_MODE, "Cpu load");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb),  RIGHT_MODE, "   %.1f%%", cpu_load_one_second);
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "nn period");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "   %ums", info->nn_period_ms);
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "Inference");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "   %ums", info->inf_ms);
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "Post process");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "   %ums", info->pp_ms);
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "Display");
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, "   %ums", info->disp_ms);
  line_nb += 1;
  UTIL_LCDEx_PrintfAt(0, LINE(line_nb), RIGHT_MODE, " Objects %u", info->tboxes_valid_nb);
  line_nb += 1;
#endif

  /* Draw bounding boxes */
  for (i = 0; i < info->tboxes_valid_nb; i++)
    Display_TrackingBox(&info->tboxes[i]);
}
#else
static void Display_NetworkOutput_Tracking(display_info_t *info)
{
  /* You should not be here */
  assert(0);
}
#endif

static void Display_NetworkOutput(display_info_t *info)
{
  if (info->tracking_enabled)
    Display_NetworkOutput_Tracking(info);
  else
    Display_NetworkOutput_NoTracking(info);
}






static const char* aton_io_errstr(int r){
  if (r == LL_ATON_User_IO_NOERROR)     return "NOERROR";
  if (r == LL_ATON_User_IO_WRONG_INDEX) return "WRONG_INDEX";
  if (r == LL_ATON_User_IO_WRONG_SIZE)  return "WRONG_SIZE";
  return "?";
}





/* ============================== */
/* nn_thread_fct — INTERNAL-IO    */
/* - camera RGB/BGR888 -> f32     */
/* - input stats (throttled)      */
/* - proper cache maintenance     */
/* ============================== */







#ifdef TRACKER_MODULE
static int TRK_Init()
{
  const trk_conf_t cfg = {
    .track_thresh = 0.25f,
    .det_thresh   = 0.60f,  /* was 0.80f; your det conf ~0.62–0.78 */
    .sim1_thresh  = 0.80f,
    .sim2_thresh  = 0.50f,
    .tlost_cnt    = 30,
  };
  return trk_init(&trk_ctx, (trk_conf_t *)&cfg, ARRAY_NB(tboxes), tboxes);
}
#endif

static int update_and_capture_tracking_enabled()
{
  static int prev_button_state = GPIO_PIN_RESET;
  static int tracking_enabled = 1;
  int cur_button_state;
  int ret;

  cur_button_state = BSP_PB_GetState(BUTTON_TOGGLE_TRACKING);
  if (cur_button_state == GPIO_PIN_SET && prev_button_state == GPIO_PIN_RESET) {
    tracking_enabled = !tracking_enabled;
    if (tracking_enabled) {
      printf("Enable tracking\n");
      ret = TRK_Init();
      assert(ret == 0);
    } else
      printf("Disable tracking\n");
  }
  prev_button_state = cur_button_state;

  return tracking_enabled;
}





static inline float clamp01f(float v){ return (v < 0.f) ? 0.f : (v > 1.f ? 1.f : v); }

#ifdef TRACKER_MODULE
static int roi_to_dbox_safe(const od_pp_outBuffer_t *roi, trk_dbox_t *dbox)
{
  if (!roi || !dbox) return 0;
  if (!isfinite(roi->x_center) || !isfinite(roi->y_center) ||
      !isfinite(roi->width)    || !isfinite(roi->height)   ||
      !isfinite(roi->conf)) return 0;

  /* center/size -> corners */
  float cx = clamp01f(roi->x_center);
  float cy = clamp01f(roi->y_center);
  float w  = fmaxf(roi->width ,  1e-4f);
  float h  = fmaxf(roi->height,  1e-4f);
  float x0 = cx - 0.5f*w, y0 = cy - 0.5f*h;
  float x1 = cx + 0.5f*w, y1 = cy + 0.5f*h;

  /* clip to [0,1] */
  x0 = fmaxf(0.f, x0); y0 = fmaxf(0.f, y0);
  x1 = fminf(1.f, x1); y1 = fminf(1.f, y1);

  /* re-derive center/size (guaranteed positive) */
  w  = fmaxf(1e-4f, x1 - x0);
  h  = fmaxf(1e-4f, y1 - y0);
  cx = 0.5f * (x0 + x1);
  cy = 0.5f * (y0 + y1);

  dbox->conf = clamp01f(roi->conf);
  dbox->cx   = cx;
  dbox->cy   = cy;
  dbox->w    = w;
  dbox->h    = h;
  return 1;
}
#endif








#ifdef TRACKER_MODULE
static int app_tracking(od_pp_out_t *pp)
{
  int tracking_enabled = update_and_capture_tracking_enabled();
  if (!tracking_enabled) return 0;

  /* Gate: accept only confident, reasonable faces */
  const float TRK_CONF_MIN   = 0.60f;   /* same as TRK_Init det_thresh */
  const float TRK_SIDE_MIN   = 0.20f;   /* min normalized side */
  const float TRK_CENTER_TOL = 0.60f;   /* ignore far-corner ghosts */

  int n_in  = (int)pp->nb_detect;
  if (n_in <= 0) return 1;

  /* Optional: feed only the best detection to the tracker */
  int best_idx = -1;
  float best_score = -1.f;
  for (int i = 0; i < n_in; ++i) {
    const od_pp_outBuffer_t *d = &pp->pOutBuff[i];
    float side = fmaxf(d->width, d->height);
    float dx = fabsf(d->x_center - 0.5f), dy = fabsf(d->y_center - 0.5f);
    float r_center = sqrtf(dx*dx + dy*dy);
    if (d->conf < TRK_CONF_MIN || side < TRK_SIDE_MIN || r_center > TRK_CENTER_TOL)
      continue;

    float s_size   = fminf(1.f, side / 0.35f);
    float s_center = 1.f - fminf(1.f, r_center / 0.6f);
    float score = d->conf * (0.25f + 0.75f * s_size) * s_center;
    if (score > best_score) { best_score = score; best_idx = i; }
  }

  int used = 0;
  if (best_idx >= 0) {
    if (roi_to_dbox_safe(&pp->pOutBuff[best_idx], &dboxes[0])) used = 1;
  }

  /* If nothing passed the gates, do not call the tracker this frame */
  if (used == 0) return 1;

  int ret = trk_update(&trk_ctx, used, dboxes);
  if (ret != 0) {
    printf("[TRK][ERR] update failed (%d) — reinit\n", ret);
    TRK_Init();
  }
  return 1;
}
#else
static int app_tracking(od_pp_out_t *pp) { (void)pp; return 0; }
#endif





// [ADD - your enrollment database]
// Fill these with your real 128-D embeddings (captured offline) and matching names.

static const float g_ref_emb[][FR_EMB_SIZE] = {
  /* Example: paste your real vectors here */
  // { /* 512 floats */ },
  // { /* 512 floats */ },
};
static const char *g_ref_name[] = {
  // "Alice",
  // "Bob",
};
static const int g_ref_count = (int)(sizeof(g_ref_name)/sizeof(g_ref_name[0]));

// [ADD - cosine similarity]
static float cosine_similarity(const float *a, const float *b, int n)
{
  float dot=0.f, na=0.f, nb=0.f;
  for (int i=0;i<n;i++) { float va=a[i], vb=b[i]; dot+=va*vb; na+=va*va; nb+=vb*vb; }
  float den = sqrtf(na)*sqrtf(nb) + 1e-6f;
  return dot / den;
}

// [ADD - clamp utility]
static inline int clampi(int v, int lo, int hi){ return (v<lo)?lo:((v>hi)?hi:v); }

// [ADD - crop+resize (bilinear) from detector input FP32 NHWC → FaceRec FP32 NHWC 112x112
// src: FP32 NHWC in [0..1], size NN_WIDTH x NN_HEIGHT x 3
// roi: od_pp_outBuffer_t (normalized cx,cy,w,h in [0..1])
// dst: FP32 NHWC, 112x112x3, normalized to [-1..1] as MobileFaceNet typically expects
static void crop_to_facerec_input_from_detector_input(
    const float *src_nhwc, int src_w, int src_h,
    const od_pp_outBuffer_t *roi,
    float *dst_nhwc, int dst_w, int dst_h)
{
  // Make square box with some margin
  float cx = roi->x_center * src_w;
  float cy = roi->y_center * src_h;
  float bw = roi->width  * src_w;
  float bh = roi->height * src_h;
  float side = fmaxf(bw, bh) * 1.10f;  // +10% margin
  float x0 = cx - side * 0.5f;
  float y0 = cy - side * 0.5f;
  float x1 = cx + side * 0.5f;
  float y1 = cy + side * 0.5f;

  // Bilinear sampling
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

      // sample 4 neighbors, NHWC
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
        // src is [0..1], convert to [-1..1] for most FR backbones:
        dst_nhwc[didx+c] = v*2.f - 1.f;
      }
    }
  }
}


#ifdef TRACKER_MODULE
static void tbox_to_tbox_info(const trk_tbox_t *tbox, tbox_info *tinfo)
{
  tinfo->cx = tbox->cx; tinfo->cy = tbox->cy;
  tinfo->w  = tbox->w;  tinfo->h  = tbox->h;
  tinfo->id = tbox->id;
}
#endif





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




static void dp_update_drawing_area()
{
  int ret;

  __disable_irq();
  ret = SCRL_SetAddress_NoReload(lcd_fg_buffer[lcd_fg_buffer_rd_idx], SCRL_LAYER_1);
  assert(ret == HAL_OK);
  __enable_irq();
}

static void dp_commit_drawing_area()
{
  int ret;

  __disable_irq();
  ret = SCRL_ReloadLayer(SCRL_LAYER_1);
  assert(ret == HAL_OK);
  __enable_irq();
  lcd_fg_buffer_rd_idx = 1 - lcd_fg_buffer_rd_idx;
}

static void dp_thread_fct(void *arg)
{
  uint32_t disp_ms = 0;
  display_info_t info;
  uint32_t ts;
  int ret;

  while (1)
  {
    ret = xSemaphoreTake(disp.update, portMAX_DELAY);
    assert(ret == pdTRUE);

    ret = xSemaphoreTake(disp.lock, portMAX_DELAY);
    assert(ret == pdTRUE);
    info = disp.info;
    ret = xSemaphoreGive(disp.lock);
    assert(ret == pdTRUE);
    info.disp_ms = disp_ms;

    ts = HAL_GetTick();
    dp_update_drawing_area();
    Display_NetworkOutput(&info);
    DCACHE_Clean(lcd_fg_buffer[lcd_fg_buffer_rd_idx], LCD_FG_WIDTH * LCD_FG_HEIGHT * 2);
    dp_commit_drawing_area();
    disp_ms = HAL_GetTick() - ts;
  }
}

static void isp_thread_fct(void *arg)
{
  int ret;

  while (1) {
    ret = xSemaphoreTake(isp_sem, portMAX_DELAY);
    assert(ret == pdTRUE);

    CAM_IspUpdate();
  }
}

static void Display_init()
{
  SCRL_LayerConfig layers_config[2] = {
    {
      .origin = {lcd_bg_area.X0, lcd_bg_area.Y0},
      .size = {lcd_bg_area.XSize, lcd_bg_area.YSize},
      .format = SCRL_RGB565,
      .address = lcd_bg_buffer[lcd_bg_buffer_disp_idx],
    },
    {
      .origin = {lcd_fg_area.X0, lcd_fg_area.Y0},
      .size = {lcd_fg_area.XSize, lcd_fg_area.YSize},
      .format = SCRL_ARGB4444,
      .address = lcd_fg_buffer[1],
    },
  };
  SCRL_ScreenConfig screen_config = {
    .size = {lcd_bg_area.XSize, lcd_bg_area.YSize},
#ifdef SCR_LIB_USE_SPI
    .format = SCRL_RGB565,
#else
    .format = SCRL_YUV422, /* Use SCRL_RGB565 if host support this format to reduce cpu load */
#endif
    .address = screen_buffer,
    .fps = CAMERA_FPS,
  };
  int ret;

  ret = SCRL_Init((SCRL_LayerConfig *[2]){&layers_config[0], &layers_config[1]}, &screen_config);
  assert(ret == 0);

  UTIL_LCD_SetLayer(SCRL_LAYER_1);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_TRANSPARENT);
  UTIL_LCD_SetFont(&LCD_FONT);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
}


static void xspi_enable_mmap(void)
{
    BSP_XSPI_NOR_Init_t cfg = {0};
#ifdef BSP_XSPI_NOR_OPI_MODE
    cfg.InterfaceMode = BSP_XSPI_NOR_OPI_MODE;
#endif
#ifdef BSP_XSPI_NOR_DTR_TRANSFER
    cfg.TransferRate  = BSP_XSPI_NOR_DTR_TRANSFER;
#endif
#ifdef BSP_XSPI_NOR_DUALFLASH_DISABLE
    cfg.DualFlash     = BSP_XSPI_NOR_DUALFLASH_DISABLE;
#endif

    int32_t st = BSP_XSPI_NOR_Init(1, &cfg);
    if (st != BSP_ERROR_NONE) { printf("XSPI1 init failed: %ld\r\n", (long)st); return; }
    st = BSP_XSPI_NOR_EnableMemoryMappedMode(1);
    if (st != BSP_ERROR_NONE) { printf("XSPI1 mmap failed: %ld\r\n", (long)st); return; }
}


static int xspi_enable_mmap_auto(void)
{
    BSP_XSPI_NOR_Init_t cfg = {0};
#ifdef BSP_XSPI_NOR_OPI_MODE
    cfg.InterfaceMode = BSP_XSPI_NOR_OPI_MODE;
#endif
#ifdef BSP_XSPI_NOR_DTR_TRANSFER
    cfg.TransferRate  = BSP_XSPI_NOR_DTR_TRANSFER;
#endif
#ifdef BSP_XSPI_NOR_DUALFLASH_DISABLE
    cfg.DualFlash     = BSP_XSPI_NOR_DUALFLASH_DISABLE;
#endif

    const int candidates[2] = {1, 0};  /* N6570-DK NOR is typically XSPI2 (index 1), fallback XSPI1 (0) */
    for (int t = 0; t < 2; ++t) {
        int inst = candidates[t];
        if (BSP_XSPI_NOR_Init(inst, &cfg) == BSP_ERROR_NONE) {
            if (BSP_XSPI_NOR_EnableMemoryMappedMode(inst) == BSP_ERROR_NONE) {
                printf("XSPI%d mapped OK (NOR @ 0x71000000)\r\n", inst + 1);
                return inst;
            } else {
                printf("XSPI%d mmap enable failed\r\n", inst + 1);
            }
        }
    }
    printf("XSPI mmap failed on both instances\r\n");
    return -1;
}


static void xspi_quick_check(void)
{
  volatile const uint32_t *pA = (const uint32_t*)0x71000000u;  // Detector base
  volatile const uint32_t *pB = (const uint32_t*)0x71028590u;  // some known word in detector
  volatile const uint32_t *pC = (const uint32_t*)0x71027FA0u;

  volatile const uint32_t *fr0 = (const uint32_t*)0x71040000u; // FaceID base (your script)
  volatile const uint32_t *fr1 = (const uint32_t*)0x71040100u;
  volatile const uint32_t *fr2 = (const uint32_t*)0x71041000u;

  printf("[XSPI] 71000000: %08lX %08lX %08lX %08lX\r\n", pA[0], pA[1], pA[2], pA[3]);
  printf("[XSPI] 71028590: %08lX %08lX %08lX %08lX\r\n", pB[0], pB[1], pB[2], pB[3]);
  printf("[XSPI] 71027FA0: %08lX %08lX %08lX %08lX\r\n", pC[0], pC[1], pC[2], pC[3]);

  printf("[XSPI] 71040000: %08lX %08lX %08lX %08lX\r\n", fr0[0], fr0[1], fr0[2], fr0[3]);
  printf("[XSPI] 71040100: %08lX %08lX %08lX %08lX\r\n", fr1[0], fr1[1], fr1[2], fr1[3]);
  printf("[XSPI] 71041000: %08lX %08lX %08lX %08lX\r\n", fr2[0], fr2[1], fr2[2], fr2[3]);
}







void app_run()
{
  /* NOTE: NOR (weights) is mapped in main_thread_fct(). Do not map again here. */

  /* RTOS priorities & handles */
  UBaseType_t isp_priority = FREERTOS_PRIORITY(2);
  UBaseType_t pp_priority  = FREERTOS_PRIORITY(-2);
  UBaseType_t dp_priority  = FREERTOS_PRIORITY(-2);
  UBaseType_t nn_priority  = FREERTOS_PRIORITY(1);
  TaskHandle_t hdl;
  int ret;

  printf("Init application\n");

  /* Enable DWT so DWT_CYCCNT works when debugger not attached */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  /* Screen init */
  memset(lcd_bg_buffer, 0, sizeof(lcd_bg_buffer));
  CACHE_OP(SCB_CleanInvalidateDCache_by_Addr(lcd_bg_buffer, sizeof(lcd_bg_buffer)));
  memset(lcd_fg_buffer, 0, sizeof(lcd_fg_buffer));
  CACHE_OP(SCB_CleanInvalidateDCache_by_Addr(lcd_fg_buffer, sizeof(lcd_fg_buffer)));
  Display_init();

  /* Queues */
  ret = bqueue_init(&nn_input_queue, 2, (uint8_t *[2]){nn_input_buffers[0], nn_input_buffers[1]});  assert(ret == 0);
  ret = bqueue_init(&nn_output_queue, 2, (uint8_t *[2]){nn_output_buffers[0], nn_output_buffers[1]});assert(ret == 0);

#ifdef TRACKER_MODULE
  /* Tracker init + button */
  ret = TRK_Init();                                   assert(ret == 0);
  ret = BSP_PB_Init(BUTTON_TOGGLE_TRACKING, BUTTON_MODE_GPIO);
  assert(ret == BSP_ERROR_NONE);
#endif

  /* CPU load stats */
  cpuload_init(&cpu_load);

  /* Camera */
  CAM_Init();

  /* Semaphores + mutex */
  isp_sem     = xSemaphoreCreateCountingStatic(1, 0, &isp_sem_buffer);     assert(isp_sem);
  disp.update = xSemaphoreCreateCountingStatic(1, 0, &disp.update_buffer); assert(disp.update);
  disp.lock   = xSemaphoreCreateMutexStatic(&disp.lock_buffer);             assert(disp.lock);

  /* Global NPU mutex/guard */
  npu_guard_init();

  /* ---- Bind FaceRec dedicated, non-aliased user IO buffers (in PSRAM) ---- */
  printf("[FR] user buffers: IN=%p (len=%lu)  OUT=%p (len=%lu)\r\n",
         (void*)g_fr_in_user,  (unsigned long)sizeof(g_fr_in_user),
         (void*)g_fr_out_user, (unsigned long)sizeof(g_fr_out_user));

  int r_in  = LL_ATON_Set_User_Input_Buffer_face_recognition (0, (void*)g_fr_in_user,  sizeof(g_fr_in_user));
  int r_out = LL_ATON_Set_User_Output_Buffer_face_recognition(0, (void*)g_fr_out_user, sizeof(g_fr_out_user));
  printf("[FR] bind IO: in=%s out=%s\r\n", aton_io_errstr(r_in), aton_io_errstr(r_out));

  bool fr_user_io_active = (r_in == LL_ATON_User_IO_NOERROR) && (r_out == LL_ATON_User_IO_NOERROR);
  if (!fr_user_io_active) {
    printf("[FR][WARN] user-IO binding unsupported on this build — "
           "falling back to internal buffers (shadow copy + cache ops remain).\r\n");
  }

  /* Report actual buffers the runtime uses (internal or user) */
  const LL_Buffer_InfoTypeDef *chk_in  = LL_ATON_Input_Buffers_Info_face_recognition();
  const LL_Buffer_InfoTypeDef *chk_out = LL_ATON_Output_Buffers_Info_face_recognition();
  void *p_in  = LL_Buffer_addr_start(&chk_in [0]);
  void *p_out = LL_Buffer_addr_start(&chk_out[0]);
  printf("[FR] post-bind addrs: IN=%p OUT=%p  (in_len=%lu out_len=%lu)\r\n",
         p_in, p_out,
         (unsigned long)LL_Buffer_len(&chk_in [0]),
         (unsigned long)LL_Buffer_len(&chk_out[0]));

  if (p_in == p_out) {
    printf("[FR][WARN] IN and OUT alias at %p — this network was exported in-place; "
           "we’ll keep using shadow copy + strict cache discipline.\r\n", p_in);
  }

  /* FR init after I/O handling */
  fr_init();
  fr_set_subject(FR_SUBJECT_NAME);
  fr_set_match_threshold(0.80f);


  /* Start LCD Display camera pipe stream */
  CAM_DisplayPipe_Start(lcd_bg_buffer[0], CMW_MODE_CONTINUOUS);

  /* Threads */
  /* Threads */
  g_nn_task = xTaskCreateStatic(nn_thread_fct, "nn",
                                configMINIMAL_STACK_SIZE * 2,
                                NULL,
                                nn_priority,
                                nn_thread_stack, &nn_thread);
  assert(g_nn_task != NULL);


  hdl = xTaskCreateStatic(pp_thread_fct, "pp",
                          configMINIMAL_STACK_SIZE * 2,
                          NULL,
                          pp_priority,
                          pp_thread_stack, &pp_thread);
  assert(hdl != NULL);

  hdl = xTaskCreateStatic(dp_thread_fct, "dp",
                          configMINIMAL_STACK_SIZE * 2,
                          NULL,
                          dp_priority,
                          dp_thread_stack, &dp_thread);
  assert(hdl != NULL);

  hdl = xTaskCreateStatic(isp_thread_fct, "isp",
                          configMINIMAL_STACK_SIZE * 2,
                          NULL,
                          isp_priority,
                          isp_thread_stack, &isp_thread);
  assert(hdl != NULL);

}







int CMW_CAMERA_PIPE_FrameEventCallback(uint32_t pipe)
{
  if (pipe == DCMIPP_PIPE1)
    app_main_pipe_frame_event();
  else if (pipe == DCMIPP_PIPE2)
    app_ancillary_pipe_frame_event();

  return HAL_OK;
}

int CMW_CAMERA_PIPE_VsyncEventCallback(uint32_t pipe)
{
  if (pipe == DCMIPP_PIPE1)
    app_main_pipe_vsync_event();

  return HAL_OK;
}
