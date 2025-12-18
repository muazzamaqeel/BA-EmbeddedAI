#ifndef PROFILER_H
#define PROFILER_H

#include <stdint.h>
#include "stm32n6xx_hal.h"

/* ---------------- CPU frequency ---------------- */
#ifndef CPU_FREQ_HZ
#define CPU_FREQ_HZ 800000000UL   // adjust if needed
#endif

/* ---------------- DWT init ---------------- */
static inline void PROF_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/* ---------------- cycle helpers ---------------- */
static inline uint32_t PROF_CycleNow(void)
{
    return DWT->CYCCNT;
}

static inline uint32_t PROF_CycleDiff(uint32_t start, uint32_t end)
{
    return (uint32_t)(end - start);
}

/* ---------------- conversions ---------------- */
static inline float PROF_CyclesToMs(uint32_t cycles)
{
    return (float)cycles * 1000.0f / (float)CPU_FREQ_HZ;
}

static inline float PROF_CyclesToUs(uint32_t cycles)
{
    return (float)cycles * 1e6f / (float)CPU_FREQ_HZ;
}

#endif /* PROFILER_H */
