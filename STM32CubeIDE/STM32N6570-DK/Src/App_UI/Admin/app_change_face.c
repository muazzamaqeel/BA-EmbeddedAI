/**
 ******************************************************************************
 * @file    app_change_face.c
 * @brief   Simple test screen shown after correct PIN
 ******************************************************************************
 */

#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery_lcd.h"
#include "stm32n6570_discovery.h"
#include "app_sleepmode.h"
#include <stdio.h>

/* ===== Public API ===== */
void UI_TestPassed_Show(void)
{
    APP_SleepMode_Disable();

    UTIL_LCD_SetLayer(1);
    BSP_LCD_DisplayOn(0);

    /* Background */
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

    /* Text */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKGREEN);
    UTIL_LCD_SetFont(&Font24);
    UTIL_LCD_DisplayStringAt(0, 200, (uint8_t*)"TEST PASSED", CENTER_MODE);

    printf("[UI] Test Passed screen displayed\r\n");
}
