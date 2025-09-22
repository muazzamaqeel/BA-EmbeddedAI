// app_tasks.c
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pipeline_start.h"

// --- Pipeline task control block + stack ---
static StaticTask_t pipeline_thread;
static StackType_t pipeline_stack[8192];

static void Pipeline_Task(void *arg)
{
    printf("[PIPELINE] Task started\r\n");
    Pipeline_Start();
    vTaskDelete(NULL);
}

void Start_ApplicationTasks(void)
{
    // Create Pipeline Task
    xTaskCreateStatic(
        Pipeline_Task,
        "Pipeline",
        8192,
        NULL,
        tskIDLE_PRIORITY + 2,
        pipeline_stack,
        &pipeline_thread
    );

    // In future: add UI task, logging task, etc.
}
