#include "error_handler.h"
#include <stdio.h>
#include "main.h"

void Error_Handler(void)
{
  __disable_irq();
  printf("Error_Handler() called — halting.\r\n");
#ifdef DEBUG
  __BKPT(0);
#endif
  while (1) {
  }
}

#ifdef  USE_FULL_ASSERT

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
