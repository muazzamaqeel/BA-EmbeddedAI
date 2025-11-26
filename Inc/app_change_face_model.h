// app_change_face_model.h
#pragma once

#include <stdbool.h>

#define FACES_DIR_PATH      "0:binaries"
#define MAX_USERS           16
#define MAX_NAME_LEN        32

bool EnsureSDMounted_Model(void);
void ReadUserListFromSD_Model(void);
