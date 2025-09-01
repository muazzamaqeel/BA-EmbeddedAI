#include "npu_guard.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stm32n6xx_hal.h"
#include <assert.h>
#include <stdio.h>

static SemaphoreHandle_t npu_mutex = NULL;
static StaticSemaphore_t npu_mutex_buf;

void npu_guard_init(void)
{
  npu_mutex = xSemaphoreCreateMutexStatic(&npu_mutex_buf);
  assert(npu_mutex);
}

void NPU_Lock(const char *tag)
{
  int r = xSemaphoreTake(npu_mutex, portMAX_DELAY);
  assert(r == pdTRUE);
  printf("[NPU] LOCK   by %-3s @%lums\r\n", tag, (unsigned long)HAL_GetTick());
}

void NPU_Unlock(const char *tag)
{
  int r = xSemaphoreGive(npu_mutex);
  assert(r == pdTRUE);
  printf("[NPU] UNLOCK by %-3s @%lums\r\n", tag, (unsigned long)HAL_GetTick());
}
