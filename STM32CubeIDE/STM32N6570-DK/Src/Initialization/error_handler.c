#include "error_handler.h"
#include <stdio.h>
#include "main.h"    // for DEBUG, __BKPT

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
