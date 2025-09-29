/**
 ******************************************************************************
 * @file    app_ui_admin.c
 * @brief   Admin PIN entry screen
 ******************************************************************************
 */

#include "app_ui_admin.h"
#include <stdio.h>
#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"

/* --- Geometry --- */
#define BTN_W   200
#define BTN_H   60
#define BTN_Y   (400)

#define BACK_BTN_X    (100)                // left side
#define CONFIRM_BTN_X (800 - BTN_W - 100)  // right side

/* --- Background address in NOR flash --- */
#define ADMIN_SCREEN_ADDR   0x77AE0000  // where 1.bin was flashed

/* --- Helpers --- */
static void UI_Admin_DrawBackground(void)
{
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)ADMIN_SCREEN_ADDR);
}

static void UI_Admin_DrawButton(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const char *label)
{
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(x, y, w, h, UTIL_LCD_COLOR_WHITE);

    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(x, y, w, h, UTIL_LCD_COLOR_BLACK);

    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DisplayStringAt(x + 10, y + (h/2) - 8, (uint8_t*)label, LEFT_MODE);
}

/* --- Public functions --- */
void UI_AdminScreen_Show(void)
{
    UTIL_LCD_SetLayer(0);
    BSP_LCD_DisplayOn(0);

    UI_Admin_DrawBackground();
    UI_Admin_DrawButton(BACK_BTN_X, BTN_Y, BTN_W, BTN_H, "Back");
    UI_Admin_DrawButton(CONFIRM_BTN_X, BTN_Y, BTN_W, BTN_H, "Confirm");
}

UI_AdminResult_t UI_AdminScreen_WaitForTouch(void)
{
    TS_State_t ts_state;
    printf("[UI] Waiting for Admin button touch...\r\n");

    while (1) {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE) {
            if (ts_state.TouchDetected) {
                printf("[TS] Touch at x=%lu, y=%lu\r\n",
                       ts_state.TouchX, ts_state.TouchY);

                /* Back button */
                if (ts_state.TouchX >= BACK_BTN_X &&
                    ts_state.TouchX <= BACK_BTN_X + BTN_W &&
                    ts_state.TouchY >= BTN_Y &&
                    ts_state.TouchY <= BTN_Y + BTN_H) {
                    printf("[UI] Admin -> Back pressed\r\n");
                    return UI_ADMIN_BACK;
                }

                /* Confirm button */
                if (ts_state.TouchX >= CONFIRM_BTN_X &&
                    ts_state.TouchX <= CONFIRM_BTN_X + BTN_W &&
                    ts_state.TouchY >= BTN_Y &&
                    ts_state.TouchY <= BTN_Y + BTN_H) {
                    printf("[UI] Admin -> Confirm pressed\r\n");
                    return UI_ADMIN_OK;
                }
            }
        }
        HAL_Delay(50);
    }
}
