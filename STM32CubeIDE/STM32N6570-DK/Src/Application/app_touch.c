#include "app_touch.h"
#include "stm32n6570_discovery_ts.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <assert.h>

#define LCD_BG_WIDTH   800   // <-- use your actual value
#define LCD_BG_HEIGHT  480   // <-- use your actual value

#define FREERTOS_PRIORITY(p) ((UBaseType_t)((int)tskIDLE_PRIORITY + configMAX_PRIORITIES/2 + (p)))

static void Touch_PollTask(void *arg)
{
    TS_State_t ts;
    (void)arg;

    while (1) {
        if (BSP_TS_GetState(0, &ts) == BSP_ERROR_NONE) {
            if (ts.TouchDetected) {
                printf("[TS] Touch at x=%lu, y=%lu\r\n",
                       (unsigned long)ts.TouchX,
                       (unsigned long)ts.TouchY);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // 10 Hz
    }
}

void APP_Touch_Init(void)
{
    TS_Init_t ts_cfg;
    ts_cfg.Width       = LCD_BG_WIDTH;
    ts_cfg.Height      = LCD_BG_HEIGHT;
    ts_cfg.Orientation = TS_SWAP_NONE;
    ts_cfg.Accuracy    = 5;

    int st = BSP_TS_Init(0, &ts_cfg);
    if (st != BSP_ERROR_NONE) {
        printf("[TS][ERR] BSP_TS_Init failed (%d)\n", st);
        return;
    }

    static StaticTask_t ts_task;
    static StackType_t ts_stack[configMINIMAL_STACK_SIZE];

    TaskHandle_t h = xTaskCreateStatic(
        Touch_PollTask, "ts",
        configMINIMAL_STACK_SIZE, NULL,
        FREERTOS_PRIORITY(-1),
        ts_stack, &ts_task
    );
    assert(h != NULL);

    printf("[TS] Touchscreen initialized\n");
}
