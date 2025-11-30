/**
 ******************************************************************************
 * @file    app_ui_pin.c
 * @brief   PIN entry / Auth screen with hidden keypad & masked stars in box
 ******************************************************************************
 */
#include "admin_pin.h"
#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "app_sleepmode.h"
#include "app_ui_pin_controller.h"

#define KEY_W   90
#define KEY_H   65
#define KEY_SP  20
#define KEYPAD_ORIGIN_X  ((800/2 - (3*KEY_W + 2*KEY_SP)/2))
#define KEYPAD_ORIGIN_Y  100
#define PIN_BOX_W   440
#define PIN_BOX_H    50
#define PIN_BOX_X   ((800 - PIN_BOX_W) / 2)
#define PIN_BOX_Y    20
#define EXPECTED_PIN  g_admin_pin

static void UI_DrawBackground(void)
{
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)0x77AE0000);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(PIN_BOX_X, PIN_BOX_Y, PIN_BOX_W, PIN_BOX_H, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(PIN_BOX_X, PIN_BOX_Y, PIN_BOX_W, PIN_BOX_H, UTIL_LCD_COLOR_BLACK);

    printf("[UI] PIN background drawn (image @0x77AE0000)\r\n");
}

void UI_DrawPinBuffer(void)
{
    char disp[16];
    memset(disp, '*', pin_len);
    disp[pin_len] = '\0';
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(PIN_BOX_X + 2, PIN_BOX_Y + 2, PIN_BOX_W - 4, PIN_BOX_H - 4, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetFont(&Font24);
    int textWidth  = strlen(disp) * 16;
    int textHeight = 24;
    int textX = PIN_BOX_X + (PIN_BOX_W - textWidth) / 2;
    int textY = PIN_BOX_Y + (PIN_BOX_H - textHeight) / 2 + 2;

    UTIL_LCD_DisplayStringAt(textX, textY, (uint8_t*)disp, LEFT_MODE);
    printf("[UI] PIN buffer updated: '%s'\r\n", disp);
}

void UI_PinScreen_Show(void)
{
    APP_SleepMode_Disable();
    pin_len = 0;
    memset(pin_buffer, 0, sizeof(pin_buffer));

    UTIL_LCD_SetLayer(0);
    BSP_LCD_DisplayOn(0);

    UI_DrawBackground();
    UI_DrawPinBuffer();

    printf("[UI] PIN screen shown (hidden keypad, masked stars in box)\r\n");
}


void UI_PinScreen_WaitForOK(void)
{
	UI_PinScreen_WaitForOK_Controller();
}
