#include "pipeline_start.h"
#include "app.h"
#include "app_ui_start.h"     // <— add this
#include <stdio.h>
#include "stm32n6xx_hal.h"

void Pipeline_Start(void)
{
    /* Minimal UI first */
    AppUI_InitStandalone();
    AppUIChoice choice = AppUI_WelcomeMenu();
    AppUI_Deinit();

    /* (Optional) react to choice — e.g., branch into enroll workflow */
    if (choice == APPUI_ENROLL) {
        printf("[UI] ENROLL selected\r\n");
        // TODO: jump to your enrollment flow here (capture + store embeddings)
        // return;  // if you don’t want to start normal app immediately
    } else {
        printf("[UI] START selected\r\n");
    }

    /* As you had: boot-time reference summary + extraction */
    FR_Refset_Summary();
    (void)FR_ExtractEmbeddings_FromAssets();

    /* Start normal app pipeline (camera + detector + display) */
    app_run();

    HAL_Delay(1000);
}
