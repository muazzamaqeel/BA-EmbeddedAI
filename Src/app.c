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
#include <inttypes.h>
#include "network.h"
#include "stm32n6570_discovery_xspi.h"
#include <math.h>
#include <stdint.h>

#include "app_cam.h"
#include "app_config.h"
#include "app_postprocess.h"
#include "isp_api.h"
#include "ll_aton_runtime.h"
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

#ifdef TRACKER_MODULE
typedef struct {
  double cx;
  double cy;
  double w;
  double h;
  uint32_t id;
} tbox_info;
#endif

typedef struct
{
  uint32_t X0;
  uint32_t Y0;
  uint32_t XSize;
  uint32_t YSize;
} Rectangle_TypeDef;

typedef struct {
  SemaphoreHandle_t free;
  StaticSemaphore_t free_buffer;
  SemaphoreHandle_t ready;
  StaticSemaphore_t ready_buffer;
  int buffer_nb;
  uint8_t *buffers[BQUEUE_MAX_BUFFERS];
  int free_idx;
  int ready_idx;
} bqueue_t;

typedef struct {
  uint64_t current_total;
  uint64_t current_thread_total;
  uint64_t prev_total;
  uint64_t prev_thread_total;
  struct {
    uint64_t total;
    uint64_t thread;
    uint32_t tick;
  } history[CPU_LOAD_HISTORY_DEPTH];
} cpuload_info_t;

typedef struct {
  int32_t nb_detect;
  od_pp_outBuffer_t detects[AI_OD_PP_MAX_BOXES_LIMIT];
  int tracking_enabled;
#ifdef TRACKER_MODULE
  int tboxes_valid_nb;
  tbox_info tboxes[AI_OD_PP_MAX_BOXES_LIMIT];
#endif
  uint32_t nn_period_ms;
  uint32_t inf_ms;
  uint32_t pp_ms;
  uint32_t disp_ms;
} display_info_t;

typedef struct {
  SemaphoreHandle_t update;
  StaticSemaphore_t update_buffer;
  SemaphoreHandle_t lock;
  StaticSemaphore_t lock_buffer;
  display_info_t info;
} display_t;

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
static display_t disp;
static cpuload_info_t cpu_load;
/* screen buffer */
static uint8_t screen_buffer[LCD_BG_WIDTH * LCD_BG_HEIGHT * 2] ALIGN_32 IN_PSRAM;

/* model */
LL_ATON_DECLARE_NAMED_NN_INSTANCE_AND_INTERFACE(Default);
 /* nn input buffers */
/* Camera NN pipe delivers RGB888: 128*128*3 = 49,152 bytes */
static uint8_t nn_input_buffers[2][NN_WIDTH * NN_HEIGHT * NN_BPP] ALIGN_32 IN_PSRAM;


static bqueue_t nn_input_queue;
 /* nn output buffers */
static const uint32_t nn_out_len_user[NN_OUT_MAX_NB] = {
  NN_OUT0_SIZE, NN_OUT1_SIZE, NN_OUT2_SIZE, NN_OUT3_SIZE
};
static uint8_t nn_output_buffers[2][NN_OUT_BUFFER_SIZE] ALIGN_32;
static bqueue_t nn_output_queue;

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


static inline void dcache_align_range(void **addr, size_t *len)
{
  uintptr_t a = (uintptr_t)(*addr);
  uintptr_t a0 = a & ~((uintptr_t)31);                 // align down
  size_t extra = (size_t)(a - a0);
  size_t l0 = *len + extra;
  l0 = (l0 + 31U) & ~31U;                              // align up
  *addr = (void *)a0;
  *len  = l0;
}

static inline void DCACHE_Invalidate(void *addr, size_t len)
{
#if defined(USE_DCACHE)
  dcache_align_range(&addr, &len);
  SCB_InvalidateDCache_by_Addr(addr, len);
#else
  (void)addr; (void)len;
#endif
}

