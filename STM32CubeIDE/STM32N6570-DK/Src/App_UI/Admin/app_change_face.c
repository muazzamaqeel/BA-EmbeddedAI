/**
 ******************************************************************************
 * @file    app_change_face.c
 * @brief   Change Face screen — lists available users from SD card
 ******************************************************************************
 */

#include "stm32_lcd.h"
#include "stm32_lcd_ex.h"
#include "stm32n6570_discovery_lcd.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_sd.h"
#include "app_sleepmode.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* ===== Constants ===== */
#define FACES_DIR_PATH      "0:faces"
#define MAX_USERS           16
#define MAX_NAME_LEN        32

/* ===== Globals ===== */
static FATFS g_fs; // persistent filesystem object
static char g_usernames[MAX_USERS][MAX_NAME_LEN];
static int  g_user_count = 0;

/* ===== Helpers ===== */
static bool EnsureSDPresent(void)
{
    if (BSP_SD_IsDetected(0) != SD_PRESENT)
    {
        printf("[UI] No SD card detected ❌\r\n");
        UTIL_LCD_SetLayer(1);
        BSP_LCD_DisplayOn(0);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_SetFont(&Font24);
        UTIL_LCD_DisplayStringAt(0, 220, (uint8_t*)"No SD card found", CENTER_MODE);
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
    else
    {
        printf("[UI] SD mount failed (%d)\r\n", res);

        UTIL_LCD_SetLayer(1);
        BSP_LCD_DisplayOn(0);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_SetFont(&Font24);
        UTIL_LCD_DisplayStringAt(0, 220, (uint8_t*)"SD mount failed", CENTER_MODE);

        return false;
    }
}

static void ReadUserListFromSD(void)
{
    g_user_count = 0;
    DIR dir;
    FILINFO fno;

    FRESULT res = f_opendir(&dir, FACES_DIR_PATH);
    if (res != FR_OK)
    {
        printf("[UI] Failed to open faces directory (%d)\r\n", res);
        UTIL_LCD_DisplayStringAt(0, 240, (uint8_t*)"No faces directory", CENTER_MODE);
        return;
    }

    printf("[UI] Reading user list from %s...\r\n", FACES_DIR_PATH);

    for (;;)
    {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0)
            break;

        if (!(fno.fattrib & AM_DIR))
        {
            const char *name = fno.fname;
            const char *dot = strrchr(name, '.');
            if (dot && strcmp(dot, ".bin") == 0)
            {
                size_t len = (size_t)(dot - name);
                if (len >= MAX_NAME_LEN) len = MAX_NAME_LEN - 1;
                strncpy(g_usernames[g_user_count], name, len);
                g_usernames[g_user_count][len] = '\0';
                g_user_count++;
                if (g_user_count >= MAX_USERS)
                    break;
            }
        }
    }
    f_closedir(&dir);

    printf("[UI] Found %d user(s):\r\n", g_user_count);
    for (int i = 0; i < g_user_count; i++)
        printf("   - %s\r\n", g_usernames[i]);
}

static void DrawUserList(void)
{
    UTIL_LCD_SetLayer(1);
    BSP_LCD_DisplayOn(0);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetFont(&Font20);

    UTIL_LCD_DisplayStringAt(0, 20, (uint8_t*)"Available Users:", CENTER_MODE);

    int y = 60;
    for (int i = 0; i < g_user_count; i++)
    {
        char line[64];
        snprintf(line, sizeof(line), "%2d. %s", i + 1, g_usernames[i]);
        UTIL_LCD_DisplayStringAt(20, y, (uint8_t*)line, LEFT_MODE);
        y += 30;
        if (y > 440)
        {
            UTIL_LCD_DisplayStringAt(20, y, (uint8_t*)"... (more)", LEFT_MODE);
            break;
        }
    }

    if (g_user_count == 0)
    {
        UTIL_LCD_DisplayStringAt(0, 240, (uint8_t*)"No faces found", CENTER_MODE);
    }
}

/* ===== Public API ===== */
void UI_TestPassed_Show(void)
{
    APP_SleepMode_Disable();

    printf("[UI] Entering Change Face screen (SD listing)...\r\n");

    /* --- Check if SD card is inserted --- */
    if (!EnsureSDPresent())
        return;

    /* --- Try to mount SD --- */
    if (!EnsureSDMounted())
        return;

    /* --- Read users from /faces --- */
    ReadUserListFromSD();

    /* --- Draw the list --- */
    DrawUserList();

    printf("[UI] Change Face screen displayed\r\n");

    while (1)
        HAL_Delay(1000);
}
