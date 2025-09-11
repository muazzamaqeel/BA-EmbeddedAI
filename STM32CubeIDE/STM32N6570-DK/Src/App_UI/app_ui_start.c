// Src/App_UI/app_ui_start.c
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"

#include "app_config.h"      // LCD_BG_WIDTH/HEIGHT
#include "scrl.h"
#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"

#ifdef STM32N6570_DK_REV
  #include "stm32n6570_discovery.h"
  // Enable touch path at build time (or define in app_config.h)
  #ifndef HAS_TS
  #define HAS_TS 1
  #endif
  #if (HAS_TS==1)
    #include "stm32n6570_discovery_ts.h"
  #endif
#else
  #include "stm32n6xx_nucleo.h"
#endif

#include "app_ui_start.h"

/* -------------------------------------------------------------------------- */
/* Portable placement/align macros                                            */
/* -------------------------------------------------------------------------- */
#ifndef ALIGN_32
#define ALIGN_32 __attribute__((aligned(32)))
#endif

/* Your linker maps .psram_bss -> PSRAM (0x9100_0000..), NOLOAD */
#ifndef IN_PSRAM
#define IN_PSRAM __attribute__((section(".psram_bss")))
#endif

/* -------------------------------------------------------------------------- */
/* Touch orientation mapping knobs (adjust if hits feel mirrored/rotated)     */
/* -------------------------------------------------------------------------- */
#ifndef TOUCH_SWAP_XY
#define TOUCH_SWAP_XY 0
#endif
#ifndef TOUCH_INVERT_X
#define TOUCH_INVERT_X 0
#endif
#ifndef TOUCH_INVERT_Y
#define TOUCH_INVERT_Y 0
#endif

/* -------------------------------------------------------------------------- */
/* Private UI framebuffers (external PSRAM)                                   */
/* -------------------------------------------------------------------------- */
static uint8_t s_ui_bg[LCD_BG_WIDTH * LCD_BG_HEIGHT * 2] ALIGN_32 IN_PSRAM;    // RGB565
static uint8_t s_ui_fg[LCD_BG_WIDTH * LCD_BG_HEIGHT * 2] ALIGN_32 IN_PSRAM;    // ARGB4444 (2Bpp)

/* -------------------------------------------------------------------------- */
/* Geometry helpers                                                           */
/* -------------------------------------------------------------------------- */
typedef struct { int x, y, w, h; } UiRect;

static inline int clampi(int v, int lo, int hi)           { return (v<lo)?lo:((v>hi)?hi:v); }
static inline int pt_in_rect(int x, int y, UiRect r)      { return (x>=r.x && x<r.x+r.w && y>=r.y && y<r.y+r.h); }
static inline int within_screen(int x, int y)             { return (x>=0 && x<LCD_BG_WIDTH && y>=0 && y<LCD_BG_HEIGHT); }

static void ts_map_point(int *x, int *y)
{
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

/* Slight shade tweak without a full color module (RGB565 and ARGB4444 safe) */
static uint32_t lighten(uint32_t c, int delta) { (void)delta; return c; }  // keep it simple on RGB565
static uint32_t darken (uint32_t c, int delta) { (void)delta; return c; }

/* -------------------------------------------------------------------------- */
/* Drawing helpers                                                            */
/* -------------------------------------------------------------------------- */
static void ui_draw_button(UiRect r, uint32_t fill, uint32_t border, const char *label, int pressed)
{
  /* Keep in-bounds even if margins are mis-set */
  r.x = clampi(r.x, 0, LCD_BG_WIDTH  - 1);
  r.y = clampi(r.y, 0, LCD_BG_HEIGHT - 1);
  r.w = clampi(r.w, 1, LCD_BG_WIDTH  - r.x);
  r.h = clampi(r.h, 1, LCD_BG_HEIGHT - r.y);

  uint32_t f = pressed ? darken(fill, 15) : fill;
  uint32_t b = pressed ? UTIL_LCD_COLOR_YELLOW : border;

  UTIL_LCD_FillRect(r.x, r.y, r.w, r.h, f);
  UTIL_LCD_DrawRect(r.x, r.y, r.w, r.h, b);

  /* Text centered; adjust baseline a bit for nicer vertical centering */
  int tx = r.x + r.w/2;
  int ty = r.y + r.h/2 - 10;
  UTIL_LCDEx_PrintfAt(tx, ty, CENTER_MODE, "%s", label);
}

static void ui_commit_fg(void)
{
  /* Push only when contents changed → reduces flicker */
  SCRL_ReloadLayer(SCRL_LAYER_1);
  SRCL_Update();
}

/* -------------------------------------------------------------------------- */
/* Screen                                                                     */
/* -------------------------------------------------------------------------- */
static void ui_layout(UiRect *btnStart, UiRect *btnEnroll)
{
  const int margin = 24;
  const int btnW   = (LCD_BG_WIDTH - 3*margin) / 2;
  const int btnH   = 72;  /* slightly taller for touch comfort */
  const int y      = LCD_BG_HEIGHT - btnH - margin;

  btnStart->x  = margin;
  btnStart->y  = y;
  btnStart->w  = btnW;
  btnStart->h  = btnH;

  btnEnroll->x = margin*2 + btnW;
  btnEnroll->y = y;
  btnEnroll->w = btnW;
  btnEnroll->h = btnH;
}

static void ui_draw_welcome(const UiRect *btnStart, const UiRect *btnEnroll, int sel /*0 none,1 start,2 enroll*/)
{
  UTIL_LCD_SetLayer(SCRL_LAYER_1);
  UTIL_LCD_Clear(0x00000000);  // full transparent FG

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCDEx_PrintfAt(LCD_BG_WIDTH/2, 36, CENTER_MODE, "Welcome");

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GRAY);
  UTIL_LCDEx_PrintfAt(LCD_BG_WIDTH/2, 68, CENTER_MODE, "Select an option to continue");

  /* Buttons */
  ui_draw_button(*btnStart , UTIL_LCD_COLOR_GREEN, UTIL_LCD_COLOR_WHITE, "Start" , (sel==1));
  ui_draw_button(*btnEnroll, UTIL_LCD_COLOR_CYAN , UTIL_LCD_COLOR_WHITE, "Enroll", (sel==2));

  ui_commit_fg();
}

