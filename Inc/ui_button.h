#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <stdint.h>
#include "ui_manager.h"   // ✅ gives UiComponent definition

// Data specific to button widget
typedef struct {
    const char *label;
    uint32_t fill;
    uint32_t border;
    int pressed;
    void (*callback)(void);
} UiButtonData;

UiComponent *UI_ButtonCreate(int x, int y, int w, int h,
                             const char *label,
                             uint32_t fill,
                             uint32_t border,
                             void (*cb)(void));

#endif // UI_BUTTON_H
