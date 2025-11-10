/**
 ******************************************************************************
 * @file    app_change_face.c
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
#include "app_ui_start.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef UTIL_LCD_COLOR_TRANSPARENT
#define UTIL_LCD_COLOR_TRANSPARENT 0x0000u
#endif

#ifndef UTIL_LCD_COLOR_DARKGRAY
#define UTIL_LCD_COLOR_DARKGRAY 0x7BEF
#endif


/* ===== Layout constants ===== */
#define FACES_DIR_PATH      "0:faces"
#define MAX_USERS           16
#define MAX_NAME_LEN        32

#define SCREEN_W            800
#define SCREEN_H            480

#define ROW_HEIGHT          48
#define TABLE_TOP_Y         70
#define CHECKBOX_SIZE       30
#define TABLE_MARGIN_X      40
#define BUTTON_W            260
#define BUTTON_H            55
#define BUTTON_GAP          100
#define BUTTON_Y            (SCREEN_H - BUTTON_H - 25)

/* ===== Globals ===== */
static FATFS g_fs;
static char g_usernames[MAX_USERS][MAX_NAME_LEN];
static bool g_selected[MAX_USERS];
static int  g_user_count = 0;

/* ===== Forward declarations ===== */
static bool EnsureSDPresent(void);
static bool EnsureSDMounted(void);
static void ReadUserListFromSD(void);
static void DrawFaceTable(void);
static void DrawButtons(void);
static void ToggleSelection(uint16_t x, uint16_t y);
static void DeleteSelectedUsers(void);

/* ===== SD helpers ===== */
static bool EnsureSDPresent(void)
{
    if (BSP_SD_IsDetected(0) != SD_PRESENT)
    {
        UTIL_LCD_SetLayer(1);
        BSP_LCD_DisplayOn(0);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_DARKGRAY);
        UTIL_LCD_Clear(UTIL_LCD_COLOR_DARKGRAY);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetFont(&Font24);
        UTIL_LCD_DisplayStringAt(0, 220, (uint8_t*)"No SD card found", CENTER_MODE);
        printf("[UI] No SD card detected ❌\r\n");
        return false;
    }
    return true;
}

static bool EnsureSDMounted(void)
{
    FRESULT res = f_mount(&g_fs, "0:", 1);
    if (res == FR_OK)
    {
        printf("[UI] SD card mounted successfully\r\n");
        return true;
    }
    printf("[UI] SD mount failed (%d)\r\n", res);
    return false;
}

/* ===== Directory listing ===== */
static void ReadUserListFromSD(void)
{
    g_user_count = 0;
    memset(g_selected, 0, sizeof(g_selected));

    DIR dir;
    FILINFO fno;
    FRESULT res = f_opendir(&dir, FACES_DIR_PATH);
    if (res != FR_OK)
    {
        printf("[UI] Failed to open faces directory (%d)\r\n", res);
        return;
    }

    while (1)
    {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0)
            break;

        if (!(fno.fattrib & AM_DIR))
        {
            const char *dot = strrchr(fno.fname, '.');
            if (dot && strcmp(dot, ".bin") == 0)
            {
                size_t len = (size_t)(dot - fno.fname);
                if (len >= MAX_NAME_LEN) len = MAX_NAME_LEN - 1;
                strncpy(g_usernames[g_user_count], fno.fname, len);
                g_usernames[g_user_count][len] = '\0';
                g_user_count++;
                if (g_user_count >= MAX_USERS)
                    break;
            }
        }
    }
    f_closedir(&dir);
}

/* ===== Drawing ===== */
static void DrawButtonRounded(int x, int y, int w, int h, uint32_t color, const char *label)
{
    /* Button background (black) */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_FillRect(x, y, w, h, UTIL_LCD_COLOR_BLACK);

    /* Border (white) */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DrawRect(x, y, w, h, UTIL_LCD_COLOR_WHITE);

    /* Text (white on black) */
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetFont(&Font20);

    /* Compute exact center coordinates for text */
    int16_t text_width  = strlen(label) * Font20.Width;
    int16_t text_x = x + (w - text_width) / 2;
    int16_t text_y = y + (h - Font20.Height) / 2;

    UTIL_LCD_DisplayStringAt(text_x, text_y, (uint8_t*)label, LEFT_MODE);
}


