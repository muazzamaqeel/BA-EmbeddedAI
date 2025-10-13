#ifndef USB_EMBEDDINGS_H
#define USB_EMBEDDINGS_H

#include <stdbool.h>

bool USB_SD_IsDetected(void);
void USB_SD_PrintStatus(void);
bool USB_SD_Mount(void);
void USB_SD_ReadEmbeddings(const char *filename);
void USB_SD_Test(void);

#endif /* USB_EMBEDDINGS_H */
