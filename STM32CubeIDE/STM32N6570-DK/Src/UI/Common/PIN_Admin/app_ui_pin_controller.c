/**
 ******************************************************************************
 * @file    app_ui_pin_controller.c
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
#include "app_ui_pin_controller.h"
#include "app_sleepmode.h"

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
char pin_buffer[8];
int  pin_len = 0;


void UI_PinScreen_WaitForOK_Controller(void)
{
    TS_State_t ts_state;
    bool touch_down = false;   // strict debounce

    printf("[UI] Waiting for keypad input (hidden)...\r\n");

    while (1)
    {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE)
        {
            /* --------------------------------------------------------
             * TOUCH DOWN EVENT (one input only)
             * -------------------------------------------------------- */
            if (ts_state.TouchDetected && !touch_down)
            {
                // Stabilize initial touch: avoid false-positive
                HAL_Delay(80);
                BSP_TS_GetState(0, &ts_state);
                if (!ts_state.TouchDetected)
                    continue;   // noise — ignore

                touch_down = true;   // real press

                uint16_t tx = ts_state.TouchX;
                uint16_t ty = ts_state.TouchY;

                const char *keys[4][3] = {
                    {"1","2","3"},
                    {"4","5","6"},
                    {"7","8","9"},
                    {"CLR","0","OK"}
                };

                for (int r = 0; r < 4; r++)
                {
                    for (int c = 0; c < 3; c++)
                    {
                        int x = KEYPAD_ORIGIN_X + c * (KEY_W + KEY_SP);
                        int y = KEYPAD_ORIGIN_Y + r * (KEY_H + KEY_SP);

                        if (r == 1) y += 10;
                        if (r == 2) y += 25;
                        if (r == 3) y += 40;
                        if (c == 2) x -= 15;

                        if (tx >= x && tx <= x + KEY_W &&
                            ty >= y && ty <= y + KEY_H)
                        {
                            const char *label = keys[r][c];
                            printf("[TS] Hidden key pressed: %s\r\n", label);

                            if (strcmp(label, "CLR") == 0) {
                                pin_len = 0;
                                memset(pin_buffer, 0, sizeof(pin_buffer));
                                UI_DrawPinBuffer();
                            }
                            else if (strcmp(label, "OK") == 0) {
                                pin_buffer[pin_len] = '\0';

                                if (strcmp(pin_buffer, g_admin_pin) == 0) {
                                    printf("[UI] Correct PIN entered!\r\n");
                                    APP_SleepMode_Enable();
                                    return;
                                } else {
                                    printf("[UI] Wrong PIN\r\n");
                                    pin_len = 0;
                                    memset(pin_buffer, 0, sizeof(pin_buffer));
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

            /* --------------------------------------------------------
             * TOUCH UP EVENT (must be stable, no jitter)
             * -------------------------------------------------------- */
            else if (!ts_state.TouchDetected && touch_down)
            {
                HAL_Delay(80);  // confirm finger really lifted
                BSP_TS_GetState(0, &ts_state);

                if (!ts_state.TouchDetected) {
                    touch_down = false; // now allow new key
                }
            }
        }

        HAL_Delay(20);
    }
}