static inline void DCACHE_Clean(void *addr, size_t len)
{
#if defined(USE_DCACHE)
  dcache_align_range(&addr, &len);
  SCB_CleanDCache_by_Addr(addr, len);
#else
  (void)addr; (void)len;
#endif
}



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

static uint8_t *bqueue_get_free(bqueue_t *bq, int is_blocking)
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

static void bqueue_put_free(bqueue_t *bq)
{
  int ret;

  ret = xSemaphoreGive(bq->free);
  assert(ret == pdTRUE);
}

static uint8_t *bqueue_get_ready(bqueue_t *bq)
{
  uint8_t *res;
  int ret;

  ret = xSemaphoreTake(bq->ready, portMAX_DELAY);
  assert(ret == pdTRUE);

  res = bq->buffers[bq->ready_idx];
  bq->ready_idx = (bq->ready_idx + 1) % bq->buffer_nb;

  return res;
}

static void bqueue_put_ready(bqueue_t *bq)
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
  UTIL_LCDEx_PrintfAt(x0 + 1, y0 + 1, LEFT_MODE, classes_table[detect->class_index]);
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
static void nn_thread_fct(void *arg)
{
  /* toggles */
  #define NN_LOG_INPUT_STATS       1    /* print input float stats ~1Hz */
  #define NN_ONE_SHOT_SYNTH_TEST   0    /* OFF: don’t overwrite real frames */

  /* input/layout */
  #define INPUT_IS_BGR 0                /* camera pipe outputs RGB888 */
  #define PACK_AS_NCHW 0                /* BlazeFace is NHWC typically */

  /* Get model input buffer (internal, not user-allocated) */
  const LL_Buffer_InfoTypeDef *nn_in_info  = LL_ATON_Input_Buffers_Info_Default();
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
  int did_synth = 0;

  /* Preprocess: map [0..255] -> [-1..1] */
  const float scale = 1.0f / 255.0f;
  const float bias  = 0.0f;

  while (1)
  {
    /* Wait a captured frame (RGB888 bytes: 128*128*3) */
    uint8_t *capture_buffer = bqueue_get_ready(&nn_input_queue);
    assert(capture_buffer);

    /* IMPORTANT: DMA wrote the frame -> invalidate before CPU reads */
    DCACHE_Invalidate(capture_buffer, NN_WIDTH * NN_HEIGHT * NN_BPP);

    /* Use output queue only as a token to sync with pp thread */
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
      for (int i = 0; i < PIX; ++i) {
        float r = (float)(*src++), g = (float)(*src++), b = (float)(*src++);
#if INPUT_IS_BGR
        float t = r; r = b; b = t;
#endif
        r = r * scale + bias; g = g * scale + bias; b = b * scale + bias;
        dstR[i] = r; dstG[i] = g; dstB[i] = b;
#if NN_LOG_INPUT_STATS
        if ((HAL_GetTick() - last_inp_print) >= 1000U && ((i & 0x3F) == 0)) {
          if (r < mn) mn = r;
          if (r > mx) mx = r;
          sum += r; cnt++;
          if (g < mn) mn = g;
          if (g > mx) mx = g;
          sum += g; cnt++;
          if (b < mn) mn = b;
          if (b > mx) mx = b;
          sum += b; cnt++;
        }

#endif
      }
#else
      float *dst = aton_in;
      for (int i = 0; i < PIX; ++i) {
        float r = (float)(*src++), g = (float)(*src++), b = (float)(*src++);
#if INPUT_IS_BGR
        float t = r; r = b; b = t;
#endif
        r = r * scale + bias; g = g * scale + bias; b = b * scale + bias;
        *dst++ = r; *dst++ = g; *dst++ = b;
#if NN_LOG_INPUT_STATS
        if ((HAL_GetTick() - last_inp_print) >= 1000U && ((i & 0x3F) == 0)) {
          if (r < mn) mn = r; if (r > mx) mx = r; sum += r; cnt++;
          if (g < mn) mn = g; if (g > mx) mx = g; sum += g; cnt++;
          if (b < mn) mn = b; if (b > mx) mx = b; sum += b; cnt++;
        }
#endif
      }
#endif /* PACK_AS_NCHW */

#if NN_ONE_SHOT_SYNTH_TEST
      if (!did_synth) {
        int n = (int)(aton_in_len / (int)sizeof(float));
        for (int k = 0; k < n; ++k) aton_in[k] = 0.5f;
        did_synth = 1;
        printf("[NN] one-shot synthetic input (all 0.5f) injected\r\n");
      }
#endif

      /* cache clean: CPU wrote input, NPU will read it */
      DCACHE_Clean(aton_in, aton_in_len);

#if NN_LOG_INPUT_STATS
      if ((HAL_GetTick() - last_inp_print) >= 1000U) {
        float mean = (cnt > 0) ? (sum / (float)cnt) : 0.f;
        printf("[NN] input stats: min=%.3f max=%.3f mean=%.3f  [%s, %s]\r\n",
               mn, mx, mean, INPUT_IS_BGR ? "BGR" : "RGB", PACK_AS_NCHW ? "NCHW" : "NHWC");
        last_inp_print = HAL_GetTick();
      }
#endif
    }

    /* -------- Inference -------- */
    {
      /* Clean output buffers before NPU writes to them */
    	/* Prepare outputs for NPU write: invalidate before peripheral writes */
    	/* Prepare outputs for NPU write: Clean+Invalidate BEFORE run */
    	const LL_Buffer_InfoTypeDef *nn_out_info = LL_ATON_Output_Buffers_Info_Default();
    	for (int i = 0; i < NN_OUT_NB; ++i) {
    	    void *oaddr = LL_Buffer_addr_start(&nn_out_info[i]);
    	    size_t olen = (size_t)LL_Buffer_len(&nn_out_info[i]);  // was uint32_t
    	#if defined(USE_DCACHE)
    	    dcache_align_range(&oaddr, &olen);
    	    SCB_CleanInvalidateDCache_by_Addr(oaddr, (int)olen);
    	#endif
    	}



    }

    uint32_t ts = HAL_GetTick();
    LL_ATON_RT_Main(&NN_Instance_Default);
    uint32_t inf_ms = HAL_GetTick() - ts;

    /* queues */
    bqueue_put_free(&nn_input_queue);
    bqueue_put_ready(&nn_output_queue);

    /* publish stats to display */
    int ret = xSemaphoreTake(disp.lock, portMAX_DELAY);  assert(ret == pdTRUE);
    disp.info.inf_ms = inf_ms;
    disp.info.nn_period_ms = nn_period_ms;
    ret = xSemaphoreGive(disp.lock);                     assert(ret == pdTRUE);
  }
}






