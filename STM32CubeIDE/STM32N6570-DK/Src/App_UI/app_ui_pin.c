/**
 ******************************************************************************
 * @file    app_ui_pin.c
 * @brief   PIN entry / Auth screen
 ******************************************************************************
 */

#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"
#include <stdio.h>
#include <string.h>
#include "app_sleepmode.h"   // <-- added include for sleep override control

/* --- Button geometry (centered) --- */
#define BTN_X   (800/2 - 100)   // center horizontally
#define BTN_Y   (480/2 - 30)    // center vertically
#define BTN_W   200
#define BTN_H   60

/* Grey background color */
#define PIN_BG_COLOR  UTIL_LCD_COLOR_GRAY

/* ===== Internal helpers ===== */
static void UI_DrawBackground(void)
{
    /* Fill whole LCD with grey instead of bitmap */
    UTIL_LCD_SetBackColor(PIN_BG_COLOR);
    UTIL_LCD_SetTextColor(PIN_BG_COLOR);
    UTIL_LCD_FillRect(0, 0, 800, 480, PIN_BG_COLOR);
    printf("[UI] PIN background drawn (grey)\r\n");
}

static void UI_DrawButton(void)
{
    /* Simple “OK” button */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(BTN_X, BTN_Y, BTN_W, BTN_H, UTIL_LCD_COLOR_WHITE);

    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(BTN_X, BTN_Y, BTN_W, BTN_H, UTIL_LCD_COLOR_BLACK);

    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DisplayStringAt(BTN_X + 50, BTN_Y + 20,
                             (uint8_t *)"OK", LEFT_MODE);

    printf("[UI] OK button drawn @ (%d,%d) size=%dx%d\r\n",
           BTN_X, BTN_Y, BTN_W, BTN_H);
}

/* ===== Public API ===== */

void UI_PinScreen_Show(void)
{
    APP_SleepMode_Disable();   // <-- disable sleep while PIN screen is active

    /* Put this screen on TOP foreground layer */
    UTIL_LCD_SetLayer(1);
    BSP_LCD_DisplayOn(0);

    UI_DrawBackground();
    UI_DrawButton();

    printf("[UI] PIN screen shown (layer=1)\r\n");
}

void UI_PinScreen_WaitForOK(void)
{
    TS_State_t ts_state;
    printf("[UI] Waiting for OK button touch...\r\n");

    while (1) {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE) {
            if (ts_state.TouchDetected) {
                uint16_t tx = ts_state.TouchX;
                uint16_t ty = ts_state.TouchY;
                printf("[TS] Touch at x=%u, y=%u\r\n", tx, ty);

                if (tx >= BTN_X && tx <= BTN_X + BTN_W &&
                    ty >= BTN_Y && ty <= BTN_Y + BTN_H) {

                    printf("[UI] OK button pressed!\r\n");
                    APP_SleepMode_Enable();   // <-- re-enable sleep after PIN entry
                    return;
                }
            }
        }
        HAL_Delay(50);
    }
}
