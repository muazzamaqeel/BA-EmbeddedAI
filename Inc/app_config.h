/**
 ******************************************************************************
 * @file    app_config.h
 * @author  GPM Application Team
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#ifndef APP_CONFIG
#define APP_CONFIG

#define USE_DCACHE

#include "postprocess_conf.h"

/* Define sensor orientation */
#define CAMERA_FLIP CMW_MIRRORFLIP_MIRROR

/* Define display size */
#define LCD_BG_WIDTH 800
#define LCD_BG_HEIGHT 480
/* Delay display by DISPLAY_DELAY frame number */
#define DISPLAY_DELAY 1

/* Model Related Info */
#define POSTPROCESS_TYPE                          POSTPROCESS_OD_YOLO_V2_UF
#define AI_OBJDETECT_YOLOV2_PP_CONF_THRESHOLD     (0.6f)
#define AI_OBJDETECT_YOLOV2_PP_IOU_THRESHOLD      (0.3f)
#define AI_OBJDETECT_YOLOV2_PP_MAX_BOXES_LIMIT    (10)

#define NN_WIDTH 224
#define NN_HEIGHT 224
#define NN_BUFFER_OUT_SIZE 5880
#define NN_FORMAT DCMIPP_PIXEL_PACKER_FORMAT_RGB888_YUV444_1
#define NN_BPP 3
#define NB_CLASSES 2
#define DECLARE_CLASSES_TABLE const char* classes_table[NB_CLASSES] = {\
  "person", "not_person"}

#endif
