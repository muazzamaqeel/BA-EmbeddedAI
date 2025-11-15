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
#include "stm32n6570_discovery.h"

/* --------------------------------------------------------
 * GLOBAL SLEEP STATE
 * -------------------------------------------------------- */
static volatile bool g_sleep_active = false;
static uint32_t g_last_face_time = 0;
static uint32_t g_wake_time = 0;
static bool touch_detected_since_last_sleep = false;
static volatile bool g_sleep_disabled_override = false;
static volatile bool g_sleep_counter_enabled = false;

/* RTOS task storage */
static StaticTask_t sleep_task_tcb;
static StackType_t sleep_task_stack[configMINIMAL_STACK_SIZE];
static TaskHandle_t hSleepTask = NULL;

static void SleepMode_Task(void *arg);
static void enter_sleep(void);
static void exit_sleep(void);

/* --------------------------------------------------------
 * LCD Backlight control
 * -------------------------------------------------------- */
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

/* --------------------------------------------------------
 * Initialization
 * -------------------------------------------------------- */
void APP_SleepMode_Init(void)
{
    if (hSleepTask == NULL)
    {
        hSleepTask = xTaskCreateStatic(
            SleepMode_Task,
            "sleep",
            configMINIMAL_STACK_SIZE,
            NULL,
            tskIDLE_PRIORITY,          // Lowest → more WFI time
            sleep_task_stack,
            &sleep_task_tcb
        );
        configASSERT(hSleepTask != NULL);
    }
}

/* --------------------------------------------------------
 * Control functions
 * -------------------------------------------------------- */
bool APP_SleepMode_IsActive(void)
{
    return g_sleep_active;
}

void APP_SleepMode_Disable(void)
{
    g_sleep_disabled_override = true;
    printf("S0\n");   // Sleep override → disabled
}

void APP_SleepMode_Enable(void)
{
    g_sleep_disabled_override = false;
    printf("S1\n");   // Sleep override → enabled
}

void APP_SleepMode_EnableCounter(bool enable)
{
    g_sleep_counter_enabled = enable;
    printf(enable ? "S1\n" : "S0\n");
}

void APP_SleepMode_UpdateFaceActivity(bool face_detected)
{
    if (!g_sleep_counter_enabled)
        return;

    if (face_detected)
    {
        g_last_face_time = HAL_GetTick();
        touch_detected_since_last_sleep = false;
        printf("FA\n");   // Face Activity
    }
}

/* --------------------------------------------------------
 * SleepMode RTOS Task
 * -------------------------------------------------------- */
static const uint32_t WAKE_GRACE_MS = 1500;

static void SleepMode_Task(void *arg)
{
    (void)arg;

    TS_State_t ts;
    bool face_present_now = false;
    static bool prev_touch = false;

    extern bool g_pipeline_running;
    extern bool g_fr_active;

    while (1)
    {
        /* COUNTER DISABLED */
        if (!g_sleep_counter_enabled)
        {
            vTaskDelay(pdMS_TO_TICKS(350));
            continue;
        }

        /* PIPELINE OR FACE RECOGNITION RUNNING */
        if (g_pipeline_running || g_fr_active)
        {
            printf("SP\n");   // Sleep Prevented by pipeline
            vTaskDelay(pdMS_TO_TICKS(350));
            continue;
        }

        /* SLEEP OVERRIDE */
        if (g_sleep_disabled_override)
        {
            printf("S0\n");   // Override disables sleep
            vTaskDelay(pdMS_TO_TICKS(350));
            continue;
        }

        /* WAKE GRACE PERIOD */
        if (!g_sleep_active && (HAL_GetTick() - g_wake_time) < WAKE_GRACE_MS)
        {
            printf("SG\n");   // Grace period active
            prev_touch = false;
            vTaskDelay(pdMS_TO_TICKS(200));
            continue;
        }

        /* --------------------------------------------------------
         * ACTIVE MODE
         * -------------------------------------------------------- */
        if (!g_sleep_active)
        {
            face_present_now = (HAL_GetTick() - g_last_face_time) < 500;

            if (!face_present_now)
            {
                // Face gone → starting countdown
                printf("FD\n");

                if ((HAL_GetTick() - g_last_face_time) > 7000 &&
                    !touch_detected_since_last_sleep)
                {
                    printf("SL\n");  // Entering Sleep
                    enter_sleep();
                    prev_touch = false;
                }
            }

            vTaskDelay(pdMS_TO_TICKS(350));
            continue;
        }

        /* --------------------------------------------------------
         * SLEEP MODE (screen off)
         * -------------------------------------------------------- */
        if (BSP_TS_GetState(0, &ts) == BSP_ERROR_NONE)
        {
            bool edge = (ts.TouchDetected && !prev_touch);
            prev_touch = ts.TouchDetected;

            if (edge && !touch_detected_since_last_sleep)
            {
                printf("ST\n");   // Touch event in sleep
                touch_detected_since_last_sleep = true;
                printf("SW\n");   // Wake
                exit_sleep();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/* --------------------------------------------------------
 * Enter / Exit Sleep
 * -------------------------------------------------------- */
static void enter_sleep(void)
{
    if (g_sleep_active)
        return;

    g_sleep_active = true;

    CMW_CAMERA_Suspend(DCMIPP_PIPE1);
    CMW_CAMERA_Suspend(DCMIPP_PIPE2);

    UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);
    BSP_LCD_DisplayOff(0);
    LCD_Backlight_Off();
}

static void exit_sleep(void)
{
    if (!g_sleep_active)
        return;

    g_sleep_active = false;

    BSP_LCD_DisplayOn(0);
    LCD_Backlight_On();

    UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

    CMW_CAMERA_Resume(DCMIPP_PIPE1);
    CMW_CAMERA_Resume(DCMIPP_PIPE2);

    HAL_Delay(600);

    extern bool USB_SD_Mount(void);
    extern bool USB_SD_EnsureMounted(void);

    USB_SD_Mount();
    USB_SD_EnsureMounted();

    g_last_face_time = HAL_GetTick();
    g_wake_time = g_last_face_time;
    touch_detected_since_last_sleep = false;
}

void APP_SleepMode_EnableCounterDelayed(uint32_t delay_ms)
{
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
    g_last_face_time = HAL_GetTick();
    g_sleep_counter_enabled = true;
    printf("S1\n");
}