static void DrawFaceTable(void)
{
    UTIL_LCD_SetLayer(1);
    BSP_LCD_DisplayOn(0);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_DARKGRAY);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_DARKGRAY);

    /* White text and elements */
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);

    UTIL_LCD_SetFont(&Font24);
    UTIL_LCD_DisplayStringAt(0, 20, (uint8_t*)"FACE MANAGEMENT", CENTER_MODE);
    UTIL_LCD_SetFont(&Font20);

    int y = TABLE_TOP_Y;
    for (int i = 0; i < g_user_count; i++)
    {
        uint16_t box_x = TABLE_MARGIN_X;
        uint16_t box_y = y + (ROW_HEIGHT - CHECKBOX_SIZE) / 2;

        /* Checkbox border white */
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_DrawRect(box_x, box_y, CHECKBOX_SIZE, CHECKBOX_SIZE, UTIL_LCD_COLOR_WHITE);

        /* Fill if selected */
        if (g_selected[i])
        {
            UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKGREEN);
            UTIL_LCD_FillRect(box_x + 4, box_y + 4,
                              CHECKBOX_SIZE - 8, CHECKBOX_SIZE - 8,
                              UTIL_LCD_COLOR_DARKGREEN);
        }

        /* Username text (white) */
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_DisplayStringAt(box_x + CHECKBOX_SIZE + 20, y + 10,
                                 (uint8_t*)g_usernames[i], LEFT_MODE);

        /* Horizontal divider line (white/grayish) */
        UTIL_LCD_DrawHLine(TABLE_MARGIN_X, y + ROW_HEIGHT,
                           SCREEN_W - 2 * TABLE_MARGIN_X, UTIL_LCD_COLOR_WHITE);

        y += ROW_HEIGHT + 2;
    }

    if (g_user_count == 0)
        UTIL_LCD_DisplayStringAt(0, 240, (uint8_t*)"No faces found", CENTER_MODE);

    DrawButtons();
}


static void DrawButtons(void)
{
    int x_delete = 120;
    int x_back   = x_delete + BUTTON_W + BUTTON_GAP;

    DrawButtonRounded(x_delete, BUTTON_Y, BUTTON_W, BUTTON_H, UTIL_LCD_COLOR_BLACK, "DELETE SELECTED");
    DrawButtonRounded(x_back,   BUTTON_Y, BUTTON_W, BUTTON_H, UTIL_LCD_COLOR_BLACK, "BACK");
}


/* ===== Touch logic ===== */
static void ToggleSelection(uint16_t x, uint16_t y)
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

static void DeleteSelectedUsers(void)
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
        }
    }

    ReadUserListFromSD();
    DrawFaceTable();

    char msg[64];
    snprintf(msg, sizeof(msg), "Deleted %d file(s)", deleted);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKGREEN);
    UTIL_LCD_DisplayStringAt(0, BUTTON_Y - 35, (uint8_t*)msg, CENTER_MODE);
}

/* ===== Main function ===== */
/* ===== Main function ===== */
void UI_TestPassed_Show(void)
{
    APP_SleepMode_Disable();
    printf("[UI] Entering Face Management screen...\r\n");

    if (!EnsureSDPresent()) return;
    if (!EnsureSDMounted()) return;

    ReadUserListFromSD();
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

                /* Checkbox / row selection */
                ToggleSelection(tx, ty);

                /* Buttons */
                if (ty >= BUTTON_Y && ty <= BUTTON_Y + BUTTON_H)
                {
                    if (tx >= 120 && tx <= 120 + BUTTON_W)
                    {
                        printf("[UI] DELETE SELECTED pressed\r\n");
                        DeleteSelectedUsers();
                    }
                    else if (tx >= 120 + BUTTON_W + BUTTON_GAP &&
                             tx <= 120 + BUTTON_W + BUTTON_GAP + BUTTON_W)
                    {
                        printf("[UI] BACK pressed → returning to caller\r\n");

                        /* --- Clean up overlay and ensure L1 is usable by caller --- */
                        UTIL_LCD_SetLayer(1);
                        UTIL_LCD_Clear(UTIL_LCD_COLOR_TRANSPARENT);
                        BSP_LCD_SetLayerVisible(0, 1, ENABLE);
                        BSP_LCD_Reload(0, BSP_LCD_RELOAD_IMMEDIATE);

                        /* If you disabled sleep on entry, re-enable it now */
                        // APP_SleepMode_Enable();  // <- use your real re-enable call if available

                        /* IMPORTANT: Do NOT call UI_StartScreen_Show() or UI_WaitForButton() here.
                           Just return and let the caller (Admin/FSM) decide. */
                        return;
                    }

                }
            }
            else if (!ts.TouchDetected)
                touch_active = false;
        }
        HAL_Delay(80);
    }
}

