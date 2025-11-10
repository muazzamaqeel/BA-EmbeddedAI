#ifndef USB_EMBEDDINGS_H
#define USB_EMBEDDINGS_H

#include "fatfs.h"   // ✅ defines FATFS type (sometimes "ff.h" depending on your CubeMX version)
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Global FatFs handle (defined in usb_embeddings.c) */
extern FATFS fs;

/* SD Card helper functions */
bool USB_SD_IsDetected(void);
void USB_SD_PrintStatus(void);
bool USB_SD_Mount(void);
void USB_SD_ListDir(const char *path);
void USB_SD_Test(void);

/* Reference set loader (optional FaceRec helper) */
void FR_LoadRefsetFromSD(void);

#ifdef __cplusplus
}
#endif

#endif /* USB_EMBEDDINGS_H */
