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
#include "pipeline_start.h"
#include "app_sleepmode.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#ifndef UI_State
typedef enum {
    UI_STATE_START = 0,
    UI_STATE_PIN,
    UI_STATE_ADMIN,
    UI_STATE_CHANGE_PIN,
    UI_STATE_FACE_REC
} UI_State;
#endif

static UI_State currentState;

void UI_FSM_Init(void)
{
    currentState = UI_STATE_START;
    printf("[FSM] Initialized at START state\r\n");
}

void UI_FSM_Run(void)
{
    while (1)
    {
        switch (currentState)
        {
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
            case UI_STATE_FACE_REC:
            {
                printf("[FSM] EVENT: Launching application pipeline...\r\n");
                app_start_pipeline();
                printf("[FSM] Pipeline started. Returning from UI_FSM_Run().\r\n");
                return;
            }
            default:
                printf("[FSM] Invalid state. Resetting to START.\r\n");
                currentState = UI_STATE_START;
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void UI_FSM_Task(void *arg)
{
    (void)arg;
    UI_FSM_Run();
    vTaskDelete(NULL);
}
