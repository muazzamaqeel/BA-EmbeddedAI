#ifndef APP_CHANGE_PIN_H
#define APP_CHANGE_PIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    CP_STEP_VERIFY_OLD = 0,
    CP_STEP_ENTER_NEW,
    CP_STEP_CONFIRM_NEW
} CP_State;

typedef enum {
    CP_RESULT_BACK_TO_ADMIN = 0,
    CP_RESULT_BACK_TO_START
} CP_Result;

#define CP_KEY_W   90
#define CP_KEY_H   65
#define CP_KEY_SP  20

#define CP_KEYPAD_ORIGIN_X  ((800/2 - (3*CP_KEY_W + 2*CP_KEY_SP)/2))
#define CP_KEYPAD_ORIGIN_Y  100

#define PIN_BOX_X   180
#define PIN_BOX_Y    20
#define PIN_BOX_W   440
#define PIN_BOX_H    50

extern char g_current_pin[8];
extern char g_admin_pin[8];
extern char g_decrypted_pin[8];

extern char cp_pin_buffer[8];
extern int  cp_pin_len;
extern char new_pin[8];
extern CP_State cp_state;

CP_Result UI_ChangePinScreen_Show(void);
CP_Result UI_ChangePinScreen_Show_Controller(void);

/* UI functions used by Controller */
void CP_UI_DrawBackground(void);
void CP_UI_LogPinBuffer(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_CHANGE_PIN_H */
