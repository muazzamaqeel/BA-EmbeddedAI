/**
 ******************************************************************************
 * @file    app_ui_task.c
 * @brief   Central UI task (FreeRTOS) with state machine:
 *          START screen -> PIN screen -> RUNNING
 ******************************************************************************
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

#include "app_ui_start.h"
#include "app_ui_pin.h"
#include "app_sleepmode.h"
#include "app_tasks.h"   // for Start_ApplicationTasks()

/* ====== UI States ====== */
typedef enum {
    UI_STATE_START,
    UI_STATE_PIN,
    UI_STATE_RUNNING
} UI_State_t;

/* Current UI state */
static UI_State_t ui_state = UI_STATE_START;

/* ====== Static task allocation ====== */
static StaticTask_t ui_task_tcb;
static StackType_t  ui_task_stack[2048];   // 2048 words (~8 KB stack)

/* ====== UI Task ====== */
static void UI_Task(void *arg)
{
    (void)arg;

    printf("[UI_TASK] Started\r\n");

    while (1) {
        switch (ui_state) {

        case UI_STATE_START: {
            UI_StartScreen_Show();
            UI_ButtonResult_t btn = UI_WaitForButton();

            if (btn == UI_BTN_START) {
                printf("[UI] Start pressed -> go RUNNING\r\n");
                ui_state = UI_STATE_RUNNING;
            }
            else if (btn == UI_BTN_PIN) {
                printf("[UI] PIN button pressed -> go PIN\r\n");
                ui_state = UI_STATE_PIN;
            }
            break;
        }

        case UI_STATE_PIN: {
            UI_PinScreen_Show();
            UI_PinScreen_WaitForOK();
            printf("[UI] PIN correct -> go RUNNING\r\n");
            ui_state = UI_STATE_RUNNING;
            break;
        }

        case UI_STATE_RUNNING: {
            printf("[UI] Launching application tasks...\r\n");
            Start_ApplicationTasks();
            APP_SleepMode_Init();

            /* At this point, UI is no longer needed */
            printf("[UI_TASK] Done -> suspending task\r\n");
            vTaskSuspend(NULL);
            break;
        }

        default:
            ui_state = UI_STATE_START;
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(50)); // debounce / yield
    }
}

void UI_StartTask(void)
{
    static TaskHandle_t hdl = NULL;

    if (hdl != NULL) {
        printf("[UI_StartTask] UI task already created, skipping.\r\n");
        return;
    }

    hdl = xTaskCreateStatic(
        UI_Task,
        "UI",
        sizeof(ui_task_stack) / sizeof(StackType_t),
        NULL,
        tskIDLE_PRIORITY + 2,
        ui_task_stack,
        &ui_task_tcb
    );

    configASSERT(hdl != NULL);
}
