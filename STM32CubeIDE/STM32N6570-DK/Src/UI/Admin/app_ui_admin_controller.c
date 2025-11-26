#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"
#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "app_ui_admin.h"
#include "app_change_pin.h"
#include "app_change_face.h"
#include "app_sleepmode.h"
#include <stdio.h>
#include <stdbool.h>



AdminResult UI_AdminScreen_Show_Controller(void)
{
    UI_Admin_DrawBackground();
    printf("[UI] Admin screen shown (with 2 transparent buttons)\r\n");

    TS_State_t ts_state;
    bool touch_active = false;

    while (1) {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE) {
            if (ts_state.TouchDetected && !touch_active) {
                touch_active = true;
                uint16_t tx = ts_state.TouchX;
                uint16_t ty = ts_state.TouchY;
                if (tx >= BTN1_X && tx <= BTN1_X + BTN1_W &&
                    ty >= BTN1_Y && ty <= BTN1_Y + BTN1_H)
                {
                    printf("[UI] Admin Button1 pressed (Change PIN)\r\n");
                    CP_Result res = UI_ChangePinScreen_Show();

                    if (res == CP_RESULT_BACK_TO_START) {
                        printf("[UI] Returning to Start screen after PIN change\r\n");
                        APP_SleepMode_ResetState();
                        for (int i = 0; i < 10; i++) {
                            BSP_TS_GetState(0, &ts_state);
                            if (!ts_state.TouchDetected) break;
                            HAL_Delay(50);
                        }
                        return ADMIN_RESULT_BACK_TO_START;
                    } else {
                        UI_Admin_DrawBackground();
                        return ADMIN_RESULT_STAY;
                    }
                }
                else if (tx >= BTN2_X && tx <= BTN2_X + BTN2_W &&
                         ty >= BTN2_Y && ty <= BTN2_Y + BTN2_H)
                {
                    printf("[UI] Admin Button2 pressed (Change Face)\r\n");
                    UI_TestPassed_Show();
                    APP_SleepMode_ResetState();
                    for (int i = 0; i < 10; i++) {
                        BSP_TS_GetState(0, &ts_state);
                        if (!ts_state.TouchDetected) break;
                        HAL_Delay(50);
                    }
                    return ADMIN_RESULT_BACK_TO_START;
                }



            }
            else if (!ts_state.TouchDetected) {
                touch_active = false;
            }
        }
        HAL_Delay(50);
    }
}
