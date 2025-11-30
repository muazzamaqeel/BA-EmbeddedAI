#ifndef APP_UI_ADMIN_H
#define APP_UI_ADMIN_H

#include "stm32n6570_discovery_ts.h"
#include "app_change_pin.h"
#include <stdbool.h>
#include <stdint.h>

#define ADMIN_BG_ADDR   ((uint32_t)0x778A0000)

#define BTN1_X   200
#define BTN1_Y   300
#define BTN1_W   150
#define BTN1_H   80

#define BTN2_X   450
#define BTN2_Y   300
#define BTN2_W   150
#define BTN2_H   80


typedef enum {
    ADMIN_RESULT_STAY,
    ADMIN_RESULT_BACK_TO_START
} AdminResult;

AdminResult UI_AdminScreen_Show(void);
void UI_Admin_DrawBackground(void);
extern bool g_pipeline_running;
extern bool g_fr_active;
#endif
