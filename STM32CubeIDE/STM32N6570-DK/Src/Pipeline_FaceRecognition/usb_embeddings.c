#include "usb_embeddings.h"
#include "stm32n6570_discovery_sd.h"
#include <stdio.h>
#include <string.h>

/* Single global FS instance (others use extern in headers) */
FATFS fs;
static bool sd_mounted = false;

/* =============================== */
/* SD Card low-level helpers       */
/* =============================== */

bool USB_SD_IsDetected(void)
{
    return (BSP_SD_IsDetected(0) == SD_PRESENT);
}

void USB_SD_PrintStatus(void)
{
    if (USB_SD_IsDetected())
        printf("[SD] Card detected ✅\r\n");
    else
        printf("[SD] No card detected ❌\r\n");
}

bool USB_SD_Mount(void)
{
    FRESULT res = f_mount(&fs, "0:", 1);
    if (res == FR_OK)
    {
        sd_mounted = true;
        printf("[SD] Mounted successfully ✅\r\n");
        return true;
    }
    else
    {
        sd_mounted = false;
        printf("[SD] Mount failed (%d)\r\n", res);
        return false;
    }
}

bool USB_SD_EnsureMounted(void)
{
    if (!USB_SD_IsDetected())
    {
        printf("[SD] No card detected ❌\r\n");
        return false;
    }
    if (sd_mounted)
        return true;
    return USB_SD_Mount();
}

void USB_SD_ListDir(const char *path)
{
    DIR dir;
    FILINFO fno;
    FRESULT res = f_opendir(&dir, path);
    if (res == FR_OK)
    {
        printf("[SD] Listing: %s\r\n", path);
        for (;;)
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0)
                break;
            printf("  %s%s\r\n", fno.fname, (fno.fattrib & AM_DIR) ? "/" : "");
        }
        f_closedir(&dir);
    }
    else
    {
        printf("[SD] Failed to open dir %s (err %d)\r\n", path, res);
    }
}

void USB_SD_Test(void)
{
    USB_SD_PrintStatus();

    if (USB_SD_IsDetected())
    {
        if (USB_SD_Mount())
        {
            USB_SD_ListDir("0:");
            USB_SD_ListDir("0:/binaries");
        }
    }
}
