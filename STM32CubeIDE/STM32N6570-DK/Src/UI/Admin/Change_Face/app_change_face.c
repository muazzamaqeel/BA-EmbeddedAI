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
#include "app_change_face_model.h"
#include "app_change_face.h"

#ifndef UTIL_LCD_COLOR_TRANSPARENT
#define UTIL_LCD_COLOR_TRANSPARENT 0x0000u
#endif
#ifndef UTIL_LCD_COLOR_DARKGRAY
#define UTIL_LCD_COLOR_DARKGRAY 0x7BEF
#endif

#define FACES_DIR_PATH      "0:binaries"
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


FATFS g_fs;
char g_usernames[MAX_USERS][MAX_NAME_LEN];
bool g_selected[MAX_USERS];
int  g_user_count = 0;


bool EnsureSDPresent(void)
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



static void DrawButtonRounded(int x, int y, int w, int h, uint32_t color, const char *label)
{
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_FillRect(x, y, w, h, UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DrawRect(x, y, w, h, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetFont(&Font20);
    int16_t text_width  = strlen(label) * Font20.Width;
    int16_t text_x = x + (w - text_width) / 2;
    int16_t text_y = y + (h - Font20.Height) / 2;
    UTIL_LCD_DisplayStringAt(text_x, text_y, (uint8_t*)label, LEFT_MODE);
}


void DrawFaceTable(void)
{
    UTIL_LCD_SetLayer(1);
    BSP_LCD_DisplayOn(0);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_DARKGRAY);
    UTIL_LCD_Clear(UTIL_LCD_COLOR_DARKGRAY);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetFont(&Font24);
    UTIL_LCD_DisplayStringAt(0, 20, (uint8_t*)"FACE MANAGEMENT", CENTER_MODE);
    UTIL_LCD_SetFont(&Font20);

    int y = TABLE_TOP_Y;
    for (int i = 0; i < g_user_count; i++)
    {
        uint16_t box_x = TABLE_MARGIN_X;
        uint16_t box_y = y + (ROW_HEIGHT - CHECKBOX_SIZE) / 2;

        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_DrawRect(box_x, box_y, CHECKBOX_SIZE, CHECKBOX_SIZE, UTIL_LCD_COLOR_WHITE);

        if (g_selected[i])
        {
            UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKGREEN);
            UTIL_LCD_FillRect(box_x + 4, box_y + 4,
                              CHECKBOX_SIZE - 8, CHECKBOX_SIZE - 8,
                              UTIL_LCD_COLOR_DARKGREEN);
        }

        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_DisplayStringAt(box_x + CHECKBOX_SIZE + 20, y + 10,
                                 (uint8_t*)g_usernames[i], LEFT_MODE);

        UTIL_LCD_DrawHLine(TABLE_MARGIN_X, y + ROW_HEIGHT,
                           SCREEN_W - 2 * TABLE_MARGIN_X, UTIL_LCD_COLOR_WHITE);

        y += ROW_HEIGHT + 2;
    }

    if (g_user_count == 0)
        UTIL_LCD_DisplayStringAt(0, 240, (uint8_t*)"No faces found", CENTER_MODE);

    DrawButtons();
}

void DrawButtons(void)
{
    int x_delete = 120;
    int x_back   = x_delete + BUTTON_W + BUTTON_GAP;

    DrawButtonRounded(x_delete, BUTTON_Y, BUTTON_W, BUTTON_H, UTIL_LCD_COLOR_BLACK, "DELETE SELECTED");
    DrawButtonRounded(x_back,   BUTTON_Y, BUTTON_W, BUTTON_H, UTIL_LCD_COLOR_BLACK, "BACK");
}




