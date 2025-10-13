#ifndef USB_EMBEDDINGS_H
#define USB_EMBEDDINGS_H

#include "stm32n6xx_hal.h"
#include <stdbool.h>

bool USB_SD_IsDetected(void);
void USB_SD_PrintStatus(void);

#endif // USB_EMBEDDINGS_H
