#include "ui_manager.h"
#include "stm32_lcd.h"
#include "scrl.h"

#define MAX_COMPONENTS 16
static UiComponent *g_components[MAX_COMPONENTS];
static int g_count = 0;

void UI_Init(void) {
    g_count = 0;
}

void UI_AddComponent(UiComponent *c) {
    if (g_count < MAX_COMPONENTS) {
        g_components[g_count++] = c;
    }
}

void UI_DrawAll(void) {
    UTIL_LCD_SetLayer(SCRL_LAYER_1);
    UTIL_LCD_Clear(0x00000000);

    for (int i = 0; i < g_count; i++) {
        if (g_components[i] && g_components[i]->draw) {
            g_components[i]->draw(g_components[i]);
        }
    }

    SCRL_ReloadLayer(SCRL_LAYER_1);
    SRCL_Update();
}

void UI_DispatchTouch(int x, int y, int pressed) {
    for (int i = 0; i < g_count; i++) {
        UiComponent *c = g_components[i];
        if (c && c->onTouch) {
            if (x >= c->x && x < c->x+c->w &&
                y >= c->y && y < c->y+c->h) {
                c->onTouch(c, x, y, pressed);
            }
        }
    }
}
