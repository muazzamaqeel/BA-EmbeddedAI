/**
 ******************************************************************************
 * @file    app_ui_admin.c
 * @brief   Admin menu screen (after PIN success)
 ******************************************************************************
 */

#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"
#include <stdio.h>
#include "app_ui_admin.h"
#include <stdbool.h>
#include "app_change_pin.h"

/* --- Address of Admin background image in NOR Flash --- */
#define ADMIN_BG_ADDR   ((uint32_t)0x778A0000)

/* --- Button geometry (transparent) --- */
#define BTN1_X   200
#define BTN1_Y   300
#define BTN1_W   150
#define BTN1_H   80

#define BTN2_X   450
#define BTN2_Y   300
#define BTN2_W   150
#define BTN2_H   80

/* ===== Internal helpers ===== */
static void UI_Admin_DrawBackground(void)
{
    UTIL_LCD_SetLayer(0);
    BSP_LCD_DisplayOn(0);

    /* Draw RGB565 raw image stored in NOR */
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)ADMIN_BG_ADDR);

    printf("[UI] Admin background drawn (image @0x%08lX)\r\n", (unsigned long)ADMIN_BG_ADDR);
}

/* ===== Public API ===== */
void UI_AdminScreen_Show(void)
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
                    ty >= BTN1_Y && ty <= BTN1_Y + BTN1_H) {
                    printf("[UI] Admin Button1 pressed (Change PIN)\r\n");
                    UI_ChangePinScreen_Show();
                    UI_Admin_DrawBackground();   // redraw Admin screen after return
                }
                else if (tx >= BTN2_X && tx <= BTN2_X + BTN2_W &&
                         ty >= BTN2_Y && ty <= BTN2_Y + BTN2_H) {
                    printf("[UI] Admin Button2 pressed\r\n");
                }
            }
            else if (!ts_state.TouchDetected) {
                touch_active = false;
            }
        }
        HAL_Delay(50);
    }
}
