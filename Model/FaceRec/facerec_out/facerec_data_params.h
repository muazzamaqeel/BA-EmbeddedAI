/**
  ******************************************************************************
  * @file    facerec_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-08-29T11:45:26+0200
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef FACEREC_DATA_PARAMS_H
#define FACEREC_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_FACEREC_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_facerec_data_weights_params[1]))
*/

#define AI_FACEREC_DATA_CONFIG               (NULL)


#define AI_FACEREC_DATA_ACTIVATIONS_SIZES \
  { 4816896, }
#define AI_FACEREC_DATA_ACTIVATIONS_SIZE     (4816896)
#define AI_FACEREC_DATA_ACTIVATIONS_COUNT    (1)
#define AI_FACEREC_DATA_ACTIVATION_1_SIZE    (4816896)



#define AI_FACEREC_DATA_WEIGHTS_SIZES \
  { 5044480, }
#define AI_FACEREC_DATA_WEIGHTS_SIZE         (5044480)
#define AI_FACEREC_DATA_WEIGHTS_COUNT        (1)
#define AI_FACEREC_DATA_WEIGHT_1_SIZE        (5044480)



#define AI_FACEREC_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_facerec_activations_table[1])

extern ai_handle g_facerec_activations_table[1 + 2];



#define AI_FACEREC_DATA_WEIGHTS_TABLE_GET() \
  (&g_facerec_weights_table[1])

extern ai_handle g_facerec_weights_table[1 + 2];


#endif    /* FACEREC_DATA_PARAMS_H */
