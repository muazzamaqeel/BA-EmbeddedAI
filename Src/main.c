 /**
 ******************************************************************************
 * @file    main.c
 * @author  GPM Application Team
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <assert.h>
#include <string.h>
#include "app.h"
#include "app_config.h"
#include "app_fuseprogramming.h"
#include "main.h"
#include "npu_cache.h"
#ifdef STM32N6570_DK_REV
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_bus.h"
#include "stm32n6570_discovery_lcd.h"
#include "stm32n6570_discovery_xspi.h"
#else
#include "stm32n6xx_nucleo.h"
#include "stm32n6xx_nucleo_bus.h"
#include "stm32n6xx_nucleo_xspi.h"
#endif
#include <stdio.h>
#include "stm32n6xx_hal_rif.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pipeline_start.h"
#include "app_ui_start.h"
#include "xspi_debug.h"
#include "system_init.h"
#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery_ts.h"

UART_HandleTypeDef huart1;

static StaticTask_t main_thread;
static StackType_t main_thread_stack[configMINIMAL_STACK_SIZE];


static int main_freertos(void);
static void main_thread_fct(void *arg);
void Error_Handler(void);

/* This is defined in port.c */
void vPortSetupTimerInterrupt(void);




























int main(void)
{
  /* Power on ICACHE */
  MEMSYSCTL->MSCR |= MEMSYSCTL_MSCR_ICACTIVE_Msk;

  /* Set back system and CPU clock source to HSI */
  __HAL_RCC_CPUCLK_CONFIG(RCC_CPUCLKSOURCE_HSI);
  __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_HSI);

  HAL_Init();

  SCB_EnableICache();

#if defined(USE_DCACHE)
  /* Power on DCACHE */
  MEMSYSCTL->MSCR |= MEMSYSCTL_MSCR_DCACTIVE_Msk;
  SCB_EnableDCache();
#endif

  return main_freertos();
}


static int main_freertos()
{
  TaskHandle_t hdl;

  hdl = xTaskCreateStatic(main_thread_fct, "main", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1,
                          main_thread_stack, &main_thread);
  assert(hdl != NULL);

  vTaskStartScheduler();
  assert(0);

  return -1;
}








static void main_thread_fct(void *arg)
{
  uint32_t preemptPriority;
  uint32_t subPriority;
  IRQn_Type i;

  /* Inherit SysTick priority to all IRQs so they stay below
     configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY. */
  HAL_NVIC_GetPriority(SysTick_IRQn, HAL_NVIC_GetPriorityGrouping(),
                       &preemptPriority, &subPriority);
  for (i = PVD_PVM_IRQn; i <= LTDC_UP_ERR_IRQn; i++) {
    HAL_NVIC_SetPriority(i, preemptPriority, subPriority);
  }

  /* System clock uses HAL_Delay() → call after scheduler starts */
  SystemClock_Config();
  vPortSetupTimerInterrupt();

  /* UART for logs */
  CONSOLE_Config();

  /* NPU SRAMs + caches */
  NPURam_enable();
  Fuse_Programming();
  NPUCache_config();

#ifdef STM32N6570_DK_REV
  /* ---- External RAM: HyperRAM on XSPI1 ----
     Map ONCE (idempotent), used by IN_PSRAM buffers. */
  if (HyperRAM_Map_Once() != 0) {
    Error_Handler();
  }
#endif

  /* ---- External NOR (weights): prefer XSPI2, fallback XSPI1 ---- */
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

    // in main_thread_fct()
    const int candidates[2] = {0, 1};
    int used = -1;

    for (int t = 0; t < 2; ++t) {
      int inst = candidates[t];
      printf("[XSPI-NOR] Init inst=%d...\r\n", inst);
      int32_t st = BSP_XSPI_NOR_Init(inst, &NOR_Init);
      if (st != BSP_ERROR_NONE) {
        printf("[XSPI-NOR] Init failed on inst=%d, st=%ld\r\n", inst, (long)st);
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
  }

  /* !!! IMPORTANT !!!
     You previously had *another* unconditional NOR init here:
         BSP_XSPI_NOR_Init(0, ...);
         BSP_XSPI_NOR_EnableMemoryMappedMode(0);
     That re-mapped NOR again and could stall.  → Removed. */

  /* Secure privileged attributes for IPs */
  Security_Config();

  /* Illegal Access Controller (optional trap) */
  IAC_Config();

  /* Keep IPs clocked in low-power (as before) */
  LL_BUS_EnableClockLowPower(~0);
  LL_MEM_EnableClockLowPower(~0);
  LL_AHB1_GRP1_EnableClockLowPower(~0);
  LL_AHB2_GRP1_EnableClockLowPower(~0);
  LL_AHB3_GRP1_EnableClockLowPower(~0);
  LL_AHB4_GRP1_EnableClockLowPower(~0);
  LL_AHB5_GRP1_EnableClockLowPower(~0);
  LL_APB1_GRP1_EnableClockLowPower(~0);
  LL_APB1_GRP2_EnableClockLowPower(~0);
  LL_APB2_GRP1_EnableClockLowPower(~0);
  LL_APB4_GRP1_EnableClockLowPower(~0);
  LL_APB4_GRP2_EnableClockLowPower(~0);
  LL_APB5_GRP1_EnableClockLowPower(~0);
  LL_MISC_EnableClockLowPower(~0);

  /* ---- LCD / GUI Start ---- */
  printf("[APP] Initializing LCD...\r\n");
  if (BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE) != BSP_ERROR_NONE) {
      printf("[ERROR] LCD init failed!\r\n");
      Error_Handler();
  }
  /* Link UTIL to BSP driver */
  UTIL_LCD_SetFuncDriver(&LCD_Driver);
  UTIL_LCD_SetLayer(0);
  BSP_LCD_DisplayOn(0);
  /* ---- Touchscreen Init ---- */
  TS_Init_t hTS;
  hTS.Width  = 800;   // LCD X resolution
  hTS.Height = 480;   // LCD Y resolution
  hTS.Orientation = TS_SWAP_NONE;  // adjust if axes swapped
  hTS.Accuracy = 5;   // pixels
  if (BSP_TS_Init(0, &hTS) != BSP_ERROR_NONE) {
      printf("[ERROR] TS init failed!\r\n");
      Error_Handler();
  }
  printf("[APP] Touchscreen initialized\r\n");
  UI_StartScreen_Show();
  UI_WaitForButton();
  Pipeline_Start();
  vTaskDelete(NULL);
}



void Error_Handler(void)
{
  __disable_irq();
  printf("Error_Handler() called — halting.\r\n");
#ifdef DEBUG
  __BKPT(0);  /* trap once if a debugger is attached */
#endif
  while (1) {
    /* Optionally blink a LED here */
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  UNUSED(file);
  UNUSED(line);
  __BKPT(0);
  while (1)
  {
  }
}
#endif

/* Allow to debug with cache enable */
__attribute__ ((section (".keep_me"))) void app_clean_invalidate_dbg()
{
  SCB_CleanInvalidateDCache();
}
