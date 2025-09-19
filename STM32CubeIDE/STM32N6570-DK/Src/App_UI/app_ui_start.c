#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include <stdio.h>

/* --- Button geometry --- */
#define BTN_X   280
#define BTN_Y   200
#define BTN_W   240
#define BTN_H   60

static void UI_DrawBackground(void)
{
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_GRAY);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_GRAY);
}

static void UI_DrawButton(void)
{
    /* Button background */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_FillRect(BTN_X, BTN_Y, BTN_W, BTN_H, UTIL_LCD_COLOR_BLUE);

    /* Button border */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DrawRect(BTN_X, BTN_Y, BTN_W, BTN_H, UTIL_LCD_COLOR_WHITE);
}

void UI_StartScreen_Show(void)
{
    UTIL_LCD_SetLayer(0);
    BSP_LCD_DisplayOn(0);

    UI_DrawBackground();
    UI_DrawButton();
}


void UI_WaitForButton(void)
{
    TS_State_t ts_state;
    printf("[UI] Waiting for Start button touch...\r\n");

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
                    break;
                }
            }
        }
        HAL_Delay(50);
    }
}

