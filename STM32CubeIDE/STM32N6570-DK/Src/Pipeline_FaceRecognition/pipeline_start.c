/**
 * @file pipeline_start.c
 * @brief Boot pipeline entrypoint: run detector, then one-shot FR embedding extraction
 */

#include "pipeline_start.h"
#include "app.h"
#include <stdio.h>
#include "stm32n6xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

int FR_Rebuild_UserFace_Embeddings(void);

void Pipeline_Start(void)
{

	FR_Refset_Summary();
    (void)FR_ExtractEmbeddings_FromAssets();
    vTaskDelay(pdMS_TO_TICKS(1000));
}

