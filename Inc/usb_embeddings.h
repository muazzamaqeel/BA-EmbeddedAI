#ifndef USB_EMBEDDINGS_H
#define USB_EMBEDDINGS_H

#include "fatfs.h"   // ✅ defines FATFS type (sometimes "ff.h" depending on your CubeMX version)
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SD card interface for embeddings and PINs */

extern FATFS fs;   // global filesystem instance (defined in usb_embeddings.c)

bool USB_SD_IsDetected(void);
void USB_SD_PrintStatus(void);
bool USB_SD_Mount(void);
bool USB_SD_EnsureMounted(void);
void USB_SD_ListDir(const char *path);
void USB_SD_Test(void);

/* Reference set loader (optional FaceRec helper) */
void FR_LoadRefsetFromSD(void);

#ifdef __cplusplus
}
#endif

#endif /* USB_EMBEDDINGS_H */
