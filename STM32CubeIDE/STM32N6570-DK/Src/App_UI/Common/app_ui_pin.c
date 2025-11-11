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
#include "app_sleepmode.h"   // for sleep override control

/* --- Keypad layout (invisible, touch only) --- */
#define KEY_W   90
#define KEY_H   65
#define KEY_SP  20
#define KEYPAD_ORIGIN_X  ((800/2 - (3*KEY_W + 2*KEY_SP)/2))
#define KEYPAD_ORIGIN_Y  100

/* --- PIN display box (white rectangle above keypad) --- */
#define PIN_BOX_W   440
#define PIN_BOX_H    50
#define PIN_BOX_X   ((800 - PIN_BOX_W) / 2)
#define PIN_BOX_Y    20

/* Expected PIN */
#define EXPECTED_PIN  g_admin_pin

/* Buffer */
static char pin_buffer[8];
static int  pin_len = 0;

/* ===== Internal helpers ===== */
static void UI_DrawBackground(void)
{
    /* Background image */
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)0x77AE0000);

    /* Draw PIN input box */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(PIN_BOX_X, PIN_BOX_Y, PIN_BOX_W, PIN_BOX_H, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(PIN_BOX_X, PIN_BOX_Y, PIN_BOX_W, PIN_BOX_H, UTIL_LCD_COLOR_BLACK);

    printf("[UI] PIN background drawn (image @0x77AE0000)\r\n");
}

static void UI_DrawPinBuffer(void)
{
    char disp[16];
    memset(disp, '*', pin_len);
    disp[pin_len] = '\0';

    /* Clear inside box */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(PIN_BOX_X + 2, PIN_BOX_Y + 2, PIN_BOX_W - 4, PIN_BOX_H - 4, UTIL_LCD_COLOR_WHITE);

    /* Draw stars centered in box */
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

/* ===== Public API ===== */
void UI_PinScreen_Show(void)
{
    APP_SleepMode_Disable();
    UTIL_LCD_SetLayer(0);
    BSP_LCD_DisplayOn(0);

    UI_DrawBackground();
    UI_DrawPinBuffer();

    printf("[UI] PIN screen shown (hidden keypad, masked stars in box)\r\n");
}

void UI_PinScreen_WaitForOK(void)
{
    TS_State_t ts_state;
    bool touch_active = false;

    printf("[UI] Waiting for keypad input (hidden)...\r\n");

    while (1) {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE) {
            if (ts_state.TouchDetected && !touch_active) {
                touch_active = true;
                uint16_t tx = ts_state.TouchX;
                uint16_t ty = ts_state.TouchY;

                const char *keys[4][3] = {
                    {"1","2","3"},
                    {"4","5","6"},
                    {"7","8","9"},
                    {"CLR","0","OK"}
                };

                for (int r=0; r<4; r++) {
                    for (int c=0; c<3; c++) {
                        int x = KEYPAD_ORIGIN_X + c * (KEY_W + KEY_SP);
                        int y = KEYPAD_ORIGIN_Y + r * (KEY_H + KEY_SP);
                        if (r == 1) y += 10;
                        if (r == 2) y += 25;
                        if (r == 3) y += 40;
                        if (c == 2) x -= 15;

                        if (tx >= x && tx <= x+KEY_W &&
                            ty >= y && ty <= y+KEY_H) {

                            const char *label = keys[r][c];
                            printf("[TS] Hidden key pressed: %s\r\n", label);

                            if (strcmp(label,"CLR")==0) {
                                pin_len = 0;
                                memset(pin_buffer,0,sizeof(pin_buffer));
                                UI_DrawPinBuffer();
                            }
                            else if (strcmp(label,"OK")==0) {
                                pin_buffer[pin_len] = '\0';
                                if (strcmp(pin_buffer, g_admin_pin)==0) {
                                    printf("[UI] Correct PIN entered!\r\n");
                                    APP_SleepMode_Enable();
                                    return;
                                } else {
                                    printf("[UI] Wrong PIN (hidden, no message)\r\n");
                                    pin_len = 0;
                                    memset(pin_buffer,0,sizeof(pin_buffer));
                                    UI_DrawPinBuffer();
                                }
                            }
                            else {
                                if (pin_len < (int)(sizeof(pin_buffer)-1)) {
                                    pin_buffer[pin_len++] = label[0];
                                    UI_DrawPinBuffer();
                                }
                            }
                        }
                    }
                }
            } else if (!ts_state.TouchDetected) {
                touch_active = false;
            }
        }
        HAL_Delay(30);
    }
}
