#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"
#include <stdio.h>
#include <stdbool.h>
#include "app_ui_start.h"
#include "app_ui_pin.h"
#include "app_sleepmode.h"
#include <string.h>

#ifndef UTIL_LCD_COLOR_TRANSPARENT
#define UTIL_LCD_COLOR_TRANSPARENT 0x0000u
#endif

#define BTN_START_X   (800 - 240 - 20)
#define BTN_START_Y   (480 - 60 - 20)
#define BTN_START_W   240
#define BTN_START_H   60

#define BTN_ADMIN_X   (BTN_START_X - BTN_START_W - 20)
#define BTN_ADMIN_Y   BTN_START_Y
#define BTN_ADMIN_W   BTN_START_W
#define BTN_ADMIN_H   BTN_START_H

static void UI_DrawBackground(void)
{
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)0x77E00000);
}

static void UI_DrawButton(int x, int y, int w, int h,
                          uint32_t fill, const char *label)
{
    UTIL_LCD_SetTextColor(fill);
    UTIL_LCD_FillRect(x, y, w, h, fill);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DrawRect(x, y, w, h, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetBackColor(fill);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetFont(&Font24);   // Bold
    uint32_t char_width  = UTIL_LCD_GetFont()->Width;
    uint32_t text_width  = strlen(label) * char_width;
    uint32_t text_height = UTIL_LCD_GetFont()->Height;
    int text_x = x + (w - text_width) / 2;
    int text_y = y + (h - text_height) / 2;

    UTIL_LCD_DisplayStringAt(text_x, text_y, (uint8_t*)label, LEFT_MODE);
}

void UI_StartScreen_Show(void)
{
    APP_SleepMode_ResetState();
    UTIL_LCD_SetLayer(1);
    BSP_LCD_DisplayOn(0);
    BSP_LCD_SetLayerVisible(0, 1, ENABLE);
    BSP_LCD_Reload(0, BSP_LCD_RELOAD_IMMEDIATE);

    UTIL_LCD_Clear(UTIL_LCD_COLOR_TRANSPARENT);
    UI_DrawBackground();
    UI_DrawButton(BTN_START_X, BTN_START_Y, BTN_START_W, BTN_START_H,
                  UTIL_LCD_COLOR_BLACK, "START");
    UI_DrawButton(BTN_ADMIN_X, BTN_ADMIN_Y, BTN_ADMIN_W, BTN_ADMIN_H,
                  UTIL_LCD_COLOR_BLACK, "ADMIN");
}


UI_ButtonResult UI_WaitForButton(void)
{
    TS_State_t ts_state;
    printf("[UI] Waiting for Start/Admin button...\r\n");

    while (1) {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE && ts_state.TouchDetected) {

            uint16_t tx = ts_state.TouchX;
            uint16_t ty = ts_state.TouchY;

            /* -------------------- START -------------------- */
            if (tx >= BTN_START_X && tx <= BTN_START_X + BTN_START_W &&
                ty >= BTN_START_Y && ty <= BTN_START_Y + BTN_START_H)
            {
                printf("[UI] Start pressed!\r\n");

                APP_SleepMode_ResetState();  // make sure sleep logic clean

                BSP_LCD_SetLayerVisible(0, 1, DISABLE);
                UTIL_LCD_Clear(UTIL_LCD_COLOR_TRANSPARENT);
                UTIL_LCD_SetLayer(1);
                BSP_LCD_Reload(0, BSP_LCD_RELOAD_IMMEDIATE);
                HAL_Delay(100);

                extern void Pipeline_TriggerStart(void);
                Pipeline_TriggerStart();

                return UI_BTN_START;
            }

            /* -------------------- ADMIN -------------------- */
            if (tx >= BTN_ADMIN_X && tx <= BTN_ADMIN_X + BTN_ADMIN_W &&
                ty >= BTN_ADMIN_Y && ty <= BTN_ADMIN_Y + BTN_ADMIN_H)
            {
                printf("[UI] Admin pressed!\r\n");

                APP_SleepMode_ResetState();  // clean state before admin flow

                BSP_LCD_SetLayerVisible(0, 1, DISABLE);
                BSP_LCD_Reload(0, BSP_LCD_RELOAD_IMMEDIATE);
                UTIL_LCD_Clear(UTIL_LCD_COLOR_TRANSPARENT);
                HAL_Delay(100);

                UI_PinScreen_Show();
                UI_PinScreen_WaitForOK();

                BSP_LCD_SetLayerVisible(0, 1, ENABLE);
                BSP_LCD_Reload(0, BSP_LCD_RELOAD_IMMEDIATE);

                printf("[UI] PIN screen finished, returning to start menu...\r\n");
                return UI_BTN_ADMIN;
            }
        }
        HAL_Delay(50);
    }
}

