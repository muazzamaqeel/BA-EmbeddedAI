/**
 ******************************************************************************
 * @file    app_change_face_controller.c
 * @brief   Face management screen — list users, select and delete .bin files
 ******************************************************************************
 */

#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery_lcd.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_sd.h"
#include "app_sleepmode.h"
#include "app_change_face_model.h"
#include "app_ui_start.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "app_change_face.h"

extern FATFS g_fs;
extern char g_usernames[MAX_USERS][MAX_NAME_LEN];
extern bool g_selected[MAX_USERS];
extern int  g_user_count;


void UI_TestPassed_Show_Controller(void)
{
    APP_SleepMode_Disable();
    printf("[UI] Entering Face Management screen...\r\n");

    /* -----------------------------
     * SD missing or mount fails
     * MUST re-enable sleep before exit
     * -----------------------------
     */
    if (!EnsureSDPresent()) {
        APP_SleepMode_Enable();
        APP_SleepMode_ResetState();
        return;
    }
    if (!EnsureSDMounted_Model()) {
        APP_SleepMode_Enable();
        APP_SleepMode_ResetState();
        return;
    }

    ReadUserListFromSD_Model();
    DrawFaceTable();

    TS_State_t ts;
    bool touch_active = false;

    while (1)
    {
        if (BSP_TS_GetState(0, &ts) == BSP_ERROR_NONE)
        {
            if (ts.TouchDetected && !touch_active)
            {
                touch_active = true;
                uint16_t tx = ts.TouchX;
                uint16_t ty = ts.TouchY;

                ToggleSelection_Controller(tx, ty);

                if (ty >= BUTTON_Y && ty <= BUTTON_Y + BUTTON_H)
                {
                    /* DELETE */
                    if (tx >= 120 && tx <= 120 + BUTTON_W)
                    {
                        printf("[UI] DELETE SELECTED pressed\r\n");
                        DeleteSelectedUsers_Controller();
                    }
                    /* BACK */
                    else if (tx >= 120 + BUTTON_W + BUTTON_GAP &&
                             tx <= 120 + BUTTON_W + BUTTON_GAP + BUTTON_W)
                    {
                        printf("[UI] BACK pressed → returning to caller\r\n");

                        UTIL_LCD_SetLayer(1);
                        UTIL_LCD_Clear(UTIL_LCD_COLOR_TRANSPARENT);
                        BSP_LCD_SetLayerVisible(0, 1, ENABLE);
                        BSP_LCD_Reload(0, BSP_LCD_RELOAD_IMMEDIATE);
                        APP_SleepMode_Enable();
                        APP_SleepMode_ResetState();

                        return;
                    }
                }
            }
            else if (!ts.TouchDetected)
            {
                touch_active = false;
            }
        }
        HAL_Delay(80);
    }
}


void ToggleSelection_Controller(uint16_t x, uint16_t y)
{
    for (int i = 0; i < g_user_count; i++)
    {
        int row_y = TABLE_TOP_Y + i * (ROW_HEIGHT + 2);
        if (y >= row_y && y <= row_y + ROW_HEIGHT)
        {
            g_selected[i] = !g_selected[i];
            printf("[UI] %s %s\r\n", g_selected[i] ? "Selected" : "Deselected", g_usernames[i]);
            DrawFaceTable();
            break;
        }
    }
}


void DeleteSelectedUsers_Controller(void)
{
    char path[128];
    int deleted = 0;

    for (int i = 0; i < g_user_count; i++)
    {
        if (g_selected[i])
        {
            snprintf(path, sizeof(path), "%s/%s.bin", FACES_DIR_PATH, g_usernames[i]);
            FRESULT res = f_unlink(path);
            if (res == FR_OK)
            {
                printf("[UI] Deleted %s\r\n", path);
                deleted++;
            }
            else
            {
                printf("[UI] Failed to delete %s (err %d)\r\n", path, res);
            }
            snprintf(path, sizeof(path), "%s/%s_pin.bin", FACES_DIR_PATH, g_usernames[i]);
            res = f_unlink(path);
            if (res == FR_OK)
            {
                printf("[UI] Deleted PIN file %s\r\n", path);
            }
            else if (res != FR_NO_FILE)
            {
                printf("[UI] Failed to delete PIN file %s (err %d)\r\n", path, res);
            }
        }
    }

    ReadUserListFromSD_Model();
    DrawFaceTable();

    char msg[64];
    snprintf(msg, sizeof(msg), "Deleted %d user(s)", deleted);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKGREEN);
    UTIL_LCD_DisplayStringAt(0, BUTTON_Y - 35, (uint8_t*)msg, CENTER_MODE);
}

