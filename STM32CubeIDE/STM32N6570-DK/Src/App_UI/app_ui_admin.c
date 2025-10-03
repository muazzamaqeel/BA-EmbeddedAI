/**
 ******************************************************************************
 * @file    app_ui_admin.c
 * @brief   Admin menu screen (after PIN success)
 ******************************************************************************
 */

#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_lcd.h"
#include <stdio.h>
#include "app_ui_admin.h"

/* --- Address of Admin background image in NOR Flash --- */
#define ADMIN_BG_ADDR   ((uint32_t)0x778A0000)

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

    /* Here you can later add buttons, menus, etc. */
    printf("[UI] Admin screen shown\r\n");
}
