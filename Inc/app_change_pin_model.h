#ifndef APP_CHANGE_PIN_MODEL_H
#define APP_CHANGE_PIN_MODEL_H

#include <stdbool.h>

void CP_Model_Init(void);

bool CP_Model_VerifyOldPin(const char *pin);
void CP_Model_SetNewPin(const char *pin);
bool CP_Model_ConfirmNewPin(const char *pin);
void CP_Model_SaveFinalPin(void);

void CP_Model_ResetBuffer(void);

extern char cp_pin_buffer[8];
extern int  cp_pin_len;

#endif
