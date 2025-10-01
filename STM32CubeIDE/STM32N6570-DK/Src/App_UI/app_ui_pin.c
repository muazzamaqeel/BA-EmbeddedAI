/**
 ******************************************************************************
 * @file    app_ui_pin.c
 * @brief   PIN entry / Auth screen with keypad (centered, debounced)
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

/* --- Keypad layout (balanced size) --- */
#define KEY_W   90
#define KEY_H   65
#define KEY_SP  20

/* Center the keypad slightly higher on screen */
#define KEYPAD_ORIGIN_X  ((800/2 - (3*KEY_W + 2*KEY_SP)/2))
#define KEYPAD_ORIGIN_Y  140   // was 160, moved up a bit

/* Expected PIN */
#define EXPECTED_PIN  "1234"

/* Buffer */
static char pin_buffer[8];
static int pin_len = 0;

/* Grey background color */
#define PIN_BG_COLOR  UTIL_LCD_COLOR_LIGHTGRAY

/* ===== Internal helpers ===== */
static void UI_DrawBackground(void)
{
    /* Draw RGB565 raw image from NOR at 0x77AE0000 */
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)0x77AE0000);

    printf("[UI] PIN background drawn (image @0x77AE0000)\r\n");
}


static void UI_DrawKey(int row, int col, const char *label,
                       uint32_t fillColor, uint32_t textColor)
{
    int x = KEYPAD_ORIGIN_X + col * (KEY_W + KEY_SP);
    int y = KEYPAD_ORIGIN_Y + row * (KEY_H + KEY_SP);

    UTIL_LCD_SetTextColor(fillColor);
    UTIL_LCD_FillRect(x, y, KEY_W, KEY_H, fillColor);

    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(x, y, KEY_W, KEY_H, UTIL_LCD_COLOR_BLACK);

    UTIL_LCD_SetBackColor(fillColor);
    UTIL_LCD_SetTextColor(textColor);
    UTIL_LCD_SetFont(&Font20);
    UTIL_LCD_DisplayStringAt(x + KEY_W/2 - 10, y + KEY_H/2 - 10,
                             (uint8_t*)label, LEFT_MODE);

    printf("[UI] Key drawn '%s' @ (%d,%d)\r\n", label, x, y);
}

static void UI_DrawKeypad(void)
{
    const char *keys[4][3] = {
        {"1","2","3"},
        {"4","5","6"},
        {"7","8","9"},
        {"CLR","0","OK"}
    };

    for (int r=0; r<4; r++) {
        for (int c=0; c<3; c++) {
            const char *label = keys[r][c];
            uint32_t fill = UTIL_LCD_COLOR_WHITE;
            uint32_t text = UTIL_LCD_COLOR_BLACK;

            if (strcmp(label,"CLR")==0) {
                fill = UTIL_LCD_COLOR_RED;
                text = UTIL_LCD_COLOR_WHITE;
            } else if (strcmp(label,"OK")==0) {
                fill = UTIL_LCD_COLOR_GREEN;
                text = UTIL_LCD_COLOR_WHITE;
            }

            UI_DrawKey(r, c, label, fill, text);
        }
    }
}

static void UI_DrawPinBuffer(void)
{
    char disp[16];
    memset(disp, '*', pin_len);
    disp[pin_len] = '\0';

    // Clear area
    UTIL_LCD_SetBackColor(PIN_BG_COLOR);
    UTIL_LCD_SetTextColor(PIN_BG_COLOR);
    UTIL_LCD_FillRect(200, 60, 400, 50, PIN_BG_COLOR); // moved up (was 80)

    // White box
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(200, 60, 400, 50, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(200, 60, 400, 50, UTIL_LCD_COLOR_BLACK);

    // Masked PIN
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetFont(&Font20);
    UTIL_LCD_DisplayStringAt(220, 75, (uint8_t*)disp, LEFT_MODE);

    printf("[UI] PIN buffer updated: '%s'\r\n", disp);
}

/* ===== Public API ===== */
void UI_PinScreen_Show(void)
{
    APP_SleepMode_Disable();   // disable sleep while PIN screen is active

    UTIL_LCD_SetLayer(1);
    BSP_LCD_DisplayOn(0);

    UI_DrawBackground();
    UI_DrawKeypad();
    UI_DrawPinBuffer();

    printf("[UI] PIN screen shown (layer=1)\r\n");
}

void UI_PinScreen_WaitForOK(void)
{
    TS_State_t ts_state;
    printf("[UI] Waiting for keypad input...\r\n");

    while (1) {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE && ts_state.TouchDetected) {
            uint16_t tx = ts_state.TouchX;
            uint16_t ty = ts_state.TouchY;

            for (int r=0; r<4; r++) {
                for (int c=0; c<3; c++) {
                    int x = KEYPAD_ORIGIN_X + c * (KEY_W + KEY_SP);
                    int y = KEYPAD_ORIGIN_Y + r * (KEY_H + KEY_SP);

                    if (tx >= x && tx <= x+KEY_W &&
                        ty >= y && ty <= y+KEY_H) {

                        const char *keys[4][3] = {
                            {"1","2","3"},
                            {"4","5","6"},
                            {"7","8","9"},
                            {"CLR","0","OK"}
                        };
                        const char *label = keys[r][c];
                        printf("[TS] Key pressed: %s\r\n", label);

                        if (strcmp(label,"CLR")==0) {
                            pin_len = 0;
                            memset(pin_buffer,0,sizeof(pin_buffer));
                            UI_DrawPinBuffer();
                        }
                        else if (strcmp(label,"OK")==0) {
                            pin_buffer[pin_len] = '\0';
                            if (strcmp(pin_buffer, EXPECTED_PIN)==0) {
                                printf("[UI] Correct PIN entered!\r\n");
                                APP_SleepMode_Enable(); // re-enable sleep
                                return;
                            } else {
                                printf("[UI] Wrong PIN!\r\n");
                                UTIL_LCD_SetBackColor(PIN_BG_COLOR);
                                UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
                                UTIL_LCD_DisplayStringAt(220, 120,
                                    (uint8_t*)"Wrong PIN", LEFT_MODE);

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

                        /* Debounce */
                        do {
                            BSP_TS_GetState(0, &ts_state);
                            HAL_Delay(30);
                        } while (ts_state.TouchDetected);

                        HAL_Delay(150);
                    }
                }
            }
        }
        HAL_Delay(50);
    }
}
