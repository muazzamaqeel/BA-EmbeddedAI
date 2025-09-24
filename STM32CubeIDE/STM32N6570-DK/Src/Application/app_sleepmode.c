/**
 ******************************************************************************
 * @file    app_sleepmode.c
 * @brief   Sleep when no face detected, wake on touch
 ******************************************************************************
 */

#include "app_sleepmode.h"
#include "cmw_camera.h"
#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_touch.h"
#include <stdio.h>
#include <stdbool.h>
#include "app_cam.h"

static volatile bool g_sleep_active = false;
static uint32_t g_last_face_time = 0;

static void SleepMode_Task(void *arg);
static void enter_sleep(void);
static void exit_sleep(void);

static StaticTask_t sleep_task_tcb;
static StackType_t  sleep_task_stack[configMINIMAL_STACK_SIZE];
static TaskHandle_t hSleepTask = NULL;

/* ===== Public API ===== */
void APP_SleepMode_Init(void)
{
    if (hSleepTask == NULL) {
        hSleepTask = xTaskCreateStatic(SleepMode_Task, "sleep",
                                       configMINIMAL_STACK_SIZE,
                                       NULL,
                                       tskIDLE_PRIORITY + 1,
                                       sleep_task_stack, &sleep_task_tcb);
        configASSERT(hSleepTask != NULL);
    }
}

bool APP_SleepMode_IsActive(void)
{
    return g_sleep_active;
}

/* Called from detector display/update code */
void APP_SleepMode_UpdateFaceActivity(bool face_detected)
{
    if (face_detected) {
        g_last_face_time = HAL_GetTick();
    }
}

/* ===== Internal ===== */
static void enter_sleep(void)
{
    if (g_sleep_active) return;
    g_sleep_active = true;
    printf("[SLEEP] Entering sleep mode...\r\n");

    CMW_CAMERA_Suspend(DCMIPP_PIPE1);
    CMW_CAMERA_Suspend(DCMIPP_PIPE2);

    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);
}
static void exit_sleep(void)
{
    if (!g_sleep_active) return;
    g_sleep_active = false;
    printf("[SLEEP] Exiting sleep mode...\r\n");

    // Resume camera pipes safely
    CMW_CAMERA_Resume(DCMIPP_PIPE1);
    CMW_CAMERA_Resume(DCMIPP_PIPE2);

    // OR, if resume not supported, restart with buffer reassign:
    // CAM_DisplayPipe_Start(APP_GetLcdBgBuffer0(), CMW_MODE_CONTINUOUS);

    UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);
}

/* ===== Task ===== */
static void SleepMode_Task(void *arg)
{
    (void)arg;
    TS_State_t ts;

    while (1) {
        if (!g_sleep_active) {
            if ((HAL_GetTick() - g_last_face_time) > 30000) { // 30s no face
                enter_sleep();
            }
        } else {
            if (BSP_TS_GetState(0, &ts) == BSP_ERROR_NONE && ts.TouchDetected) {
                exit_sleep();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
