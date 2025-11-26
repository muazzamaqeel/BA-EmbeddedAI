#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"
#include <stdio.h>
#include <stdbool.h>
#include "app_ui_admin.h"
#include "app_change_pin.h"
#include "app_change_face.h"
#include "app_shared.h"


void UI_Admin_DrawBackground(void)
{
    UTIL_LCD_SetLayer(0);
    BSP_LCD_DisplayOn(0);
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)ADMIN_BG_ADDR);
    printf("[UI] Admin background drawn (image @0x%08lX)\r\n", (unsigned long)ADMIN_BG_ADDR);
}

AdminResult UI_AdminScreen_Show(void)
{
    return UI_AdminScreen_Show_Controller();
}

