#include "app_ui_start.h"
#include "ui_screen.h"

void AppUI_InitStandalone(void)
{
    UI_Screen_Init();
}

AppUIChoice AppUI_WelcomeMenu(void)
{
    return UI_Screen_Welcome();
}

void AppUI_Deinit(void)
{
    UI_Screen_Deinit();
}
