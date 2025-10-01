#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"
#include <stdio.h>
#include "app_ui_start.h"

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
    UTIL_LCD_SetLayer(0);
    BSP_LCD_DisplayOn(0);

    UI_DrawBackground();
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

            if (tx >= BTN_START_X && tx <= BTN_START_X + BTN_START_W &&
                ty >= BTN_START_Y && ty <= BTN_START_Y + BTN_START_H) {
                printf("[UI] Start pressed!\r\n");
                return UI_BTN_START;
            }
            if (tx >= BTN_ADMIN_X && tx <= BTN_ADMIN_X + BTN_ADMIN_W &&
                ty >= BTN_ADMIN_Y && ty <= BTN_ADMIN_Y + BTN_ADMIN_H) {
                printf("[UI] Admin pressed!\r\n");
                return UI_BTN_ADMIN;
            }
        }
        HAL_Delay(50);
    }
}
