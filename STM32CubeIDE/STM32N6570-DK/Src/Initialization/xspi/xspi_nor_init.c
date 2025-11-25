#include "xspi_nor_init.h"
#include "stm32n6570_discovery_xspi.h"
#include "xspi_debug.h"
#include <stdio.h>
#include <string.h>
#include "error_handler.h"

int XSPI_NOR_Map_Once(void)
{
  BSP_XSPI_NOR_Init_t NOR_Init;
  memset(&NOR_Init, 0, sizeof(NOR_Init));
#ifdef BSP_XSPI_NOR_OPI_MODE
  NOR_Init.InterfaceMode = BSP_XSPI_NOR_OPI_MODE;
#endif
#ifdef BSP_XSPI_NOR_DTR_TRANSFER
  NOR_Init.TransferRate  = BSP_XSPI_NOR_DTR_TRANSFER;
#endif
#ifdef BSP_XSPI_NOR_DUALFLASH_DISABLE
  NOR_Init.DualFlash     = BSP_XSPI_NOR_DUALFLASH_DISABLE;
#endif

  const int candidates[2] = {0, 1};
  int used = -1;

  for (int t = 0; t < 2; ++t) {
    int inst = candidates[t];
    printf("[XSPI-NOR] Init_inst=%d...\r\n", inst);
    int32_t st = BSP_XSPI_NOR_Init(inst, &NOR_Init);
    if (st != BSP_ERROR_NONE) {
      printf("[XSPI-NOR] Init_failed on inst=%d, st=%ld\r\n", inst, (long)st);
      continue;
    }
    printf("[XSPI-NOR] Enable MMAP inst=%d...\r\n", inst);
    st = BSP_XSPI_NOR_EnableMemoryMappedMode(inst);
    if (st != BSP_ERROR_NONE) {
      printf("[XSPI-NOR] MMAP failed on inst=%d, st=%ld\r\n", inst, (long)st);
      (void)BSP_XSPI_NOR_DeInit(inst);
      continue;
    }
    used = inst;
    break;
  }

  if (used < 0) {
    printf("[XSPI-NOR][FATAL] Could not enable memory-mapped NOR on inst=1 or 0.\r\n");
    Error_Handler();
  } else {
    printf("[XSPI-NOR] Mapped OK on inst=%d\r\n", used);
    /* --- Check contents --- */
    xspi_quick_check();
    xspi_crc_check();
  }
  return 0;
}
