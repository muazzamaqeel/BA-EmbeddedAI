#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

void APP_SleepMode_Init(void);
void APP_SleepMode_Disable(void);
void APP_SleepMode_Enable(void);
void APP_SleepMode_EnableCounter(bool enable);
void APP_SleepMode_EnableCounterDelayed(uint32_t delay_ms);  // ✅ this needs uint32_t
void APP_SleepMode_UpdateFaceActivity(bool face_detected);
bool APP_SleepMode_IsActive(void);
void APP_SleepMode_ResetState(void);


#ifdef __cplusplus
}
#endif
