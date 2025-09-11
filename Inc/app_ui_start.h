// app_ui_start.h
#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  APPUI_NONE = 0,
  APPUI_START,
  APPUI_ENROLL
} AppUIChoice;

/* Initialize a minimal LCD stack (own buffers, independent of app.c) */
void AppUI_InitStandalone(void);

/* Draw the Welcome screen and wait for a selection (touch or fallback buttons) */
AppUIChoice AppUI_WelcomeMenu(void);

/* Optional cleanup (kept simple here) */
void AppUI_Deinit(void);

#ifdef __cplusplus
}
#endif
