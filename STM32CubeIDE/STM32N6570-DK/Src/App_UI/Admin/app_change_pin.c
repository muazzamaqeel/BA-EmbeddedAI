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

/* Shared current PIN (default "1234") */
char g_current_pin[8] = "1234";
char g_admin_pin[8] = "1234";
/* Global decrypted PIN buffer shared across modules */
char g_decrypted_pin[8] = {0};

/* --- Keypad layout (same as app_ui_pin.c) --- */
#define CP_KEY_W   90
#define CP_KEY_H   65
#define CP_KEY_SP  20
#define CP_KEYPAD_ORIGIN_X  ((800/2 - (3*CP_KEY_W + 2*CP_KEY_SP)/2))
#define CP_KEYPAD_ORIGIN_Y  100

/* --- PIN display box (white background above keypad) --- */
#define PIN_BOX_X   180   // move a bit more to the left (align with keypad)
#define PIN_BOX_Y    20   // raise higher above keypad
#define PIN_BOX_W   440   // make wider (matches keypad width visually)
#define PIN_BOX_H    50   // slightly taller for clearer visibility



/* Buffers */
static char cp_pin_buffer[8];
static int  cp_pin_len = 0;
static char new_pin[8];

/* Current FSM state */
static CP_State cp_state;

static void CP_UI_DrawBackground(void)
{
    /* Just draw background image, no text */
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)0x77AE0000);

    /* Draw PIN input box */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(PIN_BOX_X, PIN_BOX_Y, PIN_BOX_W, PIN_BOX_H, UTIL_LCD_COLOR_WHITE);

    /* Outline for clarity */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(PIN_BOX_X, PIN_BOX_Y, PIN_BOX_W, PIN_BOX_H, UTIL_LCD_COLOR_BLACK);

    printf("[UI-CP] Change PIN screen background drawn\r\n");
}


static void CP_UI_LogPinBuffer(void)
{
    char disp[16];
    memset(disp, '*', cp_pin_len);
    disp[cp_pin_len] = '\0';

    /* Clear old content area */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillRect(PIN_BOX_X + 2, PIN_BOX_Y + 2, PIN_BOX_W - 4, PIN_BOX_H - 4, UTIL_LCD_COLOR_WHITE);

    /* Draw centered stars */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetFont(&Font24);  // Bigger, visible font

    int textWidth  = strlen(disp) * 14;  // Approx width per char
    int textHeight = 24;                 // Font24 height
    int textX = PIN_BOX_X + (PIN_BOX_W - textWidth) / 2;
    int textY = PIN_BOX_Y + (PIN_BOX_H - textHeight) / 2;

    UTIL_LCD_DisplayStringAt(textX, textY, (uint8_t*)disp, LEFT_MODE);

    printf("[UI-CP] PIN buffer updated: '%s'\r\n", disp);
}


/* ===== Public API ===== */
CP_Result UI_ChangePinScreen_Show(void)
{
    APP_SleepMode_Disable();

    cp_state = CP_STEP_VERIFY_OLD;
    cp_pin_len = 0;
    memset(cp_pin_buffer, 0, sizeof(cp_pin_buffer));

    CP_UI_DrawBackground();
    CP_UI_LogPinBuffer();

    printf("[UI-CP] Change PIN started: waiting for old PIN\r\n");

    TS_State_t ts_state;
    bool touch_active = false;

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

                for (int r = 0; r < 4; r++) {
                    for (int c = 0; c < 3; c++) {
                        int x = CP_KEYPAD_ORIGIN_X + c * (CP_KEY_W + CP_KEY_SP);
                        int y = CP_KEYPAD_ORIGIN_Y + r * (CP_KEY_H + CP_KEY_SP);
                        if (r == 1) y += 10;
                        if (r == 2) y += 25;
                        if (r == 3) y += 40;
                        if (c == 2) x -= 15;

                        if (tx >= x && tx <= x + CP_KEY_W &&
                            ty >= y && ty <= y + CP_KEY_H) {

                            const char *label = keys[r][c];
                            printf("[UI-CP] Hidden key pressed: %s\r\n", label);

                            if (strcmp(label, "CLR") == 0) {
                                cp_pin_len = 0;
                                memset(cp_pin_buffer, 0, sizeof(cp_pin_buffer));
                                CP_UI_LogPinBuffer();
                            }
                            else if (strcmp(label, "OK") == 0) {
                                cp_pin_buffer[cp_pin_len] = '\0';

                                if (cp_state == CP_STEP_VERIFY_OLD) {
                                	if (strcmp(cp_pin_buffer, g_admin_pin) == 0){
                                        cp_state = CP_STEP_ENTER_NEW;
                                        printf("[UI-CP] Old PIN verified, enter new PIN\r\n");
                                    } else {
                                        printf("[UI-CP] Wrong old PIN! Try again\r\n");
                                    }
                                }
                                else if (cp_state == CP_STEP_ENTER_NEW) {
                                    strcpy(new_pin, cp_pin_buffer);
                                    cp_state = CP_STEP_CONFIRM_NEW;
                                    printf("[UI-CP] New PIN entered, please confirm\r\n");
                                }
                                else if (cp_state == CP_STEP_CONFIRM_NEW) {
                                    if (strcmp(cp_pin_buffer, new_pin) == 0) {
                                    	strcpy(g_admin_pin, new_pin);
                                        printf("[UI-CP] PIN successfully changed to: %s\r\n", g_admin_pin);
                                        HAL_Delay(1000);
                                        APP_SleepMode_Enable();
                                        return CP_RESULT_BACK_TO_START;  // ✅ go back to Start screen
                                    } else {
                                        printf("[UI-CP] Mismatch! Restarting from new PIN entry\r\n");
                                        cp_state = CP_STEP_ENTER_NEW;
                                    }
                                }

                                cp_pin_len = 0;
                                memset(cp_pin_buffer, 0, sizeof(cp_pin_buffer));
                                CP_UI_LogPinBuffer();
                            }
                            else {
                                if (cp_pin_len < (int)(sizeof(cp_pin_buffer) - 1)) {
                                    cp_pin_buffer[cp_pin_len++] = label[0];
                                    CP_UI_LogPinBuffer();
                                }
                            }
                        }
                    }
                }
            }
            else if (!ts_state.TouchDetected) {
                touch_active = false;
            }
        }
        HAL_Delay(30);
    }
}