#ifdef TRACKER_MODULE
static int TRK_Init()
{
  const trk_conf_t cfg = {
    .track_thresh = 0.25,
    .det_thresh = 0.8,
    .sim1_thresh = 0.8,
    .sim2_thresh = 0.5,
    .tlost_cnt = 30,
  };

  return trk_init(&trk_ctx, (trk_conf_t *) &cfg, ARRAY_NB(tboxes), tboxes);
}

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

static void roi_to_dbox(od_pp_outBuffer_t *roi, trk_dbox_t *dbox)
{
  dbox->conf = roi->conf;
  dbox->cx = roi->x_center;
  dbox->cy = roi->y_center;
  dbox->w = roi->width;
  dbox->h = roi->height;
}

static int app_tracking(od_pp_out_t *pp)
{
  int tracking_enabled = update_and_capture_tracking_enabled();
  int ret;
  int i;

  if (!tracking_enabled)
    return 0;

  for (i = 0; i < pp->nb_detect; i++)
    roi_to_dbox(&pp->pOutBuff[i], &dboxes[i]);

  ret = trk_update(&trk_ctx, pp->nb_detect, dboxes);
  assert(ret == 0);

  return 1;
}

static void tbox_to_tbox_info(trk_tbox_t *tbox, tbox_info *tinfo)
{
  tinfo->cx = tbox->cx;
  tinfo->cy = tbox->cy;
  tinfo->w = tbox->w;
  tinfo->h = tbox->h;
  tinfo->id = tbox->id;
}
#else
static int app_tracking(od_pp_out_t *pp)
{
  return 0;
}
#endif











