#include <stdlib.h>
#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "ui_manager.h"
#include "ui_button.h"


static void button_draw(UiComponent *c) {
    UiButtonData *d = (UiButtonData*)c->userdata;
    uint32_t color = d->pressed ? UTIL_LCD_COLOR_YELLOW : d->fill;

    UTIL_LCD_FillRect(c->x, c->y, c->w, c->h, color);
    UTIL_LCD_DrawRect(c->x, c->y, c->w, c->h, d->border);
    UTIL_LCDEx_PrintfAt(c->x + c->w/2, c->y + c->h/2 - 10,
                        CENTER_MODE, "%s", d->label);
}

static void button_touch(UiComponent *c, int x, int y, int pressed) {
    UiButtonData *d = (UiButtonData*)c->userdata;
    d->pressed = pressed;
    if (!pressed && d->callback) {
        d->callback();
    }
}

UiComponent *UI_ButtonCreate(int x, int y, int w, int h,
                             const char *label,
                             uint32_t fill,
                             uint32_t border,
                             void (*cb)(void)) {
    UiComponent *c = malloc(sizeof(UiComponent));
    UiButtonData *d = malloc(sizeof(UiButtonData));

    d->label = label;
    d->fill = fill;
    d->border = border;
    d->pressed = 0;
    d->callback = cb;

    c->type = UI_TYPE_BUTTON;
    c->x = x; c->y = y; c->w = w; c->h = h;
    c->draw = button_draw;
    c->onTouch = button_touch;
    c->userdata = d;

    return c;
}