/* -------------------------------------------------------------------------- */
/* Public API                                                                 */
/* -------------------------------------------------------------------------- */
void AppUI_InitStandalone(void)
{
  /* Layers config */
  SCRL_LayerConfig layers[2] = {
    { .origin = {0,0}, .size = {LCD_BG_WIDTH, LCD_BG_HEIGHT}, .format = SCRL_RGB565,   .address = s_ui_bg },
    { .origin = {0,0}, .size = {LCD_BG_WIDTH, LCD_BG_HEIGHT}, .format = SCRL_ARGB4444, .address = s_ui_fg },
  };
  SCRL_ScreenConfig screen = {
    .size    = {LCD_BG_WIDTH, LCD_BG_HEIGHT},
  #ifdef SCR_LIB_USE_SPI
    .format  = SCRL_RGB565,
  #else
    .format  = SCRL_YUV422,   /* matches your main app’s Display_init to avoid flicker */
  #endif
    .address = s_ui_bg,
    .fps     = 60,            /* steady refresh; we only reload FG on change */
  };

  memset(s_ui_bg, 0, sizeof(s_ui_bg));
  memset(s_ui_fg, 0, sizeof(s_ui_fg));

  /* Init composer */
  (void)SCRL_Init((SCRL_LayerConfig *[2]){ &layers[0], &layers[1] }, &screen);

  /* Background (solid black) */
  UTIL_LCD_SetLayer(SCRL_LAYER_0);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

  /* Foreground for UI */
  UTIL_LCD_SetLayer(SCRL_LAYER_1);
  UTIL_LCD_Clear(0x00000000);
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);

#if (HAS_TS==1)
  /* Touch init after LCD/LTDC are up */
  TS_Init_t ts_cfg = {
    .Width       = LCD_BG_WIDTH,
    .Height      = LCD_BG_HEIGHT,
    .Orientation = TS_SWAP_NONE,  /* we do extra mapping in ts_map_point() */
    .Accuracy    = 8
  };
  int tret = BSP_TS_Init(0, &ts_cfg);
  if (tret != BSP_ERROR_NONE) {
    printf("[UI][TS] BSP_TS_Init failed (%d) — falling back to USER button.\r\n", tret);
  } else {
    printf("[UI][TS] OK: %dx%d\r\n", ts_cfg.Width, ts_cfg.Height);
  }
#endif
}

AppUIChoice AppUI_WelcomeMenu(void)
{
  UiRect btnStart, btnEnroll;
  ui_layout(&btnStart, &btnEnroll);

  /* One-time draw, no blinking animation */
  ui_draw_welcome(&btnStart, &btnEnroll, 0);

#if (HAS_TS==1)
  /* Touch-driven selection: press→highlight; release→confirm */
  int pressed_id = 0;    /* 0 none, 1 start, 2 enroll */
  int was_down   = 0;

  while (1) {
    TS_State_t st;
    int got = BSP_TS_GetState(0, &st);
    if (got == BSP_ERROR_NONE) {
      int down = (st.TouchDetected > 0);

      if (down) {
        /* On some BSPs TouchX/Y are scalars; on others arrays[0]. You have scalars. */
        int x = st.TouchX;
        int y = st.TouchY;
        if (!within_screen(x,y)) {
          /* Some FW reports raw; map them */
          ts_map_point(&x, &y);
        }

        int id = 0;
        if (pt_in_rect(x, y, btnStart))  id = 1;
        else if (pt_in_rect(x, y, btnEnroll)) id = 2;

        if (id != pressed_id) {
          pressed_id = id;
          ui_draw_welcome(&btnStart, &btnEnroll, pressed_id);
        }
        was_down = 1;
      } else {
        if (was_down && pressed_id != 0) {
          /* Finger released while a button was pressed → selection confirmed */
          int ret = pressed_id;
          /* Visual release */
          ui_draw_welcome(&btnStart, &btnEnroll, 0);
          return (ret == 1) ? APPUI_START : APPUI_ENROLL;
        }
        was_down = 0;
        /* keep idle UI (no flicker) */
      }
    }
    vTaskDelay(pdMS_TO_TICKS(8));
  }

#else
  /* Hardware-button fallback: short press to pick Start / long press to pick Enroll */
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

    if (b && !pressed) { pressed = 1; press_ts = HAL_GetTick(); ui_draw_welcome(&btnStart,&btnEnroll,1); }
    else if (!b && pressed) {
      uint32_t dur = HAL_GetTick() - press_ts;
      pressed = 0;
      ui_draw_welcome(&btnStart,&btnEnroll,0);
      if (dur >= 800) return APPUI_ENROLL;  /* long press */
      else            return APPUI_START;   /* short press */
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
#endif
}

void AppUI_Deinit(void)
{
  UTIL_LCD_SetLayer(SCRL_LAYER_1);
  UTIL_LCD_Clear(0x00000000);
  ui_commit_fg();
}
