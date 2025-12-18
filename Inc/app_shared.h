#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "app_postprocess.h"
#ifdef TRACKER_MODULE
#include "tracker.h"
#endif
#include "ll_aton_runtime.h"

/* ------------------------------------------------------------
 * GLOBAL SHARED STATES (defined once in app_shared.c)
 * ------------------------------------------------------------ */
extern bool g_pipeline_running;   // Pipeline running flag
extern bool g_fr_active;          // Face Recognition active flag

extern uint32_t g_last_face_time; // Timestamp of last detected face
extern uint32_t g_wake_time;      // Timestamp of last wake-up event

/* ------------------------------------------------------------
 * CONFIG LIMITS
 * ------------------------------------------------------------ */
#ifndef BQUEUE_MAX_BUFFERS
#define BQUEUE_MAX_BUFFERS 2
#endif

#ifndef CPU_LOAD_HISTORY_DEPTH
#define CPU_LOAD_HISTORY_DEPTH 8
#endif

#ifndef AI_OD_PP_MAX_BOXES_LIMIT
#define AI_OD_PP_MAX_BOXES_LIMIT 32
#endif

/* ------------------------------------------------------------
 * Rectangle definition (used for LCD background/foreground)
 * ------------------------------------------------------------ */
typedef struct {
    uint32_t X0;
    uint32_t Y0;
    uint32_t XSize;
    uint32_t YSize;
} Rectangle_TypeDef;

/* ------------------------------------------------------------
 * CPU load info (implementation in app.c)
 * ------------------------------------------------------------ */
typedef struct {
    struct {
        uint64_t total;
        uint64_t thread;
        uint32_t tick;
    } history[CPU_LOAD_HISTORY_DEPTH];
} cpuload_info_t;

/* ------------------------------------------------------------
 * Simple 2-buffer queue for NN input/output
 * ------------------------------------------------------------ */
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

/* ------------------------------------------------------------
 * Tracking information (only if tracking enabled)
 * ------------------------------------------------------------ */
#ifdef TRACKER_MODULE
typedef struct {
    double cx;
    double cy;
    double w;
    double h;
    uint32_t id;
} tbox_info;
#endif

/* ------------------------------------------------------------
 * Display information sent to dp thread
 * ------------------------------------------------------------ */
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

/* Semaphore-protected display info wrapper */
typedef struct {
    SemaphoreHandle_t update;
    StaticSemaphore_t update_buffer;

    SemaphoreHandle_t lock;
    StaticSemaphore_t lock_buffer;

    display_info_t info;
} display_t;

/* ------------------------------------------------------------
 * bqueue API
 * ------------------------------------------------------------ */
uint8_t *bqueue_get_free(bqueue_t *bq, int is_blocking);
void     bqueue_put_free(bqueue_t *bq);

uint8_t *bqueue_get_ready(bqueue_t *bq);
void     bqueue_put_ready(bqueue_t *bq);

/* ------------------------------------------------------------
 * GLOBAL QUEUES & DISPLAY (defined in app.c)
 * ------------------------------------------------------------ */
extern bqueue_t  nn_input_queue;
extern bqueue_t  nn_output_queue;
extern display_t disp;

/* ------------------------------------------------------------
 * Detector wrappers (no cross-TU reference to NN_Instance_Default)
 * ------------------------------------------------------------ */
const LL_Buffer_InfoTypeDef *Detector_In_Info(void);
const LL_Buffer_InfoTypeDef *Detector_Out_Info(void);
void Detector_Run(void);


typedef struct
{
    /* preprocessing */
    uint32_t det_pre_cycles;

    /* inference (Detector_Run) */
    uint32_t det_infer_cycles;

    /* total = pre + infer */
    uint32_t det_total_cycles;

    /* FR */
    uint32_t fr_pre_cycles;
    uint32_t fr_infer_cycles;
    uint32_t fr_total_cycles;

    /* statistics */
    uint32_t det_count;
    uint32_t det_infer_cycles_avg;
} det_profile_t;

extern volatile det_profile_t g_det_prof;
