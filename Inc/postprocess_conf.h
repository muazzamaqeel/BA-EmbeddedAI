/*---------------------------------------------------------------------------------------------
#  * Copyright (c) 2023 STMicroelectronics.
#  * All rights reserved.
#  *
#  * This software is licensed under terms that can be found in the LICENSE file in
#  * the root directory of this software component.
#  * If no LICENSE file comes with this software, it is provided AS-IS.
#  *--------------------------------------------------------------------------------------------*/

/* ---------------    Generated code    ----------------- */
#ifndef __POSTPROCESS_CONF_H__
#define __POSTPROCESS_CONF_H__

#include "arm_math.h"

#ifdef STM32N6570_DK_REV
#define POSTPROCESS_TYPE                          POSTPROCESS_OD_ST_YOLOX_UF

#define AI_OD_ST_YOLOX_PP_NB_CLASSES                (1)
#define AI_OD_ST_YOLOX_PP_L_GRID_WIDTH              (60)
#define AI_OD_ST_YOLOX_PP_L_GRID_HEIGHT             (60)
#define AI_OD_ST_YOLOX_PP_M_GRID_WIDTH              (30)
#define AI_OD_ST_YOLOX_PP_M_GRID_HEIGHT             (30)
#define AI_OD_ST_YOLOX_PP_S_GRID_WIDTH              (15)
#define AI_OD_ST_YOLOX_PP_S_GRID_HEIGHT             (15)
#define AI_OD_ST_YOLOX_PP_NB_ANCHORS                (3)
/* Anchor boxes */
static const float32_t AI_OD_ST_YOLOX_PP_L_ANCHORS[2 * AI_OD_ST_YOLOX_PP_NB_ANCHORS] = {30.000000, 30.000000, 4.200000, 15.000000, 13.800000, 41.999999};
static const float32_t AI_OD_ST_YOLOX_PP_M_ANCHORS[2 * AI_OD_ST_YOLOX_PP_NB_ANCHORS] = {15.000000, 15.000000, 2.100000, 7.500000, 6.900000, 21.000000};
static const float32_t AI_OD_ST_YOLOX_PP_S_ANCHORS[2 * AI_OD_ST_YOLOX_PP_NB_ANCHORS] = {7.500000, 7.500000, 1.050000, 3.750000, 3.450000, 10.500000};
#else
#define POSTPROCESS_TYPE                          POSTPROCESS_OD_YOLO_V2_UF

#define AI_OD_YOLOV2_PP_NB_CLASSES                (1)
#define AI_OD_YOLOV2_PP_NB_ANCHORS                (5)
#define AI_OD_YOLOV2_PP_GRID_WIDTH                (7)
#define AI_OD_YOLOV2_PP_GRID_HEIGHT               (7)
#define AI_OD_YOLOV2_PP_NB_INPUT_BOXES            (AI_OD_YOLOV2_PP_GRID_WIDTH * AI_OD_YOLOV2_PP_GRID_HEIGHT)
/* Anchor boxes */
static const float32_t AI_OD_YOLOV2_PP_ANCHORS[2*AI_OD_YOLOV2_PP_NB_ANCHORS] = {
    0.9883000000f,     3.3606000000f,
    2.1194000000f,     5.3759000000f,
    3.0520000000f,     9.1336000000f,
    5.5517000000f,     9.3066000000f,
    9.7260000000f,     11.1422000000f,
  };
#endif

#endif      /* __POSTPROCESS_CONF_H__  */
