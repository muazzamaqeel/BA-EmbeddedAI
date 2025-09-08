#pragma once

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "app_postprocess.h"
#ifdef TRACKER_MODULE
#include "tracker.h"
#endif
#include "ll_aton_runtime.h"  // for LL_Buffer_InfoTypeDef

#ifndef BQUEUE_MAX_BUFFERS
#define BQUEUE_MAX_BUFFERS 2
#endif
#ifndef CPU_LOAD_HISTORY_DEPTH
#define CPU_LOAD_HISTORY_DEPTH 8
#endif
#ifndef AI_OD_PP_MAX_BOXES_LIMIT
#define AI_OD_PP_MAX_BOXES_LIMIT 32
#endif

/* Rectangle_TypeDef used by app.c for LCD areas */
typedef struct {
  uint32_t X0;
  uint32_t Y0;
  uint32_t XSize;
  uint32_t YSize;
} Rectangle_TypeDef;

/* Lightweight CPU load struct; functions live in app.c */
typedef struct {
  struct {
    uint64_t total;
    uint64_t thread;
    uint32_t tick;
  } history[CPU_LOAD_HISTORY_DEPTH];
} cpuload_info_t;

/* Simple 2-buffer queue */
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

#ifdef TRACKER_MODULE
typedef struct {
  double cx;
  double cy;
  double w;
  double h;
  uint32_t id;
} tbox_info;
#endif

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

/* bqueue API (implemented in app.c) */
uint8_t *bqueue_get_free (bqueue_t *bq, int is_blocking);
void     bqueue_put_free (bqueue_t *bq);
uint8_t *bqueue_get_ready(bqueue_t *bq);
void     bqueue_put_ready(bqueue_t *bq);

/* Globals defined in app.c */
extern bqueue_t  nn_input_queue;
extern bqueue_t  nn_output_queue;
extern display_t disp;

/* Detector wrappers (implemented in app.c; avoid referencing NN_Instance_Default across TUs) */
const LL_Buffer_InfoTypeDef *Detector_In_Info(void);
const LL_Buffer_InfoTypeDef *Detector_Out_Info(void);
void Detector_Run(void);
