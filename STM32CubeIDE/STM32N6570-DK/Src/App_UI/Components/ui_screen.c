#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "ui_screen.h"
#include "app_config.h"
#include "scrl.h"
#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"





#ifdef STM32N6570_DK_REV
  #include "stm32n6570_discovery.h"
  #ifndef HAS_TS
  #define HAS_TS 1
  #endif
  #if (HAS_TS==1)
    #include "stm32n6570_discovery_ts.h"
  #endif
#else
  #include "stm32n6xx_nucleo.h"
#endif

#include "ui_screen.h"

// --------------------------------------------------------------------------
// Framebuffers
// --------------------------------------------------------------------------
#ifndef ALIGN_32
#define ALIGN_32 __attribute__((aligned(32)))
#endif

#ifndef IN_PSRAM
#define IN_PSRAM __attribute__((section(".psram_bss")))
#endif

static uint8_t s_ui_bg[LCD_BG_WIDTH * LCD_BG_HEIGHT * 2] ALIGN_32 IN_PSRAM;    // RGB565
static uint8_t s_ui_fg[LCD_BG_WIDTH * LCD_BG_HEIGHT * 2] ALIGN_32 IN_PSRAM;    // ARGB4444

// --------------------------------------------------------------------------
// Touch mapping helpers
// --------------------------------------------------------------------------
#ifndef TOUCH_SWAP_XY
#define TOUCH_SWAP_XY 0
#endif
#ifndef TOUCH_INVERT_X
#define TOUCH_INVERT_X 0
#endif
#ifndef TOUCH_INVERT_Y
#define TOUCH_INVERT_Y 0
#endif

static inline int clampi(int v, int lo, int hi) { return (v<lo)?lo:((v>hi)?hi:v); }



static void ts_map_point(int *x, int *y) {
#if (TOUCH_SWAP_XY==1)
  int tx = *x; *x = *y; *y = tx;
#endif
#if (TOUCH_INVERT_X==1)
  *x = LCD_BG_WIDTH  - 1 - *x;
#endif
#if (TOUCH_INVERT_Y==1)
  *y = LCD_BG_HEIGHT - 1 - *y;
#endif
  *x = clampi(*x, 0, LCD_BG_WIDTH  - 1);
  *y = clampi(*y, 0, LCD_BG_HEIGHT - 1);
}

// --------------------------------------------------------------------------
// Commit FG layer
// --------------------------------------------------------------------------
static void ui_commit_fg(void) {
  SCRL_ReloadLayer(SCRL_LAYER_1);
  SRCL_Update();
}

// --------------------------------------------------------------------------
// Public API
// --------------------------------------------------------------------------
void UI_Screen_Init(void)
{
  SCRL_LayerConfig layers[2] = {
    { .origin = {0,0}, .size = {LCD_BG_WIDTH, LCD_BG_HEIGHT}, .format = SCRL_RGB565,   .address = s_ui_bg },
    { .origin = {0,0}, .size = {LCD_BG_WIDTH, LCD_BG_HEIGHT}, .format = SCRL_ARGB4444, .address = s_ui_fg },
  };
  SCRL_ScreenConfig screen = {
    .size    = {LCD_BG_WIDTH, LCD_BG_HEIGHT},
  #ifdef SCR_LIB_USE_SPI
    .format  = SCRL_RGB565,
  #else
    .format  = SCRL_YUV422,
  #endif
    .address = s_ui_bg,
    .fps     = 60,
  };

  memset(s_ui_bg, 0, sizeof(s_ui_bg));
  memset(s_ui_fg, 0, sizeof(s_ui_fg));

  (void)SCRL_Init((SCRL_LayerConfig *[2]){ &layers[0], &layers[1] }, &screen);

  UTIL_LCD_SetLayer(SCRL_LAYER_0);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

  UTIL_LCD_SetLayer(SCRL_LAYER_1);
  UTIL_LCD_Clear(0x00000000);
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);

