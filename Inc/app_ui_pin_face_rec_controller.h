#pragma once

#include <stdint.h>

#define FR_MAX_WRONG_PINS 3

extern char fr_pin_buffer[8];
extern int  fr_pin_len;
extern int  fr_wrong_pin_count;

/* Controller entry */
void UI_FR_PinScreen_WaitForOK_Controller(void);
