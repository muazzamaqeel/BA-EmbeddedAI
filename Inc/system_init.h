#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include "stm32n6xx_hal.h"

void SystemClock_Config(void);
void NPURam_enable(void);
void NPUCache_config(void);
void Security_Config(void);
void IAC_Config(void);
void IAC_IRQHandler(void);
void CONSOLE_Config(void);
int  HyperRAM_Map_Once(void);

HAL_StatusTypeDef MX_DCMIPP_ClockConfig(DCMIPP_HandleTypeDef *hdcmipp);
void HAL_CACHEAXI_MspInit(CACHEAXI_HandleTypeDef *hcacheaxi);
void HAL_CACHEAXI_MspDeInit(CACHEAXI_HandleTypeDef *hcacheaxi);

#endif
