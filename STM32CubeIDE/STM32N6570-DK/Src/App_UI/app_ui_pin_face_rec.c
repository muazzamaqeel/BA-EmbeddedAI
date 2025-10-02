//app_ui_pin_face_rec.c
/**
 ******************************************************************************
 * @file    app_ui_pin_face_rec.c
 * @brief   PIN entry / Auth screen (FaceRec variant)
 *          - Visible keypad
 *          - Debounced touch
 *          - Shows "Wrong PIN" text
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
#include "app_sleepmode.h"

/* --- Keypad layout --- */
#define FR_KEY_W   90
#define FR_KEY_H   65
#define FR_KEY_SP  20

#define FR_KEYPAD_ORIGIN_X  ((800/2 - (3*FR_KEY_W + 2*FR_KEY_SP)/2))
#define FR_KEYPAD_ORIGIN_Y  140

/* Expected PIN */
#define FR_EXPECTED_PIN  "1234"

/* Buffer */
static char fr_pin_buffer[8];
static int  fr_pin_len = 0;

/* Colors */
#define FR_PIN_BG_COLOR  UTIL_LCD_COLOR_LIGHTGRAY

/* ===== Internal helpers ===== */
static void FR_UI_DrawBackground(void)
{
    UTIL_LCD_SetBackColor(FR_PIN_BG_COLOR);
    UTIL_LCD_SetTextColor(FR_PIN_BG_COLOR);
    UTIL_LCD_FillRect(0, 0, 800, 480, FR_PIN_BG_COLOR);
    printf("[UI-FR] PIN background drawn (grey)\r\n");
}

static void FR_UI_DrawKey(int row, int col, const char *label,
                          uint32_t fillColor, uint32_t textColor)
{
    int x = FR_KEYPAD_ORIGIN_X + col * (FR_KEY_W + FR_KEY_SP);
    int y = FR_KEYPAD_ORIGIN_Y + row * (FR_KEY_H + FR_KEY_SP);

    UTIL_LCD_SetTextColor(fillColor);
    UTIL_LCD_FillRect(x, y, FR_KEY_W, FR_KEY_H, fillColor);

    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(x, y, FR_KEY_W, FR_KEY_H, UTIL_LCD_COLOR_BLACK);

    UTIL_LCD_SetBackColor(fillColor);
    UTIL_LCD_SetTextColor(textColor);
    UTIL_LCD_SetFont(&Font20);
    UTIL_LCD_DisplayStringAt(x + FR_KEY_W/2 - 10, y + FR_KEY_H/2 - 10,
                             (uint8_t*)label, LEFT_MODE);

    printf("[UI-FR] Key drawn '%s' @ (%d,%d)\r\n", label, x, y);
}

static void FR_UI_DrawKeypad(void)
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

            FR_UI_DrawKey(r, c, label, fill, text);
        }
    }
}

static void FR_UI_DrawPinBuffer(void)
{
    char disp[16];
    memset(disp, '*', fr_pin_len);
    disp[fr_pin_len] = '\0';

    // Clear area
    UTIL_LCD_SetBackColor(FR_PIN_BG_COLOR);
    UTIL_LCD_SetTextColor(FR_PIN_BG_COLOR);
    UTIL_LCD_FillRect(200, 60, 400, 50, FR_PIN_BG_COLOR);

    // White box
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(200, 60, 400, 50, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(200, 60, 400, 50, UTIL_LCD_COLOR_BLACK);

    // Masked PIN
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetFont(&Font20);
    UTIL_LCD_DisplayStringAt(220, 75, (uint8_t*)disp, LEFT_MODE);

    printf("[UI-FR] PIN buffer updated: '%s'\r\n", disp);
}

/* ===== Public API (FaceRec variant) ===== */
void UI_FR_PinScreen_Show(void)
{
    APP_SleepMode_Disable();

    UTIL_LCD_SetLayer(1);
    BSP_LCD_DisplayOn(0);

    FR_UI_DrawBackground();
    FR_UI_DrawKeypad();
    FR_UI_DrawPinBuffer();

    printf("[UI-FR] PIN screen shown (layer=1)\r\n");
}

void UI_FR_PinScreen_WaitForOK(void)
{
    TS_State_t ts_state;
    printf("[UI-FR] Waiting for keypad input...\r\n");

    while (1) {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE && ts_state.TouchDetected) {
            uint16_t tx = ts_state.TouchX;
            uint16_t ty = ts_state.TouchY;

            for (int r=0; r<4; r++) {
                for (int c=0; c<3; c++) {
                    int x = FR_KEYPAD_ORIGIN_X + c * (FR_KEY_W + FR_KEY_SP);
                    int y = FR_KEYPAD_ORIGIN_Y + r * (FR_KEY_H + FR_KEY_SP);

                    if (tx >= x && tx <= x+FR_KEY_W &&
                        ty >= y && ty <= y+FR_KEY_H) {

                        const char *keys[4][3] = {
                            {"1","2","3"},
                            {"4","5","6"},
                            {"7","8","9"},
                            {"CLR","0","OK"}
                        };
                        const char *label = keys[r][c];
                        printf("[TS-FR] Key pressed: %s\r\n", label);

                        if (strcmp(label,"CLR")==0) {
                            fr_pin_len = 0;
                            memset(fr_pin_buffer,0,sizeof(fr_pin_buffer));
                            FR_UI_DrawPinBuffer();
                        }
                        else if (strcmp(label,"OK")==0) {
                            fr_pin_buffer[fr_pin_len] = '\0';
                            if (strcmp(fr_pin_buffer, FR_EXPECTED_PIN)==0) {
                                printf("[UI-FR] Correct PIN entered!\r\n");
                                APP_SleepMode_Enable();
                                return;
                            } else {
                                printf("[UI-FR] Wrong PIN!\r\n");
                                UTIL_LCD_SetBackColor(FR_PIN_BG_COLOR);
                                UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
                                UTIL_LCD_DisplayStringAt(220, 120,
                                    (uint8_t*)"Wrong PIN", LEFT_MODE);

                                fr_pin_len = 0;
                                memset(fr_pin_buffer,0,sizeof(fr_pin_buffer));
                                FR_UI_DrawPinBuffer();
                            }
                        }
                        else {
                            if (fr_pin_len < (int)(sizeof(fr_pin_buffer)-1)) {
                                fr_pin_buffer[fr_pin_len++] = label[0];
                                FR_UI_DrawPinBuffer();
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
