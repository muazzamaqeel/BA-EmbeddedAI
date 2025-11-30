#ifndef ADMIN_PIN_H
#define ADMIN_PIN_H

/* Global system PIN used across all modules */
extern char g_admin_pin[8];
void UI_DrawPinBuffer(void);
void UI_PinScreen_Show(void);
void UI_PinScreen_WaitForOK(void);

#endif /* ADMIN_PIN_H */
