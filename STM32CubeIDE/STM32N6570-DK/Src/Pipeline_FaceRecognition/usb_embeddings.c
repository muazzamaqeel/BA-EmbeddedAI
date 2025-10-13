#include "usb_embeddings.h"
#include "stm32n6xx_hal.h"
#include "stm32n6xx_hal_sd.h"
#include "stm32n6xx_ll_sdmmc.h"
#include "stm32n6570_discovery_sd.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Local objects */
static FATFS fs;
static FIL file;
static bool sd_mounted = false;

/* -------------------------------------------------------------------------- */
/* Helper: Check card presence                                                */
/* -------------------------------------------------------------------------- */
bool USB_SD_IsDetected(void)
{
    return (BSP_SD_IsDetected(0) == SD_PRESENT);
}

/* -------------------------------------------------------------------------- */
/* Helper: Print card detection status                                        */
/* -------------------------------------------------------------------------- */
void USB_SD_PrintStatus(void)
{
    if (USB_SD_IsDetected())
        printf("[SD] Card detected ✅\r\n");
    else
        printf("[SD] No card detected ❌\r\n");
}

/* -------------------------------------------------------------------------- */
/* Mount SD card                                                              */
/* -------------------------------------------------------------------------- */
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
        printf("[SD] Mount failed (%d)\r\n", res);
        return false;
    }
}

/* -------------------------------------------------------------------------- */
/* List directory contents (for debugging)                                   */
/* -------------------------------------------------------------------------- */
static void USB_SD_ListDir(const char *path)
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

/* -------------------------------------------------------------------------- */
/* Read and print content of embeddings file                                  */
/* -------------------------------------------------------------------------- */
void USB_SD_ReadEmbeddings(const char *filename)
{
    if (!USB_SD_IsDetected())
    {
        printf("[SD] No card detected ❌\r\n");
        return;
    }

    if (!sd_mounted)
    {
        if (!USB_SD_Mount())
            return;
    }

    char filepath[128];
    snprintf(filepath, sizeof(filepath), "0:%s", filename);

    FRESULT res = f_open(&file, filepath, FA_READ);
    if (res != FR_OK)
    {
        printf("[SD] Failed to open file: %s (error %d)\r\n", filepath, res);
        USB_SD_ListDir("0:");
        USB_SD_ListDir("0:faces");
        return;
    }

    printf("[SD] Reading file: %s\r\n", filepath);

    char buffer[256];
    while (f_gets(buffer, sizeof(buffer), &file))
    {
        printf("%s", buffer);
    }

    f_close(&file);
    printf("\r\n[SD] Done reading %s ✅\r\n", filepath);
}

/* -------------------------------------------------------------------------- */
/* Simple test entry point                                                    */
/* -------------------------------------------------------------------------- */
void USB_SD_Test(void)
{
    USB_SD_PrintStatus();

    if (USB_SD_IsDetected())
    {
        if (USB_SD_Mount())
        {
            /* Optional: see SD card contents */
            USB_SD_ListDir("0:");
            USB_SD_ListDir("0:faces");

            /* Read Muazzam’s embeddings file */
            USB_SD_ReadEmbeddings("faces/embeddings_table_Muazzam.c");

            /* Optionally:
             * USB_SD_ReadEmbeddings("faces/embeddings_table_Keti.c");
             * USB_SD_ReadEmbeddings("faces/embeddings_table_Aida.c");
             */
        }
    }
}
