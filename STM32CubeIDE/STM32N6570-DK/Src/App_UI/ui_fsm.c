/**
 ******************************************************************************
 * @file    ui_fsm.c
 * @brief   Finite State Machine for STM32 UI screens
 ******************************************************************************
 */

#include "ui_fsm.h"
#include "app_ui_start.h"
#include "app_ui_pin.h"
#include "app_ui_admin.h"
#include "app_change_pin.h"
#include "app_ui_pin_face_rec.h"
#include "pipeline_start.h"
#include "app_sleepmode.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

/* Current UI state */
static UI_State currentState;

/* Initialize FSM */
void UI_FSM_Init(void)
{
    currentState = UI_STATE_START;
    printf("[FSM] Initialized at START state\r\n");
}

/* Run FSM loop forever */
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
                    printf("[FSM] EVENT: Admin pressed → PIN\r\n");
                    currentState = UI_STATE_PIN;
                }
                break;
            }

            /* =======================================================
             *  ADMIN PIN ENTRY SCREEN
             * ======================================================= */
            case UI_STATE_PIN:
            {
                UI_PinScreen_Show();
                UI_PinScreen_WaitForOK();
                printf("[FSM] EVENT: PIN accepted → ADMIN\r\n");
                currentState = UI_STATE_ADMIN;
                break;
            }

            /* =======================================================
             *  ADMIN SCREEN
             * ======================================================= */
            case UI_STATE_ADMIN:
            {
                UI_AdminScreen_Show();
                printf("[FSM] Admin screen active\r\n");

                /* When Change PIN returns, go back to START */
                CP_Result res = UI_ChangePinScreen_Show();
                if (res == CP_RESULT_BACK_TO_START)
                {
                    printf("[FSM] EVENT: Back to START after PIN change\r\n");
                    currentState = UI_STATE_START;
                }
                else
                {
                    printf("[FSM] EVENT: Stay in ADMIN\r\n");
                }
                break;
            }

            /* =======================================================
             *  CHANGE PIN SCREEN (explicit if needed)
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
                Start_ApplicationTasks();     // your main face recognition pipeline
                APP_SleepMode_Init();         // enable sleep management
                currentState = UI_STATE_START;
                break;
            }

            /* =======================================================
             *  FALLBACK STATE
             * ======================================================= */
            default:
                printf("[FSM] Invalid state. Resetting to START.\r\n");
                currentState = UI_STATE_START;
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));  // let FreeRTOS breathe
    }
}
