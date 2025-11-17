#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void Error_Handler(void);

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line);
#endif

#ifdef __cplusplus
}
#endif

#endif // ERROR_HANDLER_H
