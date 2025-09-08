#pragma once
#include "ll_aton_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

const LL_Buffer_InfoTypeDef *Detector_In_Info(void);
const LL_Buffer_InfoTypeDef *Detector_Out_Info(void);
void Detector_Run(void);

#ifdef __cplusplus
}
#endif
