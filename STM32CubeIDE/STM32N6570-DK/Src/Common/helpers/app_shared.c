/**
 *******************************************************************************
 * @file    app_shared.c
 * @brief   Shared global state for pipeline, face recognition, and sleep mode
 *******************************************************************************
 */

#include "app_shared.h"
#include <stdbool.h>
#include <stdint.h>

bool g_pipeline_running = false;   /* true when NN + camera pipeline is active */
bool g_fr_active        = false;   /* true when face recognition is running   */

uint32_t g_last_face_time = 0;     /* last time a face was detected           */
uint32_t g_wake_time      = 0;     /* last time device woke from sleep        */
volatile det_profile_t g_det_prof = {0};
