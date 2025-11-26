#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "fatfs.h"

// Shared UI constants
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

extern FATFS g_fs;
extern char g_usernames[16][32];
extern bool g_selected[16];
extern int  g_user_count;

// View functions (correct)
bool EnsureSDPresent(void);
void DrawFaceTable(void);
void DrawButtons(void);
