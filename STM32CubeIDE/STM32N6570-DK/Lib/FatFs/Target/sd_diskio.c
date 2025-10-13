/**
  ******************************************************************************
  * @file    sd_diskio.c
  * @brief   FatFs SD driver for STM32N6570-DK (SDMMC interface)
  ******************************************************************************
  */

#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "stm32n6570_discovery_sd.h"

/* Private variables ---------------------------------------------------------*/
static volatile DSTATUS Stat = STA_NOINIT;

/* Private function prototypes -----------------------------------------------*/
static DSTATUS SD_initialize(BYTE);
static DSTATUS SD_status(BYTE);
static DRESULT SD_read(BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
static DRESULT SD_write(BYTE, const BYTE*, DWORD, UINT);
#endif
#if _USE_IOCTL == 1
static DRESULT SD_ioctl(BYTE, BYTE, void*);
#endif

/* Public FatFs driver structure ---------------------------------------------*/
const Diskio_drvTypeDef SD_Driver = {
  SD_initialize,
  SD_status,
  SD_read,
#if _USE_WRITE == 1
  SD_write,
#endif
#if _USE_IOCTL == 1
  SD_ioctl,
#endif
};

/**
  * @brief  Initializes the SD card.
  */
static DSTATUS SD_initialize(BYTE lun)
{
  if (BSP_SD_Init(0) == BSP_ERROR_NONE)
  {
    Stat &= ~STA_NOINIT;
  }
  else
  {
    Stat = STA_NOINIT;
  }
  return Stat;
}

/**
  * @brief  Gets the disk status.
  */
static DSTATUS SD_status(BYTE lun)
{
  return Stat;
}

/**
  * @brief  Reads sector(s).
  */
static DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  if (BSP_SD_ReadBlocks(0, buff, sector, count) != BSP_ERROR_NONE)
  {
    return RES_ERROR;
  }

  while (BSP_SD_GetCardState(0) != SD_TRANSFER_OK)
  {
  }
  return RES_OK;
}

/**
  * @brief  Writes sector(s).
  */
#if _USE_WRITE == 1
static DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  if (BSP_SD_WriteBlocks(0, (uint8_t *)buff, sector, count) != BSP_ERROR_NONE)
  {
    return RES_ERROR;
  }

  while (BSP_SD_GetCardState(0) != SD_TRANSFER_OK)
  {
  }
  return RES_OK;
}
#endif

/**
  * @brief  I/O control operation.
  */
#if _USE_IOCTL == 1
static DRESULT SD_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  BSP_SD_CardInfo CardInfo;

  switch (cmd)
  {
    case CTRL_SYNC:
      res = RES_OK;
      break;

    case GET_SECTOR_COUNT:
      BSP_SD_GetCardInfo(0, &CardInfo);
      *(DWORD *)buff = CardInfo.LogBlockNbr;
      res = RES_OK;
      break;

    case GET_SECTOR_SIZE:
      BSP_SD_GetCardInfo(0, &CardInfo);
      *(WORD *)buff = CardInfo.LogBlockSize;
      res = RES_OK;
      break;

    case GET_BLOCK_SIZE:
      *(DWORD *)buff = 1;
      res = RES_OK;
      break;

    default:
      res = RES_PARERR;
      break;
  }

  return res;
}
#endif
