#ifndef __POSTPROCESS_CONF_H__
#define __POSTPROCESS_CONF_H__

#include "arm_math.h"

/* define a concrete value for CUSTOM */
#ifndef POSTPROCESS_CUSTOM
#define POSTPROCESS_CUSTOM  999
#endif

#ifdef STM32N6570_DK_REV
  /* BlazeFace: use our custom postprocess */
  #define POSTPROCESS_TYPE  POSTPROCESS_CUSTOM

  /* (These YOLOX constants can stay; they won’t be used in CUSTOM) */
  #define AI_OD_ST_YOLOX_PP_NB_CLASSES     (1)
  #define AI_OD_ST_YOLOX_PP_L_GRID_WIDTH   (60)
  #define AI_OD_ST_YOLOX_PP_L_GRID_HEIGHT  (60)
  #define AI_OD_ST_YOLOX_PP_M_GRID_WIDTH   (30)
  #define AI_OD_ST_YOLOX_PP_M_GRID_HEIGHT  (30)
  #define AI_OD_ST_YOLOX_PP_S_GRID_WIDTH   (15)
  #define AI_OD_ST_YOLOX_PP_S_GRID_HEIGHT  (15)
  #define AI_OD_ST_YOLOX_PP_NB_ANCHORS     (3)
  static const float32_t AI_OD_ST_YOLOX_PP_L_ANCHORS[2 * AI_OD_ST_YOLOX_PP_NB_ANCHORS] = {30.0f, 30.0f, 4.2f, 15.0f, 13.8f, 42.0f};
  static const float32_t AI_OD_ST_YOLOX_PP_M_ANCHORS[2 * AI_OD_ST_YOLOX_PP_NB_ANCHORS] = {15.0f, 15.0f, 2.1f, 7.5f, 6.9f, 21.0f};
  static const float32_t AI_OD_ST_YOLOX_PP_S_ANCHORS[2 * AI_OD_ST_YOLOX_PP_NB_ANCHORS] = {7.5f, 7.5f, 1.05f, 3.75f, 3.45f, 10.5f};
#else
  /* non-DK build keeps the original YOLOv2 UF postprocess */
  #define POSTPROCESS_TYPE                          POSTPROCESS_OD_YOLO_V2_UF
  #define AI_OD_YOLOV2_PP_NB_CLASSES                (1)
  #define AI_OD_YOLOV2_PP_NB_ANCHORS                (5)
  #define AI_OD_YOLOV2_PP_GRID_WIDTH                (7)
  #define AI_OD_YOLOV2_PP_GRID_HEIGHT               (7)
  #define AI_OD_YOLOV2_PP_NB_INPUT_BOXES            (AI_OD_YOLOV2_PP_GRID_WIDTH * AI_OD_YOLOV2_PP_GRID_HEIGHT)
  static const float32_t AI_OD_YOLOV2_PP_ANCHORS[2*AI_OD_YOLOV2_PP_NB_ANCHORS] = {
      0.9883f, 3.3606f, 2.1194f, 5.3759f, 3.0520f, 9.1336f, 5.5517f, 9.3066f, 9.7260f, 11.1422f
  };
#endif

#endif /* __POSTPROCESS_CONF_H__ */