#if (HAS_TS==1)
  TS_Init_t ts_cfg = {
    .Width       = LCD_BG_WIDTH,
    .Height      = LCD_BG_HEIGHT,
    .Orientation = TS_SWAP_NONE,
    .Accuracy    = 8
  };
  int tret = BSP_TS_Init(0, &ts_cfg);
  if (tret != BSP_ERROR_NONE) {
    printf("[UI][TS] BSP_TS_Init failed (%d) — fallback USER button.\r\n", tret);
  } else {
    printf("[UI][TS] OK: %ldx%ld\r\n", (long)ts_cfg.Width, (long)ts_cfg.Height);
  }
#endif
}

AppUIChoice UI_Screen_Welcome(void)
{
    UI_Init();

    UiComponent *btnStart = UI_ButtonCreate(
        40, LCD_BG_HEIGHT - 96, 200, 72,
        "Start",
        UTIL_LCD_COLOR_GREEN,
        UTIL_LCD_COLOR_WHITE,
        NULL
    );

    UiComponent *btnEnroll = UI_ButtonCreate(
        280, LCD_BG_HEIGHT - 96, 200, 72,
        "Enroll",
        UTIL_LCD_COLOR_CYAN,
        UTIL_LCD_COLOR_WHITE,
        NULL
    );

    UI_AddComponent(btnStart);
    UI_AddComponent(btnEnroll);

    UI_DrawAll();

#if (HAS_TS==1)
    int pressed_id = 0;
    int was_down   = 0;

    while (1) {
        TS_State_t st;
        if (BSP_TS_GetState(0, &st) == BSP_ERROR_NONE) {
            int down = (st.TouchDetected > 0);
            int id   = 0;

            if (down) {
                int x = st.TouchX;
                int y = st.TouchY;
                ts_map_point(&x, &y);

                if (x >= btnStart->x && x < btnStart->x+btnStart->w &&
                    y >= btnStart->y && y < btnStart->y+btnStart->h) {
                    id = 1;
                }
                else if (x >= btnEnroll->x && x < btnEnroll->x+btnEnroll->w &&
                         y >= btnEnroll->y && y < btnEnroll->y+btnEnroll->h) {
                    id = 2;
                }

                if (id != pressed_id) {
                    pressed_id = id;
                    if (id == 1) ((UiButtonData*)btnStart->userdata)->pressed = 1;
                    if (id == 2) ((UiButtonData*)btnEnroll->userdata)->pressed = 1;
                    UI_DrawAll();
                }
                was_down = 1;
            } else {
                if (was_down && pressed_id != 0) {
                    if (pressed_id == 1) {
                        ((UiButtonData*)btnStart->userdata)->pressed = 0;
                        UI_DrawAll();
                        return APPUI_START;
                    } else if (pressed_id == 2) {
                        ((UiButtonData*)btnEnroll->userdata)->pressed = 0;
                        UI_DrawAll();
                        return APPUI_ENROLL;
                    }
                }
                was_down = 0;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(8));
    }

#else
    #ifdef STM32N6570_DK_REV
        BSP_PB_Init(BUTTON_USER1, BUTTON_MODE_GPIO);
    #else
        BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
    #endif

    uint32_t press_ts = 0; int pressed = 0;
    while (1) {
        #ifdef STM32N6570_DK_REV
            int b = (BSP_PB_GetState(BUTTON_USER1) == GPIO_PIN_SET);
        #else
            int b = (BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_SET);
        #endif

        if (b && !pressed) { pressed = 1; press_ts = HAL_GetTick(); }
        else if (!b && pressed) {
            uint32_t dur = HAL_GetTick() - press_ts;
            pressed = 0;
            if (dur >= 800) return APPUI_ENROLL;
            else            return APPUI_START;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
#endif
}

void UI_Screen_Deinit(void)
{
  UTIL_LCD_SetLayer(SCRL_LAYER_1);
  UTIL_LCD_Clear(0x00000000);
  ui_commit_fg();
}
