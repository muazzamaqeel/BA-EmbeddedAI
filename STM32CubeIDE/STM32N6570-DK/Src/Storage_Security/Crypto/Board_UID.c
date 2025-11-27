#include "Board_UID.h"
#include <stdio.h>
#include <string.h>
#include "stm32n6xx_hal.h"

#define UID_ADDR0   ((uint32_t*)0x46009014)
#define UID_ADDR1   ((uint32_t*)0x46009018)
#define UID_ADDR2   ((uint32_t*)0x4600901C)

void FR_ReadUID(uint8_t uid[12])
{
    uint32_t w0 = *UID_ADDR0;
    uint32_t w1 = *UID_ADDR1;
    uint32_t w2 = *UID_ADDR2;

    memcpy(&uid[0],  &w0, 4);
    memcpy(&uid[4],  &w1, 4);
    memcpy(&uid[8],  &w2, 4);
}

void FR_PrintUID(void)
{
    uint8_t uid[12];
    FR_ReadUID(uid);

    printf("[UID] Raw (little-endian): ");
    for (int i = 0; i < 12; i++)
        printf("%02X", uid[i]);
    printf("\r\n");

    printf("[UID] Big-endian: ");
    for (int i = 11; i >= 0; i--)
        printf("%02X", uid[i]);
    printf("\r\n");
}
