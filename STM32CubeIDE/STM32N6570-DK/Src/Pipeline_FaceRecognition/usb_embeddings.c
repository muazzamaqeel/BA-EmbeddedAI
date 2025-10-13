#include "usb_embeddings.h"
#include "stm32n6xx_hal.h"
#include "stm32n6xx_hal_sd.h"
#include "stm32n6xx_ll_sdmmc.h"
#include "stm32n6570_discovery_sd.h"
#include <stdio.h>


/**
  * @brief Check if the microSD card is detected in the slot.
  * @retval true if card inserted, false otherwise
  */
bool USB_SD_IsDetected(void)
{
    /* BSP_SD_IsDetected() returns SD_PRESENT or SD_NOT_PRESENT */
    if (BSP_SD_IsDetected(0) == SD_PRESENT)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
  * @brief Print card detection status (for UART or console logging)
  */
void USB_SD_PrintStatus(void)
{
    if (USB_SD_IsDetected())
    {
        printf("[SD] Card detected ✅\n");
    }
    else
    {
        printf("[SD] No card detected ❌\n");
    }
}
