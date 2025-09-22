/**
 * @file pipeline_start.c
 * @brief Boot pipeline entrypoint: run detector, then one-shot FR embedding extraction
 */

#include "pipeline_start.h"
#include "app.h"
#include <stdio.h>
#include "stm32n6xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"              // for vTaskDelay, pdMS_TO_TICKS

int FR_Rebuild_UserFace_Embeddings(void);

void Pipeline_Start(void)
{
    /* 0) (Already done in main_thread_fct): clocks, caches, HyperRAM, NOR mmap, NPURAM, etc. */
    /* 1) Boot-time embeddings from your pictures (prints to UART) */
	FR_Refset_Summary();
    (void)FR_ExtractEmbeddings_FromAssets();
    /* 2) Start normal app pipeline (camera + detector + pp + display) */
    app_run();
    /* 3) Optional: small delay to let the pipeline settle */
    vTaskDelay(pdMS_TO_TICKS(1000));
}
