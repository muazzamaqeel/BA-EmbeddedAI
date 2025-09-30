/**
 ******************************************************************************
 * @file    app_ui_admin.c
 * @brief   Admin PIN entry screen (wrapper around UI_PinScreen)
 ******************************************************************************
 */

#include "app_ui_admin.h"
#include "app_ui_pin.h"   // reuse keypad
#include <stdio.h>

/* --- Admin PIN --- */
#define ADMIN_PASSCODE   "4321"   // change as needed

/* --- Public functions --- */
void UI_AdminScreen_Show(void)
{
    // Reuse the PIN screen with a custom title
    UI_PinScreen_Show("=== ADMIN PIN ===");
}

UI_AdminResult_t UI_AdminScreen_WaitForTouch(void)
{
    bool ok = UI_PinScreen_WaitForOK(ADMIN_PASSCODE);
    if (ok) {
        printf("[UI] Admin PIN correct\r\n");
        return UI_ADMIN_OK;
    }
    return UI_ADMIN_BACK; // normally never reached
}
