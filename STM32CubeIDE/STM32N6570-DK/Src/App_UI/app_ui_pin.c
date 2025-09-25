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

/* --- Button geometry (centered keypad style) --- */
#define BTN_X   (800/2 - 100)   // center horizontally
#define BTN_Y   (480/2 - 30)    // center vertically
#define BTN_W   200
#define BTN_H   60

/* Reuse same background address as Start screen */
#define BG_ADDR 0x77E00000UL

static void UI_DrawBackground(void)
{
    /* Draw RGB565 raw image from NOR (same as start screen) */
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)BG_ADDR);
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
}

/* ===== Public API ===== */

void UI_PinScreen_Show(void)
{
    UTIL_LCD_SetLayer(0);
    BSP_LCD_DisplayOn(0);

    UI_DrawBackground();
    UI_DrawButton();

    printf("[UI] PIN screen shown\n");
}

void UI_PinScreen_WaitForOK(void)
{
    TS_State_t ts_state;
    printf("[UI] Waiting for OK button touch...\r\n");

    while (1) {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE) {
            if (ts_state.TouchDetected) {
                printf("[TS] Touch at x=%lu, y=%lu\r\n",
                       ts_state.TouchX, ts_state.TouchY);

                if (ts_state.TouchX >= BTN_X &&
                    ts_state.TouchX <= BTN_X + BTN_W &&
                    ts_state.TouchY >= BTN_Y &&
                    ts_state.TouchY <= BTN_Y + BTN_H) {

                    printf("[UI] OK button pressed!\r\n");
                    break;
                }
            }
        }
        HAL_Delay(50);
    }
}
