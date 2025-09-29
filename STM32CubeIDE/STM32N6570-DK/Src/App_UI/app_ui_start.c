#include "app_ui_start.h"
#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"
#include <stdio.h>
#include "app_ui_admin.h"

/* --- Button geometry (bottom-right) --- */
#define BTN_X   (800 - 240 - 20)  // X position = 540
#define BTN_Y   (480 - 60 - 20)   // Y position = 400
#define BTN_W   240
#define BTN_H   60

/* --- NEW: Second button geometry (left of first one) --- */
#define BTN2_X  (BTN_X - BTN_W - 20)  // 20 px gap to the left
#define BTN2_Y  BTN_Y
#define BTN2_W  BTN_W
#define BTN2_H  BTN_H


static void UI_DrawBackground(void)
{
    /* Draw RGB565 raw image from NOR @0x72000000 */
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)0x77E00000);
}

static void UI_DrawButton(void)
{
    /* Button background */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(BTN_X, BTN_Y, BTN_W, BTN_H, UTIL_LCD_COLOR_WHITE);

    /* Button border */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(BTN_X, BTN_Y, BTN_W, BTN_H, UTIL_LCD_COLOR_BLACK);
}

/* --- NEW: Second button draw (blank, no text) --- */
static void UI_DrawButton2(void)
{
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(BTN2_X, BTN2_Y, BTN2_W, BTN2_H, UTIL_LCD_COLOR_WHITE);

    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(BTN2_X, BTN2_Y, BTN2_W, BTN2_H, UTIL_LCD_COLOR_BLACK);
}

void UI_StartScreen_Show(void)
{
    UTIL_LCD_SetLayer(0);
    BSP_LCD_DisplayOn(0);

    UI_DrawBackground();
    UI_DrawButton();
    UI_DrawButton2();   // draw new button
}

UI_ButtonResult_t UI_WaitForButton(void)
{
    TS_State_t ts_state;
    printf("[UI] Waiting for Start/Admin button touch...\r\n");

    while (1) {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE) {
            if (ts_state.TouchDetected) {
                printf("[TS] Touch at x=%lu, y=%lu\r\n",
                       ts_state.TouchX, ts_state.TouchY);

                if (ts_state.TouchX >= BTN_X &&
                    ts_state.TouchX <= BTN_X + BTN_W &&
                    ts_state.TouchY >= BTN_Y &&
                    ts_state.TouchY <= BTN_Y + BTN_H) {

                    printf("[UI] Start button pressed!\r\n");
                    return UI_BTN_START;
                }

                if (ts_state.TouchX >= BTN2_X &&
                    ts_state.TouchX <= BTN2_X + BTN2_W &&
                    ts_state.TouchY >= BTN2_Y &&
                    ts_state.TouchY <= BTN2_Y + BTN2_H) {

                    printf("[UI] Admin button pressed!\r\n");

                    UI_AdminScreen_Show();
                    UI_AdminResult_t result = UI_AdminScreen_WaitForTouch();

                    if (result == UI_ADMIN_BACK) {
                        // Go back to start screen
                        UI_StartScreen_Show();
                        continue; // keep waiting
                    } else if (result == UI_ADMIN_OK) {
                        printf("[UI] Admin Confirmed!\r\n");
                        return UI_BTN_ADMIN;
                    }
                }
            }
        }
        HAL_Delay(50);
    }
}
