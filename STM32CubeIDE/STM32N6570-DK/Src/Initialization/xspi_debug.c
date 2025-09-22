#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

/* --- XSPI debug helpers --- */
void xspi_quick_check(void) {
  volatile const uint32_t *det = (const uint32_t*)0x71000000u;  // detector base
  volatile const uint32_t *fr  = (const uint32_t*)0x71040000u;  // face rec base

  printf("[XSPI] DET@0x71000000: %08lX %08lX %08lX %08lX\r\n",
         det[0], det[1], det[2], det[3]);
  printf("[XSPI] FR @0x71040000: %08lX %08lX %08lX %08lX\r\n",
         fr[0], fr[1], fr[2], fr[3]);
}

uint32_t crc32_update(uint32_t c, uint8_t b) {
  c ^= b;
  for (int k = 0; k < 8; ++k)
    c = (c >> 1) ^ (0xEDB88320u & -(c & 1));
  return c;
}

uint32_t crc32_region(const void *base, size_t len) {
  const uint8_t *p = (const uint8_t*)base;
  uint32_t c = ~0u;
  for (size_t i = 0; i < len; ++i) c = crc32_update(c, p[i]);
  return ~c;
}

void xspi_crc_check(void) {
  const void *det = (const void*)0x71000000u;
  const void *fr  = (const void*)0x71040000u;
  const size_t span = 64 * 1024;  // first 64 KB
  uint32_t c_det = crc32_region(det, span);
  uint32_t c_fr  = crc32_region(fr , span);
  printf("[XSPI] CRC32(first 64KB): DET=0x%08lX  FR=0x%08lX\r\n",
         (unsigned long)c_det, (unsigned long)c_fr);
}
