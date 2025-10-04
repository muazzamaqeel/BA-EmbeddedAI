/**
 ******************************************************************************
 * @file    app_ui_pin.c
 * @brief   PIN entry / Auth screen with hidden keypad & no error text
 ******************************************************************************
 */

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

/* Center keypad slightly higher on screen */
#define KEYPAD_ORIGIN_X  ((800/2 - (3*KEY_W + 2*KEY_SP)/2))
#define KEYPAD_ORIGIN_Y  100

/* Expected PIN */
#define EXPECTED_PIN  "1234"

/* Buffer */
static char pin_buffer[8];
static int pin_len = 0;

/* ===== Internal helpers ===== */
static void UI_DrawBackground(void)
{
    /* Background image */
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)0x77AE0000);
    printf("[UI] PIN background drawn (image @0x77AE0000)\r\n");
}

static void UI_DrawPinBuffer(void)
{
    char disp[16];
    memset(disp, '*', pin_len);
    disp[pin_len] = '\0';

    /* Show entered PIN as stars */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetFont(&Font20);
    UTIL_LCD_DisplayStringAt(220, 35, (uint8_t*)disp, LEFT_MODE);

    printf("[UI] PIN buffer updated: '%s'\r\n", disp);
}

/* ===== Public API ===== */
void UI_PinScreen_Show(void)
{
    APP_SleepMode_Disable();   // disable sleep while PIN screen is active

    UTIL_LCD_SetLayer(0);
    BSP_LCD_DisplayOn(0);

    UI_DrawBackground();
    UI_DrawPinBuffer();

    printf("[UI] PIN screen shown (hidden keypad, only stars visible)\r\n");
}

void UI_PinScreen_WaitForOK(void)
{
    TS_State_t ts_state;
    bool touch_active = false;   // track if finger is currently down

    printf("[UI] Waiting for keypad input (hidden)...\r\n");

    while (1) {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE) {
            if (ts_state.TouchDetected && !touch_active) {
                // --- First press detected ---
                touch_active = true;   // lock until finger lifted
                uint16_t tx = ts_state.TouchX;
                uint16_t ty = ts_state.TouchY;

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

                            const char *keys[4][3] = {
                                {"1","2","3"},
                                {"4","5","6"},
                                {"7","8","9"},
                                {"CLR","0","OK"}
                            };
                            const char *label = keys[r][c];
                            printf("[TS] Hidden key pressed: %s\r\n", label);

                            if (strcmp(label,"CLR")==0) {
                                pin_len = 0;
                                memset(pin_buffer,0,sizeof(pin_buffer));
                                UI_DrawPinBuffer();
                            }
                            else if (strcmp(label,"OK")==0) {
                                pin_buffer[pin_len] = '\0';
                                if (strcmp(pin_buffer, EXPECTED_PIN)==0) {
                                    printf("[UI] Correct PIN entered!\r\n");
                                    APP_SleepMode_Enable();
                                    return;
                                }
                                else {
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
            }
            else if (!ts_state.TouchDetected) {
                // --- Finger lifted → reset state ---
                touch_active = false;
            }
        }
        HAL_Delay(30); // smaller polling delay is fine
    }
}

