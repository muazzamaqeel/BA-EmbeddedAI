#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "stm32n6570_discovery_ts.h"

/* Initializes the touchscreen and starts polling task */
void APP_Touch_Init(void);

