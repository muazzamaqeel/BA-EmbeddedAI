/**
 * @file app_tasks.c
 * @brief Application-level task management for STM32N6570-DK
 *
 * This module coordinates between the UI and the pipeline:
 *  - Pipeline task initializes everything at boot (via app_run()).
 *  - UI Start button signals a semaphore to actually launch the pipeline.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

#include "pipeline_start.h"   /* For Pipeline_TriggerStart() if used externally */
#include "app.h"              /* app_run(), app_start_pipeline() */

/* ------------------------------------------------------------
 * Static RTOS objects
 * ------------------------------------------------------------ */
static StaticTask_t pipeline_thread;
static StackType_t  pipeline_stack[8192];

static SemaphoreHandle_t start_sem;
static StaticSemaphore_t start_sem_buffer;

/* ------------------------------------------------------------
 * Pipeline Task
 * ------------------------------------------------------------ */
static void Pipeline_Task(void *arg)
{
    (void)arg;

    printf("[PIPELINE] Task started — performing base initialization...\r\n");

    /* Initialize everything except camera + NN threads */
    app_run();

    printf("[PIPELINE] Waiting for UI start signal...\r\n");

    /* Wait indefinitely for user to press Start button */
    xSemaphoreTake(start_sem, portMAX_DELAY);

    printf("[PIPELINE] Start signal received — launching camera & NN pipeline...\r\n");

    /* This starts the camera, NN, display, ISP, etc. */
    app_start_pipeline();

    /* Keep task alive for debugging/logging (optional) */
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* ------------------------------------------------------------
 * External trigger called from UI button handler
 * ------------------------------------------------------------ */
void Pipeline_TriggerStart(void)
{
    if (start_sem != NULL)
    {
        printf("[PIPELINE] UI trigger: START pressed.\r\n");
        xSemaphoreGive(start_sem);
    }
    else
    {
        printf("[PIPELINE][ERR] start_sem not initialized!\r\n");
    }
}

/* ------------------------------------------------------------
 * Create tasks & semaphore
 * ------------------------------------------------------------ */
void Start_ApplicationTasks(void)
{
    printf("[APP_TASKS] Initializing pipeline task and semaphore...\r\n");

    /* Create static binary semaphore for START trigger */
    start_sem = xSemaphoreCreateBinaryStatic(&start_sem_buffer);
    configASSERT(start_sem != NULL);

    /* Create the static pipeline initialization task */
    xTaskCreateStatic(
        Pipeline_Task,
        "Pipeline",
        sizeof(pipeline_stack) / sizeof(StackType_t),
        NULL,
        tskIDLE_PRIORITY + 2,     /* Medium priority */
        pipeline_stack,
        &pipeline_thread
    );

    printf("[APP_TASKS] Tasks initialized successfully.\r\n");
}
