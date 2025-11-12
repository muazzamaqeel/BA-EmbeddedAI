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
#include "app_tasks.h"
#include "xspi_nor_init.h"
#include "error_handler.h"
#include "system_display.h"
#include "app_sleepmode.h"
#include "usb_embeddings.h"
#include "app_ui_admin.h"
#include "ui_fsm.h"
#include "fatfs.h"                      // declares MX_FATFS_Init()
#include "stm32n6570_discovery_sd.h"    // declares BSP_SD_Init(...)  (name may be *_sd.h or *_mmc.h on your BSP)

UART_HandleTypeDef huart1;

static StaticTask_t main_thread;
static StackType_t main_thread_stack[configMINIMAL_STACK_SIZE];
static int main_freertos(void);
static void main_thread_fct(void *arg);
void Error_Handler(void);
void vPortSetupTimerInterrupt(void);


int main(void)
{
	MEMSYSCTL->MSCR |= MEMSYSCTL_MSCR_ICACTIVE_Msk; /* Power on ICACHE */
  __HAL_RCC_CPUCLK_CONFIG(RCC_CPUCLKSOURCE_HSI);	/* Set back system and CPU clock source to HSI */
  __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_HSI);
  HAL_Init();
  SCB_EnableICache();
#if defined(USE_DCACHE)
  MEMSYSCTL->MSCR |= MEMSYSCTL_MSCR_DCACTIVE_Msk;	/* Power on DCACHE */
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
  SystemClock_Config();	/* System clock uses HAL_Delay() → call after scheduler starts */
  vPortSetupTimerInterrupt();
  CONSOLE_Config(); /* UART for logs */
  /* NPU SRAMs + caches */
  NPURam_enable();
  Fuse_Programming();
  NPUCache_config();

#ifdef STM32N6570_DK_REV
/* ---- External RAM: HyperRAM on XSPI1 ---- and NOR on XSPIx ---- */
if (HyperRAM_Map_Once() != 0 || XSPI_NOR_Map_Once() != 0) {
    Error_Handler();
}
#else
if (XSPI_NOR_Map_Once() != 0) {
    Error_Handler();
}
#endif

  Security_Config();
  IAC_Config();
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

  if (System_DisplayAndTouch_Init() != 0) {
      Error_Handler();
  }


  /* ------------------------------------------------------------
   * SD Card / Embeddings Initialization
   * ------------------------------------------------------------ */
  printf("\r\n[MAIN] Initializing SD card...\r\n");

  BSP_SD_Init(0);      // low-level SD init
  MX_FATFS_Init();     // initialize FatFs stack
  USB_SD_Test();       // mount + read embeddings files
  printf("[MAIN] SD card check completed.\r\n\r\n");
  printf("[MAIN] Starting Refset Loader Task...\r\n");
  FR_StartRefsetLoader();   // <-- add this line
  /* ======================================================
   * MAIN UI via FSM (no extra task)
   * ====================================================== */
  printf("[MAIN] Initializing base application...\r\n");
  app_run();                 // init your app (no threads yet)
  APP_SleepMode_Init();      // prepare sleep system

  printf("[MAIN] Running UI FSM (blocking until pipeline starts)...\r\n");
  UI_FSM_Init();
  UI_FSM_Run();              // blocks in UI loop; returns after Start → pipeline launched

  /* Once the FSM returns, pipeline tasks are running; end this thread */
  vTaskDelete(NULL);

}


__attribute__ ((section (".keep_me"))) void app_clean_invalidate_dbg()
{
  SCB_CleanInvalidateDCache();
}