static void pp_thread_fct(void *arg)
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

  const LL_Buffer_InfoTypeDef *nn_out_info = LL_ATON_Output_Buffers_Info_Default();

  /* print once */
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
  int tracking_enabled;
  uint32_t nn_pp[2];
  int ret, i;

  /* init postprocess */
#if POSTPROCESS_TYPE == POSTPROCESS_CUSTOM
  app_postprocess_init(NULL);
#else
  app_postprocess_init(&pp_params);
#endif

  const uint32_t PRINT_BLOB_EVERY_MS   = 500;  /* throttle heavy logs */
  const uint32_t PRINT_RESULT_EVERY_MS = 250;
  uint32_t last_blob_print   = 0;
  uint32_t last_result_print = 0;

  while (1)
  {
    /* wait “inference-done” token from nn thread */
    (void)bqueue_get_ready(&nn_output_queue);

    /* grab internal output pointers/sizes */
    for (i = 0; i < NN_OUT_NB; i++) {
      pp_input[i] = LL_Buffer_addr_start(&nn_out_info[i]);
      pp_len[i]   = LL_Buffer_len(&nn_out_info[i]);
      /* cache invalidate: NPU wrote, CPU will read */
      DCACHE_Invalidate(pp_input[i], pp_len[i]);
    }

    /* ---------- BlazeFace: convert logits -> probabilities (sigmoid) ---------- */
    int idx512 = -1, idx384 = -1;  /* large/small score heads */
    for (int j = 0; j < NN_OUT_NB; ++j) {
      int nf = (int)(pp_len[j] / (uint32_t)sizeof(float));
      if (nf == 512) idx512 = j;
      if (nf == 384) idx384 = j;
    }
    if (idx512 >= 0) {
      float *p = (float*)pp_input[idx512];
      for (int k = 0; k < 512; ++k) { float v = p[k]; p[k] = 1.f / (1.f + expf(-v)); }
    }
    if (idx384 >= 0) {
      float *p = (float*)pp_input[idx384];
      for (int k = 0; k < 384; ++k) { float v = p[k]; p[k] = 1.f / (1.f + expf(-v)); }
    }
    /* ------------------------------------------------------------------------- */

    /* blob quick stats (throttled) — now scores are in [0..1] */
    {
      uint32_t now = HAL_GetTick();
      if ((now - last_blob_print) >= PRINT_BLOB_EVERY_MS) {
        for (int j = 0; j < NN_OUT_NB; ++j) {
          int nf = (int)(pp_len[j] / (int)sizeof(float));
          const float *p = (const float*)pp_input[j];
          float maxv = -1e30f, minv = 1e30f, sum = 0.f; int cnt = 0, nz = 0, nan_cnt = 0;
          for (int k = 0; k < nf; k++) {
            float v = p[k];
            if (v != v) { nan_cnt++; continue; } /* NaN */
            if (v > maxv) maxv = v;
            if (v < minv) minv = v;
            sum += v; cnt++;
            if (v != 0.f) nz++;
          }
          float mean = cnt ? (sum / (float)cnt) : 0.f;
          printf("[NN] out%d nf=%d  min=% .5f  max=% .5f  mean=% .5f  nz=%d  nan=%d\r\n",
                 j, nf, minv, maxv, mean, nz, nan_cnt);
        }

        if (idx512 >= 0 && idx384 >= 0) {
          const float *scrL = (const float*)pp_input[idx512];
          const float *scrS = (const float*)pp_input[idx384];
          float maxL = -1e30f, sumL = 0.f; int cntL = 0;
          for (int k = 0; k < 512; ++k) { float v = scrL[k]; if (v==v) { if (v > maxL) maxL = v; sumL += v; cntL++; } }
          float avgL = cntL ? (sumL / (float)cntL) : 0.f;
          float maxS = -1e30f, sumS = 0.f; int cntS = 0;
          for (int k = 0; k < 384; ++k) { float v = scrS[k]; if (v==v) { if (v > maxS) maxS = v; sumS += v; cntS++; } }
          float avgS = cntS ? (sumS / (float)cntS) : 0.f;
          printf("[NN] scores  L:max=%.3f avg=%.3f  S:max=%.3f avg=%.3f  (o%d,o%d)\r\n",
                 maxL, avgL, maxS, avgS, idx512, idx384);
        }
        last_blob_print = now;
      }
    }

    /* run postprocess */
    pp_output.pOutBuff  = NULL;
    pp_output.nb_detect = 0;

    nn_pp[0] = HAL_GetTick();
#if POSTPROCESS_TYPE == POSTPROCESS_CUSTOM
    ret = app_postprocess_run((void **)pp_input, NN_OUT_NB, &pp_output, NULL);
#else
    ret = app_postprocess_run((void **)pp_input, NN_OUT_NB, &pp_output, &pp_params);
#endif
    assert(ret == 0);
    tracking_enabled = app_tracking(&pp_output);
    nn_pp[1] = HAL_GetTick();

    /* pretty result line (throttled) */
    {
      uint32_t now = HAL_GetTick();
      if ((now - last_result_print) >= PRINT_RESULT_EVERY_MS) {
        if (pp_output.nb_detect > 0) {
          const od_pp_outBuffer_t *d = &pp_output.pOutBuff[0];
          printf("[PP] Face Found  n=%d  (cx=%.2f cy=%.2f w=%.2f h=%.2f conf=%.2f)\r\n",
                 (int)pp_output.nb_detect, d->x_center, d->y_center, d->width, d->height, d->conf);
        } else {
          printf("[PP] Face NOT Found\r\n");
        }
        last_result_print = now;
      }
    }

    /* update display */
    ret = xSemaphoreTake(disp.lock, portMAX_DELAY);  assert(ret == pdTRUE);
    disp.info.nb_detect = pp_output.nb_detect;
    for (i = 0; i < pp_output.nb_detect; i++)
      disp.info.detects[i] = pp_output.pOutBuff[i];
#ifdef TRACKER_MODULE
    disp.info.tracking_enabled = tracking_enabled;
    disp.info.tboxes_valid_nb = 0;
    for (i = 0; i < ARRAY_NB(tboxes); i++) {
      if (!tboxes[i].is_tracking || tboxes[i].tlost_cnt) continue;
      tbox_to_tbox_info(&tboxes[i], &disp.info.tboxes[disp.info.tboxes_valid_nb]);
      disp.info.tboxes_valid_nb++;
    }
#endif
    disp.info.pp_ms = nn_pp[1] - nn_pp[0];
    ret = xSemaphoreGive(disp.lock);                 assert(ret == pdTRUE);

    /* hand token back + wake display */
    bqueue_put_free(&nn_output_queue);
    xSemaphoreGive(disp.update);
  }
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
    volatile const uint32_t *pA = (const uint32_t*)0x71000000u;  // base
    volatile const uint32_t *pB = (const uint32_t*)0x71028590u;  // A_vector (scale) from network.c
    volatile const uint32_t *pC = (const uint32_t*)0x71027FA0u;  // C_vector (bias)  from network.c

    printf("[XSPI] 71000000: %08lX %08lX %08lX %08lX\r\n", pA[0], pA[1], pA[2], pA[3]);
    printf("[XSPI] 71028590: %08lX %08lX %08lX %08lX\r\n", pB[0], pB[1], pB[2], pB[3]);
    printf("[XSPI] 71027FA0: %08lX %08lX %08lX %08lX\r\n", pC[0], pC[1], pC[2], pC[3]);
}

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>


