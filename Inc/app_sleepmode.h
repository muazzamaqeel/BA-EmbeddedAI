#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void APP_SleepMode_Init(void);
bool APP_SleepMode_IsActive(void);

/* --- NEW: control APIs for override --- */
void APP_SleepMode_Disable(void);
void APP_SleepMode_Enable(void);
void APP_SleepMode_EnableCounter(bool enable);

#ifdef __cplusplus
}
#endif
