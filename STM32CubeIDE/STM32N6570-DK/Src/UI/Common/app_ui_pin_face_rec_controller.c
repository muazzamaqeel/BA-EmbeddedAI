/**
 *******************************************************************************
 * @file    app_ui_pin_face_rec_controller.c
 * @brief   PIN entry / Auth screen (FaceRec variant)
 *          - Visible keypad
 *          - Debounced touch
 *          - Shows "Wrong PIN" or "Success" above PIN box
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
#include "app_ui_pin_face_rec_controller.h"
#include "app_ui_pin_face_rec.h"

#define FR_KEY_W   100
#define FR_KEY_H   65
#define FR_KEY_SP  22
#define FR_KEYPAD_ORIGIN_X  ((800/2 - (3*FR_KEY_W + 2*FR_KEY_SP)/2))
#define FR_KEYPAD_ORIGIN_Y  145
char fr_pin_buffer[8];
int  fr_pin_len = 0;
#define FR_PIN_BG_COLOR  UTIL_LCD_COLOR_DARKGRAY


void UI_FR_PinScreen_WaitForOK_Controller(void)
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

                        if (strcmp(label,"CLR")==0) {
                            fr_pin_len = 0;
                            memset(fr_pin_buffer,0,sizeof(fr_pin_buffer));
                            FR_UI_ShowStatus("Enter PIN", UTIL_LCD_COLOR_WHITE);
                            FR_UI_DrawPinBuffer(fr_pin_len);
                        }
                        else if (strcmp(label,"OK")==0) {
                            fr_pin_buffer[fr_pin_len] = '\0';

                            if (strcmp(fr_pin_buffer, g_current_pin) == 0) {
                                FR_UI_ShowStatus("Success", UTIL_LCD_COLOR_GREEN);
                                printf("[UI-FR] Correct PIN entered!\r\n");
                                HAL_Delay(50);  // brief success flash

                                APP_SleepMode_Enable();
                                APP_FaceDetection_Reset();

                                extern bool g_pipeline_running;
                                extern bool g_fr_active;
                                extern uint32_t g_last_face_time;
                                extern uint32_t g_wake_time;

                                g_fr_active = false;

                                g_last_face_time = HAL_GetTick();
                                g_wake_time      = g_last_face_time;


                                return;
                            }
                            else {
                                FR_UI_ShowStatus("Wrong PIN", UTIL_LCD_COLOR_RED);
                                printf("[UI-FR] Wrong PIN!\r\n");
                                fr_pin_len = 0;
                                memset(fr_pin_buffer,0,sizeof(fr_pin_buffer));
                                FR_UI_DrawPinBuffer(fr_pin_len);
                            }
                        }
                        else {
                            if (fr_pin_len < (int)(sizeof(fr_pin_buffer)-1)) {
                                fr_pin_buffer[fr_pin_len++] = label[0];
                                FR_UI_DrawPinBuffer(fr_pin_len);
                            }
                        }
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