/* Option A: hardcode once from your .raw size (166225) */
#ifndef DETECT_WEIGHTS_SIZE
#define DETECT_WEIGHTS_SIZE (166225u)
#endif

static inline uint32_t rd32(uintptr_t a){ return *(volatile const uint32_t *)a; }

static void dump_weights_span(void)
{
    uintptr_t base = XSPI2_BASE;
    size_t len = (size_t)DETECT_WEIGHTS_SIZE;

    printf("[DETECT] weights @ [0x%08" PRIxPTR " .. 0x%08" PRIxPTR ")  size=%u bytes\r\n",
           base, base + len, (unsigned)len);

    /* quick sanity peek */
    printf("[XSPI] head=%08" PRIx32 " tail=%08" PRIx32 "\r\n",
           rd32(base), rd32(base + len - 4));
}


void app_run()
{
	  int inst = xspi_enable_mmap_auto();
	  dump_weights_span();
	  if (inst >= 0) {
	    xspi_quick_check();  // should match your CubeProgrammer readback
	  }

  UBaseType_t isp_priority = FREERTOS_PRIORITY(2);
  UBaseType_t pp_priority = FREERTOS_PRIORITY(-2);
  UBaseType_t dp_priority = FREERTOS_PRIORITY(-2);
  UBaseType_t nn_priority = FREERTOS_PRIORITY(1);
  TaskHandle_t hdl;
  int ret;

  printf("Init application\n");
  /* Enable DWT so DWT_CYCCNT works when debugger not attached */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  /* screen init */
  memset(lcd_bg_buffer, 0, sizeof(lcd_bg_buffer));
  CACHE_OP(SCB_CleanInvalidateDCache_by_Addr(lcd_bg_buffer, sizeof(lcd_bg_buffer)));
  memset(lcd_fg_buffer, 0, sizeof(lcd_fg_buffer));
  CACHE_OP(SCB_CleanInvalidateDCache_by_Addr(lcd_fg_buffer, sizeof(lcd_fg_buffer)));
  Display_init();

  /* create buffer queues */
  ret = bqueue_init(&nn_input_queue, 2, (uint8_t *[2]){nn_input_buffers[0], nn_input_buffers[1]});
  assert(ret == 0);
  ret = bqueue_init(&nn_output_queue, 2, (uint8_t *[2]){nn_output_buffers[0], nn_output_buffers[1]});
  assert(ret == 0);

#ifdef TRACKER_MODULE
  ret = TRK_Init();
  assert(ret == 0);
  ret = BSP_PB_Init(BUTTON_TOGGLE_TRACKING, BUTTON_MODE_GPIO);
  assert(ret == BSP_ERROR_NONE);
#endif

  cpuload_init(&cpu_load);

  /*** Camera Init ************************************************************/  
  CAM_Init();

  /* sems + mutex init */
  isp_sem = xSemaphoreCreateCountingStatic(1, 0, &isp_sem_buffer);
  assert(isp_sem);
  disp.update = xSemaphoreCreateCountingStatic(1, 0, &disp.update_buffer);
  assert(disp.update);
  disp.lock = xSemaphoreCreateMutexStatic(&disp.lock_buffer);
  assert(disp.lock);

  /* Start LCD Display camera pipe stream */
  CAM_DisplayPipe_Start(lcd_bg_buffer[0], CMW_MODE_CONTINUOUS);

  /* threads init */
  hdl = xTaskCreateStatic(nn_thread_fct, "nn", configMINIMAL_STACK_SIZE * 2, NULL, nn_priority, nn_thread_stack,
                          &nn_thread);
  assert(hdl != NULL);
  hdl = xTaskCreateStatic(pp_thread_fct, "pp", configMINIMAL_STACK_SIZE * 2, NULL, pp_priority, pp_thread_stack,
                          &pp_thread);
  assert(hdl != NULL);
  hdl = xTaskCreateStatic(dp_thread_fct, "dp", configMINIMAL_STACK_SIZE * 2, NULL, dp_priority, dp_thread_stack,
                          &dp_thread);
  assert(hdl != NULL);
  hdl = xTaskCreateStatic(isp_thread_fct, "isp", configMINIMAL_STACK_SIZE * 2, NULL, isp_priority, isp_thread_stack,
                          &isp_thread);
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
