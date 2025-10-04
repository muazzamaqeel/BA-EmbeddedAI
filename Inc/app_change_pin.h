#ifndef APP_CHANGE_PIN_H
#define APP_CHANGE_PIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* FSM for change PIN */
typedef enum {
    CP_STEP_VERIFY_OLD,
    CP_STEP_ENTER_NEW,
    CP_STEP_CONFIRM_NEW
} CP_State;

/* Return target after PIN change */
typedef enum {
    CP_RESULT_BACK_TO_ADMIN,
    CP_RESULT_BACK_TO_START
} CP_Result;

/* Function prototype */
CP_Result UI_ChangePinScreen_Show(void);

extern char g_current_pin[8];

#ifdef __cplusplus
}
#endif

#endif /* APP_CHANGE_PIN_H */
