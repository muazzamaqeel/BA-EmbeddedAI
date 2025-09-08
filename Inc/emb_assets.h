#ifndef EMB_ASSETS_H
#define EMB_ASSETS_H

#include <stdint.h>

typedef struct {
    const char *name;         /* filename or label */
    const uint8_t *data;      /* raw BMP bytes */
    uint32_t size;            /* BMP file size */
} EmbeddedBMP;

/* Auto-generated tables (you must define in .c) */
extern const EmbeddedBMP g_user_bmps[];
extern const int g_user_bmps_count;

#endif /* EMB_ASSETS_H */
