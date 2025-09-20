#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_lcd.h"
#include "app_ui_pin.h"
#include <stdio.h>
#include <string.h>
#include "app_shared.h"   // for disp
#include "FreeRTOS.h"
#include "semphr.h"
#include "cmw_camera.h"

#ifndef UTIL_LCD_COLOR_TRANSPARENT   // <-- add this
#define UTIL_LCD_COLOR_TRANSPARENT 0
#endif

/* --- Background image (keypad) in NOR Flash --- */
#define KEYPAD_ADDR 0x77E00000UL

#define KEY_W   55   // narrower
#define KEY_H   45   // shorter
#define KEY_SP  20   // a bit more vertical spacing

#define KEYPAD_ORIGIN_X  (800/2 - (3*KEY_W + 2*KEY_SP)/2) // centered horizontally
#define KEYPAD_ORIGIN_Y  170 // push grid down slightly

/* --- PIN buffer --- */
static char pin_buffer[PIN_MAX_LEN + 1];
static uint8_t pin_length = 0;



/* Stop camera + FR + live display before showing PIN */
/* Stop camera + FR + live display before showing PIN */
void UI_StopPipeline(void)
{
    printf("[UI] Stopping camera, display, and FR pipeline...\r\n");

    /* 1. Suspend camera acquisition on both pipes */
    CMW_CAMERA_Suspend(DCMIPP_PIPE1);   // display pipe
    CMW_CAMERA_Suspend(DCMIPP_PIPE2);   // NN input pipe

    /* 2. Block FR thread logic (global flag) */
    extern volatile int g_pipeline_paused;
    g_pipeline_paused = 1;

    /* 3. Reset display state so DP thread won’t draw detections */
    if (xSemaphoreTake(disp.lock, portMAX_DELAY) == pdTRUE) {
        disp.info.nb_detect = 0;
        disp.info.tracking_enabled = 0;
#ifdef TRACKER_MODULE
        disp.info.tboxes_valid_nb = 0;
#endif
        g_fr_overlay_label[0] = '\0';
        xSemaphoreGive(disp.lock);
    }

    /* 4. Clear LCD background (remove last camera frame) */
    UTIL_LCD_SetLayer(0);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

    /* 5. Clear LCD foreground (remove old overlays) */
    UTIL_LCD_SetLayer(1);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_TRANSPARENT);

    printf("[UI] Pipeline stopped, ready for PIN UI.\r\n");
}

/* --- Debug: draw rectangles where UI_KeypadHit expects buttons --- */
static void UI_DrawKeypadHitboxes(void)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
            int key_x = KEYPAD_ORIGIN_X + col * (KEY_W + KEY_SP);
            int key_y = KEYPAD_ORIGIN_Y + row * (KEY_H + KEY_SP);

            // Clamp within screen
            int w = KEY_W;
            int h = KEY_H;
            if (key_x + w > 800) w = 800 - key_x;
            if (key_y + h > 480) h = 480 - key_y;

            // Color for each row
            uint32_t color;
            switch (row) {
                case 0: color = UTIL_LCD_COLOR_RED; break;
                case 1: color = UTIL_LCD_COLOR_GREEN; break;
                case 2: color = UTIL_LCD_COLOR_BLUE; break;
                case 3: color = UTIL_LCD_COLOR_YELLOW; break;
                default: color = UTIL_LCD_COLOR_WHITE; break;
            }

            UTIL_LCD_SetTextColor(color);
            UTIL_LCD_DrawRect(key_x, key_y, w, h, color);

            printf("[DEBUG] Key rect row=%d col=%d at (%d,%d,%d,%d)\r\n",
                   row, col, key_x, key_y, w, h);
        }
    }
}



/* --- Helper: Draw keypad background --- */
static void UI_DrawBackground(void)
{
    UTIL_LCD_DrawBitmap(0, 0, (uint8_t*)KEYPAD_ADDR);
}

/* --- Helper: Draw PIN entry box --- */
static void UI_DrawPinBox(void)
{
    int box_x = 200;
    int box_y = 40;
    int box_w = 400;
    int box_h = 40;

    // Outline
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DrawRect(box_x, box_y, box_w, box_h, UTIL_LCD_COLOR_WHITE);

    // Mask entered PIN with *
    char masked[PIN_MAX_LEN + 1];
    memset(masked, '*', pin_length);
    masked[pin_length] = '\0';

    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DisplayStringAt(box_x + 10, box_y + 10, (uint8_t*)masked, LEFT_MODE);
}

/* --- Helper: Map touch to keypad digit --- */
static int UI_KeypadHit(int x, int y)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
            int key_x = KEYPAD_ORIGIN_X + col * (KEY_W + KEY_SP);
            int key_y = KEYPAD_ORIGIN_Y + row * (KEY_H + KEY_SP);

            /* Extra offset for bottom row (0 key) */
            if (row == 3) {
                key_y += 10;   // push hitbox down a little more
            }

            if (x >= key_x && x <= key_x + KEY_W &&
                y >= key_y && y <= key_y + KEY_H) {
                int digit = row * 3 + col + 1;
                if (digit == 11) return 0;    // bottom center → 0
                if (digit == 10 || digit == 12) return -1; // blanks
                return digit;
            }
        }
    }
    return -1;
}


void UI_PIN_Show(void)
{
    BSP_LCD_DisplayOn(0);
    UTIL_LCD_SetLayer(0);

    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

    // ✅ Draw the keypad background image first
    UI_DrawBackground();

    // Draw PIN box + debug hitboxes
    UI_DrawPinBox();
    UI_DrawKeypadHitboxes();

    printf("[UI] PIN screen displayed with debug hitboxes.\r\n");
}



/* --- Public: Loop for keypad --- */
void UI_PIN_Loop(void)
{
    TS_State_t ts;
    while (1) {
        if (BSP_TS_GetState(0, &ts) == BSP_ERROR_NONE) {
            if (ts.TouchDetected) {
                int digit = UI_KeypadHit(ts.TouchX, ts.TouchY);
                if (digit >= 0 && pin_length < PIN_MAX_LEN) {
                    pin_buffer[pin_length++] = '0' + digit;
                    pin_buffer[pin_length] = '\0';
                    UI_DrawBackground();
                    UI_DrawPinBox();
                    printf("[PIN] Digit pressed: %d → %s\n", digit, pin_buffer);
                }
            }
        }
        HAL_Delay(100);
    }
}




