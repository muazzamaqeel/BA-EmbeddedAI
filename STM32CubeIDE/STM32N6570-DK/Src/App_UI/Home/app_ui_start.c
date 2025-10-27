#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"
#include <stdio.h>
#include <stdbool.h>             // ✅ Add this line
#include "app_ui_start.h"
#include "app_ui_pin.h"          // <-- needed for PIN screen functions
#include "app_sleepmode.h"       // ✅ Add this line

#ifndef UTIL_LCD_COLOR_TRANSPARENT
#define UTIL_LCD_COLOR_TRANSPARENT 0x0000u  /* ARGB4444 transparent */
#endif

/* --- Start button (bottom-right) --- */
#define BTN_START_X   (800 - 240 - 20)  // 540
#define BTN_START_Y   (480 - 60 - 20)   // 400
#define BTN_START_W   240
#define BTN_START_H   60

/* --- Admin button (left of Start, same size) --- */
#define BTN_ADMIN_X   (BTN_START_X - BTN_START_W - 20)  // 280
#define BTN_ADMIN_Y   BTN_START_Y
#define BTN_ADMIN_W   BTN_START_W
#define BTN_ADMIN_H   BTN_START_H

static void UI_DrawBackground(void)
{
    /* Draw background bitmap from NOR */
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)0x77E00000);
}

static void UI_DrawButton(int x, int y, int w, int h,
                          uint32_t fill, const char *label)
{
    /* Fill */
    UTIL_LCD_SetTextColor(fill);
    UTIL_LCD_FillRect(x, y, w, h, fill);

    /* Border */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(x, y, w, h, UTIL_LCD_COLOR_BLACK);

    /* Label */
    UTIL_LCD_SetBackColor(fill);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetFont(&Font20);
    UTIL_LCD_DisplayStringAt(x + 20, y + 20, (uint8_t*)label, LEFT_MODE);
}

void UI_StartScreen_Show(void)
{
    UTIL_LCD_SetLayer(1);              // Draw on UI overlay
    BSP_LCD_DisplayOn(0);

    UTIL_LCD_Clear(UTIL_LCD_COLOR_TRANSPARENT);
    UI_DrawBackground();               // Background bitmap (still visible through transparency)
    UI_DrawButton(BTN_START_X, BTN_START_Y, BTN_START_W, BTN_START_H,
                  UTIL_LCD_COLOR_WHITE, "Start");
    UI_DrawButton(BTN_ADMIN_X, BTN_ADMIN_Y, BTN_ADMIN_W, BTN_ADMIN_H,
                  UTIL_LCD_COLOR_LIGHTGRAY, "Admin");
}


UI_ButtonResult UI_WaitForButton(void)
{
    TS_State_t ts_state;
    printf("[UI] Waiting for Start/Admin button...\r\n");

    while (1) {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE && ts_state.TouchDetected) {
            uint16_t tx = ts_state.TouchX;
            uint16_t ty = ts_state.TouchY;

            /* -------------------- START button -------------------- */
            if (tx >= BTN_START_X && tx <= BTN_START_X + BTN_START_W &&
                ty >= BTN_START_Y && ty <= BTN_START_Y + BTN_START_H)
            {
                printf("[UI] Start pressed!\r\n");

                /* Hide the entire Start screen before starting the pipeline */
                BSP_LCD_SetLayerVisible(0, 1, DISABLE);               // (Instance=0, Layer=1)
                UTIL_LCD_Clear(UTIL_LCD_COLOR_TRANSPARENT);           // clear residual pixels
                UTIL_LCD_SetLayer(1);                                 // future UI still on layer 1
                BSP_LCD_Reload(0, BSP_LCD_RELOAD_IMMEDIATE);          // apply visibility change immediately
                HAL_Delay(100);                                       // ensure clean layer transition

                /* Signal the pipeline to start */
                extern void Pipeline_TriggerStart(void);
                Pipeline_TriggerStart();

                /* Enable sleep counter after start */
                //APP_SleepMode_EnableCounter(true);

                return UI_BTN_START;
            }

            /* -------------------- ADMIN button -------------------- */
            if (tx >= BTN_ADMIN_X && tx <= BTN_ADMIN_X + BTN_ADMIN_W &&
                ty >= BTN_ADMIN_Y && ty <= BTN_ADMIN_Y + BTN_ADMIN_H)
            {
                printf("[UI] Admin pressed!\r\n");

                UI_PinScreen_Show();
                UI_PinScreen_WaitForOK();

                printf("[UI] PIN screen finished, returning to start menu...\r\n");
                return UI_BTN_ADMIN;
            }
        }
        HAL_Delay(50);
    }
}

