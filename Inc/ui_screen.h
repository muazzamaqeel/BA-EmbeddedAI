#ifndef UI_SCREEN_H
#define UI_SCREEN_H

#include "app_ui_start.h"
#include "ui_manager.h"
#include "ui_button.h"

// Init LCD + Touch + SCRL
void UI_Screen_Init(void);

// Draw / process welcome menu
AppUIChoice UI_Screen_Welcome(void);

// Cleanup
void UI_Screen_Deinit(void);

#endif // UI_SCREEN_H
