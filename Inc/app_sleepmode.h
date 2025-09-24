#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void APP_SleepMode_Init(void);
bool APP_SleepMode_IsActive(void);

#ifdef __cplusplus
}
#endif
