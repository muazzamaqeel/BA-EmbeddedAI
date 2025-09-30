#ifndef APP_UI_PIN_H
#define APP_UI_PIN_H

#include <stdbool.h>

/* PIN screen API */
void UI_PinScreen_Show(const char *title);
bool UI_PinScreen_WaitForOK(const char *expectedPin);

#endif
