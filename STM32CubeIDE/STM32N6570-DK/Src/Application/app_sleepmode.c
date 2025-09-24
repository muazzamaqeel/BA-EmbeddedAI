/**
 *******************************************************************************
 * @file    app_sleepmode.c
 * @brief   Sleep when no face detected, wake on touch
 *******************************************************************************
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
#include "stm32n6570_discovery_lcd.h"
#include "stm32n6570_discovery.h"   // for HAL and GPIO macros


static volatile bool g_sleep_active = false;
static uint32_t g_last_face_time = 0;
static uint32_t g_wake_time = 0;  // Time when the system last woke up from sleep
static bool touch_detected_since_last_sleep = false;

static void SleepMode_Task(void *arg);
static void enter_sleep(void);
static void exit_sleep(void);

static StaticTask_t sleep_task_tcb;
static StackType_t  sleep_task_stack[configMINIMAL_STACK_SIZE];
static TaskHandle_t hSleepTask = NULL;


static void LCD_Backlight_On(void)
{
    LCD_DISP_BL_GPIO_CLK_ENABLE();
    HAL_GPIO_WritePin(LCD_DISP_BL_GPIO_PORT, LCD_DISP_BL_PIN, GPIO_PIN_SET);
}

static void LCD_Backlight_Off(void)
{
    LCD_DISP_BL_GPIO_CLK_ENABLE();
    HAL_GPIO_WritePin(LCD_DISP_BL_GPIO_PORT, LCD_DISP_BL_PIN, GPIO_PIN_RESET);
}

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
        touch_detected_since_last_sleep = false;  // Reset touch flag if face detected
    }
}

/* ===== Task ===== */
static void SleepMode_Task(void *arg)
{
    (void)arg;
    TS_State_t ts;
    bool face_present_now = false;

    while (1) {
        if (!g_sleep_active) {
            // Check if a face is currently present (last update < 500ms ago)
            face_present_now = (HAL_GetTick() - g_last_face_time) < 500;

            if (!face_present_now) {
                // No face → start countdown
                if ((HAL_GetTick() - g_last_face_time) > 7000 &&
                    !touch_detected_since_last_sleep) {
                    enter_sleep();
                }
            }
            // else: face present → block sleep completely
        } else {
            // Exit sleep only by touch
            if (BSP_TS_GetState(0, &ts) == BSP_ERROR_NONE && ts.TouchDetected) {
                if (!touch_detected_since_last_sleep) {
                    touch_detected_since_last_sleep = true;
                    exit_sleep();
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(200));
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

    UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

    BSP_LCD_DisplayOff(0);   // LTDC off
    LCD_Backlight_Off();     // backlight GPIO off
}

static void exit_sleep(void)
{
    if (!g_sleep_active) return;
    g_sleep_active = false;
    printf("[SLEEP] Exiting sleep mode...\r\n");

    BSP_LCD_DisplayOn(0);    // LTDC on
    LCD_Backlight_On();      // backlight GPIO on
    UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

    CMW_CAMERA_Resume(DCMIPP_PIPE1);
    CMW_CAMERA_Resume(DCMIPP_PIPE2);

    // Reset flags and timers after exiting sleep mode
    g_last_face_time = HAL_GetTick();  // Reset face detection timer
    g_wake_time = g_last_face_time;   // Mark the wake time
    touch_detected_since_last_sleep = false;  // Reset touch flag
}

