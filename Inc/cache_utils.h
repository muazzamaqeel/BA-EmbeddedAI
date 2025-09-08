#pragma once
#include <stddef.h>
#include <stdint.h>
#include "stm32n6xx_hal.h"

static inline void dcache_align_range(void **addr, size_t *len)
{
  uintptr_t a = (uintptr_t)(*addr);
  uintptr_t a0 = a & ~((uintptr_t)31);                 // align down
  size_t extra = (size_t)(a - a0);
  size_t l0 = *len + extra;
  l0 = (l0 + 31U) & ~31U;                              // align up
  *addr = (void *)a0;
  *len  = l0;
}

static inline void DCACHE_Invalidate(void *addr, size_t len)
{
#if defined(USE_DCACHE)
  dcache_align_range(&addr, &len);
  SCB_InvalidateDCache_by_Addr(addr, len);
#else
  (void)addr; (void)len;
#endif
}

static inline void DCACHE_Clean(void *addr, size_t len)
{
#if defined(USE_DCACHE)
  dcache_align_range(&addr, &len);
  SCB_CleanDCache_by_Addr(addr, len);
#else
  (void)addr; (void)len;
#endif
}
