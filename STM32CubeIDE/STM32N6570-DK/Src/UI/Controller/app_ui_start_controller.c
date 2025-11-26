#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"
#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "app_ui_start.h"
#include "app_ui_pin.h"
#include "app_sleepmode.h"
#include <stdio.h>


UI_ButtonResult UI_WaitForButton(void)
{
    TS_State_t ts_state;
    printf("[UI] Waiting for Start/Admin button...\r\n");

    while (1) {
        if (BSP_TS_GetState(0, &ts_state) == BSP_ERROR_NONE && ts_state.TouchDetected) {

            uint16_t tx = ts_state.TouchX;
            uint16_t ty = ts_state.TouchY;

            /* -------------------- START -------------------- */
            if (tx >= BTN_START_X && tx <= BTN_START_X + BTN_START_W &&
                ty >= BTN_START_Y && ty <= BTN_START_Y + BTN_START_H)
            {
                printf("[UI] Start pressed!\r\n");

                APP_SleepMode_ResetState();  // make sure sleep logic clean

                BSP_LCD_SetLayerVisible(0, 1, DISABLE);
                UTIL_LCD_Clear(UTIL_LCD_COLOR_TRANSPARENT);
                UTIL_LCD_SetLayer(1);
                BSP_LCD_Reload(0, BSP_LCD_RELOAD_IMMEDIATE);
                HAL_Delay(100);

                extern void Pipeline_TriggerStart(void);
                Pipeline_TriggerStart();

                return UI_BTN_START;
            }

            /* -------------------- ADMIN -------------------- */
            if (tx >= BTN_ADMIN_X && tx <= BTN_ADMIN_X + BTN_ADMIN_W &&
                ty >= BTN_ADMIN_Y && ty <= BTN_ADMIN_Y + BTN_ADMIN_H)
            {
                printf("[UI] Admin pressed!\r\n");

                APP_SleepMode_ResetState();  // clean state before admin flow

                BSP_LCD_SetLayerVisible(0, 1, DISABLE);
                BSP_LCD_Reload(0, BSP_LCD_RELOAD_IMMEDIATE);
                UTIL_LCD_Clear(UTIL_LCD_COLOR_TRANSPARENT);
                HAL_Delay(100);

                UI_PinScreen_Show();
                UI_PinScreen_WaitForOK();

                BSP_LCD_SetLayerVisible(0, 1, ENABLE);
                BSP_LCD_Reload(0, BSP_LCD_RELOAD_IMMEDIATE);

                printf("[UI] PIN screen finished, returning to start menu...\r\n");
                return UI_BTN_ADMIN;
            }
        }
        HAL_Delay(50);
    }
}
