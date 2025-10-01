#ifndef APP_UI_START_H
#define APP_UI_START_H

typedef enum {
    UI_BTN_NONE = 0,
    UI_BTN_START,
    UI_BTN_ADMIN
} UI_ButtonResult;

void UI_StartScreen_Show(void);
UI_ButtonResult UI_WaitForButton(void);


#endif
