#ifndef APP_UI_ADMIN_H
#define APP_UI_ADMIN_H

#include "stm32n6570_discovery_ts.h"
#include "app_change_pin.h"
#include <stdbool.h>
#include <stdint.h>
typedef enum {
    ADMIN_RESULT_STAY,
    ADMIN_RESULT_BACK_TO_START
} AdminResult;

AdminResult UI_AdminScreen_Show(void);
extern bool g_pipeline_running;
extern bool g_fr_active;
#endif
