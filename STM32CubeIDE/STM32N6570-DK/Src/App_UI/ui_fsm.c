/**
 ******************************************************************************
 * @file    ui_fsm.c
 * @brief   Finite State Machine for STM32 UI screens (FreeRTOS-friendly)
 ******************************************************************************
 */

#include "ui_fsm.h"
#include "app_ui_start.h"
#include "app_ui_pin.h"
#include "app_ui_admin.h"
#include "app_change_pin.h"
#include "pipeline_start.h"     // app_start_pipeline()
#include "app_sleepmode.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

/* --- If these enums/typedefs are elsewhere, include the right header instead --- */
#ifndef UI_State
typedef enum {
    UI_STATE_START = 0,
    UI_STATE_PIN,
    UI_STATE_ADMIN,
    UI_STATE_CHANGE_PIN,
    UI_STATE_FACE_REC
} UI_State;
#endif

/* Current UI state */
static UI_State currentState;

/* Initialize FSM */
void UI_FSM_Init(void)
{
    currentState = UI_STATE_START;
    printf("[FSM] Initialized at START state\r\n");
}

/* Blocking runner: when Start is pressed and pipeline is launched, this RETURNS. */
void UI_FSM_Run(void)
{
    while (1)
    {
        switch (currentState)
        {
            /* =======================================================
             *  HOME / START SCREEN
             * ======================================================= */
            case UI_STATE_START:
            {
                UI_StartScreen_Show();
                UI_ButtonResult btn = UI_WaitForButton();

                if (btn == UI_BTN_START)
                {
                    printf("[FSM] EVENT: Start pressed → FACE_REC\r\n");
                    currentState = UI_STATE_FACE_REC;
                }
                else if (btn == UI_BTN_ADMIN)
                {
                    printf("[FSM] EVENT: Admin pressed → ADMIN\r\n");
                    currentState = UI_STATE_ADMIN;
                }
                break;
            }

            /* =======================================================
             *  ADMIN SCREEN (handles its own sub-screens like Change PIN)
             * ======================================================= */
            case UI_STATE_ADMIN:
            {
                printf("[FSM] Admin screen opening...\r\n");
                AdminResult adminRes = UI_AdminScreen_Show();
                if (adminRes == ADMIN_RESULT_BACK_TO_START)
                {
                    printf("[FSM] EVENT: Admin → START\r\n");
                    currentState = UI_STATE_START;
                }
                else
                {
                    printf("[FSM] EVENT: Stay in ADMIN\r\n");
                }
                break;
            }

            /* =======================================================
             *  (Optional) CHANGE PIN explicit state if you need it
             * ======================================================= */
            case UI_STATE_CHANGE_PIN:
            {
                CP_Result res = UI_ChangePinScreen_Show();
                if (res == CP_RESULT_BACK_TO_START)
                {
                    printf("[FSM] EVENT: PIN changed → START\r\n");
                    currentState = UI_STATE_START;
                }
                break;
            }

            /* =======================================================
             *  FACE RECOGNITION / MAIN PIPELINE
             * ======================================================= */
            case UI_STATE_FACE_REC:
            {
                printf("[FSM] EVENT: Launching application pipeline...\r\n");
                app_start_pipeline();   // starts camera/NN/display tasks

                /* IMPORTANT:
                   If UI_FSM_Run() is called from main_thread_fct(), do NOT delete the task here.
                   Simply return so main_thread_fct() can cleanly end (vTaskDelete(NULL) there). */
                printf("[FSM] Pipeline started. Returning from UI_FSM_Run().\r\n");
                return;  // <-- key change
            }

            /* =======================================================
             *  FALLBACK
             * ======================================================= */
            default:
                printf("[FSM] Invalid state. Resetting to START.\r\n");
                currentState = UI_STATE_START;
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));  // let FreeRTOS breathe
    }
}

/* Optional FreeRTOS task wrapper (only if you ever want a separate UI task) */
void UI_FSM_Task(void *arg)
{
    (void)arg;
    UI_FSM_Run();   // blocks until pipeline starts
    vTaskDelete(NULL);  // safe here because this is a dedicated UI task
}
