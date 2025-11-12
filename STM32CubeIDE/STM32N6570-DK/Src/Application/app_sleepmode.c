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

/* --- Override flag: when true, sleep is disabled manually --- */
static volatile bool g_sleep_disabled_override = false;

/* --- Counter enable flag: active only after Start button pressed --- */
static volatile bool g_sleep_counter_enabled = false;

static void SleepMode_Task(void *arg);
static void enter_sleep(void);
static void exit_sleep(void);

static StaticTask_t sleep_task_tcb;
static StackType_t  sleep_task_stack[configMINIMAL_STACK_SIZE];
static TaskHandle_t hSleepTask = NULL;

/* ================================================================
 * LCD backlight control helpers
 * ================================================================ */
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

/* ================================================================
 * Initialization
 * ================================================================ */
void APP_SleepMode_Init(void)
{
    if (hSleepTask == NULL) {
        hSleepTask = xTaskCreateStatic(
            SleepMode_Task,
            "sleep",
            configMINIMAL_STACK_SIZE,
            NULL,
            tskIDLE_PRIORITY + 1,
            sleep_task_stack,
            &sleep_task_tcb
        );
        configASSERT(hSleepTask != NULL);
    }
}

/* ================================================================
 * Public API
 * ================================================================ */
bool APP_SleepMode_IsActive(void)
{
    return g_sleep_active;
}

void APP_SleepMode_Disable(void)
{
    g_sleep_disabled_override = true;
    printf("[SLEEP] Override enabled — sleep disabled\r\n");
}

void APP_SleepMode_Enable(void)
{
    g_sleep_disabled_override = false;
    printf("[SLEEP] Override cleared — sleep enabled\r\n");
}

void APP_SleepMode_EnableCounter(bool enable)
{
    g_sleep_counter_enabled = enable;
    printf("[SLEEP] Counter %s\r\n", enable ? "ENABLED" : "DISABLED");
}

/* Called from detector display/update code */
void APP_SleepMode_UpdateFaceActivity(bool face_detected)
{
    /* Skip updates if counter is not active yet */
    if (!g_sleep_counter_enabled)
        return;

    if (face_detected) {
        g_last_face_time = HAL_GetTick();
        touch_detected_since_last_sleep = false;  // reset touch flag
    }
}

/* ================================================================
 * Task Implementation
 * ================================================================ */
static const uint32_t WAKE_GRACE_MS = 1500;   // ignore sleep/touch after wake

static void SleepMode_Task(void *arg)
{
    (void)arg;
    TS_State_t ts;
    bool face_present_now = false;
    static bool prev_touch = false;  // edge detect for touch

    /* --- Link with pipeline / FR state --- */
    extern bool g_pipeline_running;   // true when camera pipeline is active
    extern bool g_fr_active;          // true during face recognition

    while (1)
    {
        /* Sleep counter not yet started */
        if (!g_sleep_counter_enabled) {
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        /* Prevent sleep while FR or pipeline threads active */
        if (g_pipeline_running || g_fr_active) {
            vTaskDelay(pdMS_TO_TICKS(200));
            continue;
        }

        /* Global override: disable sleep completely */
        if (g_sleep_disabled_override) {
            vTaskDelay(pdMS_TO_TICKS(200));
            continue;
        }

        /* Grace window after wake-up */
        if (!g_sleep_active && (HAL_GetTick() - g_wake_time) < WAKE_GRACE_MS) {
            prev_touch = false;
            vTaskDelay(pdMS_TO_TICKS(50));
            continue;
        }

        /* ===================================================
         * ACTIVE MODE (system awake)
         * =================================================== */
        if (!g_sleep_active)
        {
            face_present_now = (HAL_GetTick() - g_last_face_time) < 500;

            if (!face_present_now)
            {
                /* Enter sleep only after 7s inactivity and no new touch */
                if ((HAL_GetTick() - g_last_face_time) > 7000 &&
                    !touch_detected_since_last_sleep)
                {
                    enter_sleep();
                    prev_touch = false;  // reset touch edge
                }
            }
        }

        /* ===================================================
         * SLEEP MODE (screen off, wake on touch)
         * =================================================== */
        else
        {
            if (BSP_TS_GetState(0, &ts) == BSP_ERROR_NONE)
            {
                bool edge = (ts.TouchDetected && !prev_touch);
                prev_touch = ts.TouchDetected;
                if (edge && !touch_detected_since_last_sleep)
                {
                    touch_detected_since_last_sleep = true;
                    exit_sleep();
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(40));
    }
}

/* ================================================================
 * Internal helpers
 * ================================================================ */
static void enter_sleep(void)
{
    if (g_sleep_active)
        return;

    g_sleep_active = true;
    printf("[SLEEP] Entering sleep mode...\r\n");

    /* Suspend both camera pipelines */
    CMW_CAMERA_Suspend(DCMIPP_PIPE1);
    CMW_CAMERA_Suspend(DCMIPP_PIPE2);

    UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

    BSP_LCD_DisplayOff(0);   // LTDC off
    LCD_Backlight_Off();     // Backlight off
}

static void exit_sleep(void)
{
    if (!g_sleep_active)
        return;

    g_sleep_active = false;
    printf("[SLEEP] Exiting sleep mode...\r\n");

    BSP_LCD_DisplayOn(0);    // LTDC on
    LCD_Backlight_On();      // Backlight on
    UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

    /* Resume camera pipelines */
    CMW_CAMERA_Resume(DCMIPP_PIPE1);
    CMW_CAMERA_Resume(DCMIPP_PIPE2);

    HAL_Delay(600); // allow ISP to settle

    /* --- Re-mount SD card after wake --- */
    extern bool USB_SD_Mount(void);
    extern bool USB_SD_EnsureMounted(void);

    USB_SD_Mount();
    if (!USB_SD_EnsureMounted())
        printf("[SLEEP][WARN] SD not mounted after wake!\r\n");
    else
        printf("[SLEEP] SD card re-mounted successfully\r\n");

    /* Update timers */
    g_last_face_time = HAL_GetTick();
    g_wake_time = g_last_face_time;
    touch_detected_since_last_sleep = false;
}

/* ================================================================
 * Optional delayed counter enable (used after Start pressed)
 * ================================================================ */
void APP_SleepMode_EnableCounterDelayed(uint32_t delay_ms)
{
    printf("[SLEEP] Counter will enable after %lu ms\r\n", delay_ms);
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
    g_last_face_time = HAL_GetTick();
    g_sleep_counter_enabled = true;
    printf("[SLEEP] Counter ENABLED (delayed)\r\n");
}
