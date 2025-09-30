#ifndef APP_UI_ADMIN_H
#define APP_UI_ADMIN_H

typedef enum {
    UI_ADMIN_BACK = 0,
    UI_ADMIN_OK   = 1
} UI_AdminResult_t;

/* Admin screen API */
void UI_AdminScreen_Show(void);
UI_AdminResult_t UI_AdminScreen_WaitForTouch(void);

#endif
