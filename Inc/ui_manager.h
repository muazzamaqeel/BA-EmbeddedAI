#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <stdint.h>   // ✅ fixes uint32_t
#include <stddef.h>   // for NULL

// Forward declare for use in function prototypes
typedef struct UiComponent UiComponent;

// Base component type
typedef enum {
    UI_TYPE_BUTTON,
    UI_TYPE_LABEL,
    UI_TYPE_IMAGE
} UiType;

// Base UI component struct
struct UiComponent {
    UiType type;
    int x, y, w, h;
    void (*draw)(UiComponent *self);
    void (*onTouch)(UiComponent *self, int x, int y, int pressed);
    void *userdata;
};

// Manager API
void UI_Init(void);
void UI_AddComponent(UiComponent *c);
void UI_DrawAll(void);
void UI_DispatchTouch(int x, int y, int pressed);

#endif // UI_MANAGER_H
