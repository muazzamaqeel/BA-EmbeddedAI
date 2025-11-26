

//app_change_face_model.c

// app_change_face_model.c
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "fatfs.h"      // For FRESULT, DIR, FILINFO, f_opendir, f_readdir, f_closedir
#include "stm32n6570_discovery_sd.h"  // For SD detection if needed

#include "app_change_face_model.h"   // Your new header (shown below)
extern FATFS g_fs;
extern char g_usernames[MAX_USERS][MAX_NAME_LEN];
extern bool g_selected[MAX_USERS];
extern int g_user_count;


// ---------------------------
// MODEL IMPLEMENTATION
// ---------------------------



bool EnsureSDMounted_Model(void)
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


void ReadUserListFromSD_Model(void)
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
                /* Skip PIN files */
                if (strstr(fno.fname, "_pin")) continue;
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
