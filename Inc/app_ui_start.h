#ifndef APP_UI_START_H
#define APP_UI_START_H

#include <stdint.h>

/* --------------------------------------------------
 *  Transparent color (needed by both View + Controller)
 * -------------------------------------------------- */
#ifndef UTIL_LCD_COLOR_TRANSPARENT
#define UTIL_LCD_COLOR_TRANSPARENT 0x0000u
#endif

/* --------------------------------------------------
 *  Button Layout (shared between View + Controller)
 * -------------------------------------------------- */
#define BTN_START_W   240
#define BTN_START_H   60
#define BTN_START_X   (800 - BTN_START_W - 20)
#define BTN_START_Y   (480 - BTN_START_H - 20)

#define BTN_ADMIN_W   BTN_START_W
#define BTN_ADMIN_H   BTN_START_H
#define BTN_ADMIN_X   (BTN_START_X - BTN_ADMIN_W - 20)
#define BTN_ADMIN_Y   BTN_START_Y

/* --------------------------------------------------
 *  Start screen button result
 * -------------------------------------------------- */
typedef enum {
    UI_BTN_NONE = 0,
    UI_BTN_START,
    UI_BTN_ADMIN
} UI_ButtonResult;

/* --------------------------------------------------
 *  Public API
 * -------------------------------------------------- */
void UI_StartScreen_Show(void);
UI_ButtonResult UI_WaitForButton(void);

#endif
