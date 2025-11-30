/**
 *******************************************************************************
 * @file    app_change_pin.c
 * @brief   Change PIN screen (hidden keypad, logs only, no on-screen text)
 *******************************************************************************
 */

#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "app_change_pin.h"
#include "app_sleepmode.h"
#include "admin_pin.h"



void CP_UI_DrawBackground(void)
{
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)0x77AE0000);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(PIN_BOX_X, PIN_BOX_Y, PIN_BOX_W, PIN_BOX_H, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(PIN_BOX_X, PIN_BOX_Y, PIN_BOX_W, PIN_BOX_H, UTIL_LCD_COLOR_BLACK);

    printf("[UI-CP] Change PIN screen background drawn\r\n");
}


void CP_UI_LogPinBuffer(void)
{
    char disp[16];
    memset(disp, '*', cp_pin_len);
    disp[cp_pin_len] = '\0';
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(PIN_BOX_X + 2, PIN_BOX_Y + 2, PIN_BOX_W - 4, PIN_BOX_H - 4, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetFont(&Font24);
    int textWidth  = strlen(disp) * 14;
    int textHeight = 24;
    int textX = PIN_BOX_X + (PIN_BOX_W - textWidth) / 2;
    int textY = PIN_BOX_Y + (PIN_BOX_H - textHeight) / 2;

    UTIL_LCD_DisplayStringAt(textX, textY, (uint8_t*)disp, LEFT_MODE);

    printf("[UI-CP] PIN buffer updated: '%s'\r\n", disp);
}


CP_Result UI_ChangePinScreen_Show(void)
{
    return UI_ChangePinScreen_Show_Controller();
}

