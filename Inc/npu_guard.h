#ifndef NPU_GUARD_H
#define NPU_GUARD_H

#include <stdint.h>

void npu_guard_init(void);
void NPU_Lock(const char *tag);
void NPU_Unlock(const char *tag);

/* Short tags for logs */
#define TAG_NN "NN"
#define TAG_FR "FR"

#endif /* NPU_GUARD_H */
