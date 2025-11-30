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




