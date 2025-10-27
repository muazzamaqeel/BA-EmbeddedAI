/**
 * @file app_tasks.c
 * @brief Application-level task management for STM32N6570-DK
 *
 * This module now coordinates between the UI and the pipeline:
 *  - Pipeline task initializes everything at boot (via app_run()).
 *  - UI Start button signals a semaphore to launch the pipeline.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>
#include "pipeline_start.h"
#include "app.h"   // for app_start_pipeline()

/* ------------------------------------------------------------
 * Globals
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
    printf("[PIPELINE] Task started — initializing base pipeline...\r\n");

    /* Stage 1: initialize (LCD, queues, camera, etc.) */
    app_run();

    /* Stage 2: wait until UI Start button gives signal */
    printf("[PIPELINE] Waiting for start button...\r\n");
    xSemaphoreTake(start_sem, portMAX_DELAY);

    /* Stage 3: start camera + threads */
    printf("[PIPELINE] Start signal received! Launching camera and NN threads...\r\n");
    app_start_pipeline();

    /* Optional: wait forever or self-delete */
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000)); // Keep alive for debug
    }
}

/* ------------------------------------------------------------
 * External trigger from UI
 * ------------------------------------------------------------ */
void Pipeline_TriggerStart(void)
{
    if (start_sem != NULL) {
        printf("[PIPELINE] Trigger received from UI\r\n");
        xSemaphoreGive(start_sem);
    } else {
        printf("[PIPELINE][ERR] start_sem not initialized!\r\n");
    }
}

/* ------------------------------------------------------------
 * Main entry to create tasks
 * ------------------------------------------------------------ */
void Start_ApplicationTasks(void)
{
    printf("[APP_TASKS] Creating pipeline task + semaphore...\r\n");

    /* Create the binary semaphore (starts locked) */
    start_sem = xSemaphoreCreateBinaryStatic(&start_sem_buffer);
    configASSERT(start_sem != NULL);

    /* Create the pipeline task */
    xTaskCreateStatic(
        Pipeline_Task,
        "Pipeline",
        sizeof(pipeline_stack) / sizeof(StackType_t),
        NULL,
        tskIDLE_PRIORITY + 2,
        pipeline_stack,
        &pipeline_thread
    );

    printf("[APP_TASKS] Tasks initialized successfully.\r\n");
}
