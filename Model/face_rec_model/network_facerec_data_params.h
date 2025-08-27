/**
  ******************************************************************************
  * @file    network_facerec_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-08-27T16:47:49+0200
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

#ifndef NETWORK_FACEREC_DATA_PARAMS_H
#define NETWORK_FACEREC_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_NETWORK_FACEREC_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_network_facerec_data_weights_params[1]))
*/

#define AI_NETWORK_FACEREC_DATA_CONFIG               (NULL)


#define AI_NETWORK_FACEREC_DATA_ACTIVATIONS_SIZES \
  { 7227648, }
#define AI_NETWORK_FACEREC_DATA_ACTIVATIONS_SIZE     (7227648)
#define AI_NETWORK_FACEREC_DATA_ACTIVATIONS_COUNT    (1)
#define AI_NETWORK_FACEREC_DATA_ACTIVATION_1_SIZE    (7227648)



#define AI_NETWORK_FACEREC_DATA_WEIGHTS_SIZES \
  { 136522752, }
#define AI_NETWORK_FACEREC_DATA_WEIGHTS_SIZE         (136522752)
#define AI_NETWORK_FACEREC_DATA_WEIGHTS_COUNT        (1)
#define AI_NETWORK_FACEREC_DATA_WEIGHT_1_SIZE        (136522752)



#define AI_NETWORK_FACEREC_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_network_facerec_activations_table[1])

extern ai_handle g_network_facerec_activations_table[1 + 2];



#define AI_NETWORK_FACEREC_DATA_WEIGHTS_TABLE_GET() \
  (&g_network_facerec_weights_table[1])

extern ai_handle g_network_facerec_weights_table[1 + 2];


#endif    /* NETWORK_FACEREC_DATA_PARAMS_H */
