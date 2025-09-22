
#include "system_display.h"
#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery_lcd.h"
#include "stm32n6570_discovery_ts.h"
#include <stdio.h>
#include "error_handler.h"

int System_DisplayAndTouch_Init(void)
{
/* ---- LCD / GUI Start ---- */
  printf("[APP] Initializing_LCD...\r\n");
  if (BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE) != BSP_ERROR_NONE) {
      printf("[ERROR] LCD init_failed!\r\n");
      Error_Handler();
  }
  /* Link UTIL to BSP driver */
  UTIL_LCD_SetFuncDriver(&LCD_Driver);
  UTIL_LCD_SetLayer(0);
  BSP_LCD_DisplayOn(0);
  /* ---- Touchscreen_Init ---- */
  TS_Init_t hTS;
  hTS.Width  = 800;   // LCD X resolution
  hTS.Height = 480;   // LCD Y resolution
  hTS.Orientation = TS_SWAP_NONE;  // adjust if axes swapped
  hTS.Accuracy = 5;   // pixels
  if (BSP_TS_Init(0, &hTS) != BSP_ERROR_NONE) {
      printf("[ERROR] TS init_failed!\r\n");
      Error_Handler();
  }
  printf("[APP] Touchscreen_initialized\r\n");
  return 0;
}
