#ifndef APP_UI_START_H
#define APP_UI_START_H

/* Result of start screen button press */
typedef enum {
    UI_BTN_NONE = 0,
    UI_BTN_START,
    UI_BTN_ADMIN
} UI_ButtonResult_t;

void UI_StartScreen_Show(void);
UI_ButtonResult_t UI_WaitForButton(void);

#endif /* APP_UI_START_H */
