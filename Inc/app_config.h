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
#ifdef STM32N6570_DK_REV
  #define LCD_BG_WIDTH  800
  #define LCD_BG_HEIGHT 480
#else
  #define LCD_BG_WIDTH  320
  #define LCD_BG_HEIGHT 240
#endif

/* Delay display by DISPLAY_DELAY frame number */
#define DISPLAY_DELAY 0

/* -------------------------------------------------------------------------- */
/* Object detection thresholds from the original config (not used by BlazeFace) */
#ifdef STM32N6570_DK_REV
  /* Use yolox */
  #define AI_OD_ST_YOLOX_PP_IOU_THRESHOLD             (0.5f)
  #define AI_OD_ST_YOLOX_PP_CONF_THRESHOLD            (0.6f)
  #define AI_OD_ST_YOLOX_PP_MAX_BOXES_LIMIT           (10)
  #ifndef AI_OD_PP_MAX_BOXES_LIMIT
    #define AI_OD_PP_MAX_BOXES_LIMIT                  (AI_OD_ST_YOLOX_PP_MAX_BOXES_LIMIT)
  #endif
#else
  /* Use yolov2 */
  #define AI_OD_YOLOV2_PP_CONF_THRESHOLD              (0.6f)
  #define AI_OD_YOLOV2_PP_IOU_THRESHOLD               (0.3f)
  #define AI_OD_YOLOV2_PP_MAX_BOXES_LIMIT             (10)
  #ifndef AI_OD_PP_MAX_BOXES_LIMIT
    #define AI_OD_PP_MAX_BOXES_LIMIT                  (AI_OD_YOLOV2_PP_MAX_BOXES_LIMIT)
  #endif
#endif
/* -------------------------------------------------------------------------- */

/* Input pixel format from camera to network preproc (RGB888) */
#define NN_FORMAT DCMIPP_PIXEL_PACKER_FORMAT_RGB888_YUV444_1
#define NN_BPP    3

/* Classes table (kept for UI elements even though BlazeFace is not class-based) */
#define NB_CLASSES 2
#define DECLARE_CLASSES_TABLE \
  const char* classes_table[NB_CLASSES] = { "person", "not_person" };

/* -------------------------------------------------------------------------- */
/* Force settings for BlazeFace (4 outputs, 128x128). Keep inside the guard. */
/* -------------------------------------------------------------------------- */
#undef  NN_WIDTH
#undef  NN_HEIGHT
#undef  NN_OUT_NB
#ifdef NN_OUT0_SIZE
  #undef NN_OUT0_SIZE
#endif
#ifdef NN_OUT1_SIZE
  #undef NN_OUT1_SIZE
#endif
#ifdef NN_OUT2_SIZE
  #undef NN_OUT2_SIZE
#endif
#ifdef NN_OUT3_SIZE
  #undef NN_OUT3_SIZE
#endif

#define NN_WIDTH     128
#define NN_HEIGHT    128
#define NN_OUT_NB    4

/* Sizes in BYTES (float32 outputs) as reported by ST Edge AI analyze/generate: */
#define NN_OUT0_SIZE (512 * 16 * 4)  /* 32768  */
#define NN_OUT1_SIZE (512 *  1 * 4)  /* 2048   */
#define NN_OUT2_SIZE (384 *  1 * 4)  /* 1536   */
#define NN_OUT3_SIZE (384 * 16 * 4)  /* 24576  */

#endif /* APP_CONFIG */
