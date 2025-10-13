/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for FatFs SD-card application
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * Licensed under SLA0044.
  *
  ******************************************************************************
  */

#include "fatfs.h"
#include <string.h>
#include "sd_diskio.h"
/* SD FatFs variables */
uint8_t retSD;      /* Return value for SD */
char SDPath[4];     /* SD logical drive path */
FATFS SDFatFS;      /* File system object for SD logical drive */
FIL SDFile;         /* File object for SD */

/* -------------------------------------------------------------------------- */
/* Initialize FatFs and link SD driver                                        */
/* -------------------------------------------------------------------------- */
void MX_FATFS_Init(void)
{
  /* Link the SD driver */
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
}

/* -------------------------------------------------------------------------- */
/* Example routine: Create and write a text file                              */
/* -------------------------------------------------------------------------- */
void FatFS_routine(void)
{
  FRESULT res;
  UINT byteswritten;
  const char wtext[] = "This line has been written by STM32N6570-DK SD card!\n";

  /* Mount filesystem */
  if (f_mount(&SDFatFS, (TCHAR const*)SDPath, 1) == FR_OK)
  {
    /* Open or create file */
    res = f_open(&SDFile, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (res == FR_OK)
    {
      /* Write data */
      res = f_write(&SDFile, wtext, strlen(wtext), &byteswritten);

      /* Close file */
      f_close(&SDFile);

      if (res == FR_OK)
      {
        printf("[SD] File written successfully ✅ (%u bytes)\r\n", byteswritten);
      }
      else
      {
        printf("[SD] Write failed (%d)\r\n", res);
      }
    }
    else
    {
      printf("[SD] Failed to open file (%d)\r\n", res);
    }

    /* Unmount SD */
    f_mount(NULL, (TCHAR const*)SDPath, 0);
  }
  else
  {
    printf("[SD] Mount failed ❌\r\n");
  }
}

/* -------------------------------------------------------------------------- */
/* Timestamp provider (dummy if no RTC)                                       */
/* -------------------------------------------------------------------------- */
DWORD get_fattime(void)
{
  return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
