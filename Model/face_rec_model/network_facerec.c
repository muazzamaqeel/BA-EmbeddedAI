/**
  ******************************************************************************
  * @file    network_facerec.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-08-27T16:47:49+0200
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "network_facerec.h"
#include "network_facerec_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network_facerec
 
#undef AI_NETWORK_FACEREC_MODEL_SIGNATURE
#define AI_NETWORK_FACEREC_MODEL_SIGNATURE     "0xd31492cf46f686bab409ca0e781b36e2"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2025-08-27T16:47:49+0200"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_FACEREC_N_BATCHES
#define AI_NETWORK_FACEREC_N_BATCHES         (1)

static ai_ptr g_network_facerec_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_network_facerec_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  input_1_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 37632, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 802816, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#30 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#31 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#32 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#33 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#34 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#35 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#36 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#37 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#38 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#39 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#40 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#41 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#42 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 401408, AI_STATIC)

/* Array#43 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#44 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#45 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#46 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#47 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#48 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#49 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#50 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#51 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#52 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#53 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#54 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#55 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#56 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#57 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#58 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#59 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#60 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#61 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#62 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#63 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#64 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#65 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#66 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#67 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#68 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#69 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#70 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#71 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#72 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#73 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#74 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#75 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#76 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#77 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#78 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#79 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200704, AI_STATIC)

/* Array#80 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#81 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#82 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#83 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#84 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#85 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#86 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#87 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#88 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#89 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#90 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#91 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#92 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#93 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#94 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#95 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#96 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#97 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#98 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#99 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#100 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#101 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#102 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#103 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#104 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#105 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#106 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#107 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#108 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#109 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#110 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#111 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#112 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#113 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#114 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#115 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#116 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#117 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#118 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#119 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#120 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#121 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#122 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#123 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#124 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#125 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#126 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#127 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#128 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50176, AI_STATIC)

/* Array#129 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#130 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#131 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#132 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#133 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#134 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100352, AI_STATIC)

/* Array#135 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#136 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#137 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#138 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#139 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#140 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#141 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#142 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#143 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#144 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#145 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#146 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#147 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#148 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Neg_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#149 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu_10_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#150 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#151 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#152 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#153 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#154 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_add_add0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#155 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25088, AI_STATIC)

/* Array#156 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_dense_MatMul0_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 512, AI_STATIC)

/* Array#157 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1728, AI_STATIC)

/* Array#158 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#159 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#160 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#161 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4096, AI_STATIC)

/* Array#162 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#163 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#164 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#165 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 36864, AI_STATIC)

/* Array#166 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#167 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#168 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 36864, AI_STATIC)

/* Array#169 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#170 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#171 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#172 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 36864, AI_STATIC)

/* Array#173 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#174 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#175 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 36864, AI_STATIC)

/* Array#176 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#177 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#178 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#179 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 36864, AI_STATIC)

/* Array#180 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#181 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#182 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 36864, AI_STATIC)

/* Array#183 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#184 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8192, AI_STATIC)

/* Array#185 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#186 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#187 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#188 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 73728, AI_STATIC)

/* Array#189 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#190 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#191 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#192 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 147456, AI_STATIC)

/* Array#193 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#194 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#195 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#196 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 147456, AI_STATIC)

/* Array#197 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#198 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#199 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 147456, AI_STATIC)

/* Array#200 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#201 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#202 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#203 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 147456, AI_STATIC)

/* Array#204 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#205 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#206 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 147456, AI_STATIC)

/* Array#207 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#208 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#209 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#210 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 147456, AI_STATIC)

/* Array#211 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#212 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#213 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 147456, AI_STATIC)

/* Array#214 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#215 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32768, AI_STATIC)

/* Array#216 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#217 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#218 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#219 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 294912, AI_STATIC)

/* Array#220 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#221 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#222 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#223 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 589824, AI_STATIC)

/* Array#224 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#225 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#226 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#227 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 589824, AI_STATIC)

/* Array#228 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#229 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#230 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 589824, AI_STATIC)

/* Array#231 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#232 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#233 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#234 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 589824, AI_STATIC)

/* Array#235 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#236 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#237 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 589824, AI_STATIC)

/* Array#238 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#239 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#240 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#241 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 589824, AI_STATIC)

/* Array#242 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#243 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#244 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 589824, AI_STATIC)

/* Array#245 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#246 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#247 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#248 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 589824, AI_STATIC)

/* Array#249 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#250 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#251 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 589824, AI_STATIC)

/* Array#252 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#253 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#254 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#255 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 589824, AI_STATIC)

/* Array#256 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#257 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#258 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 589824, AI_STATIC)

/* Array#259 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#260 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 131072, AI_STATIC)

/* Array#261 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#262 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#263 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#264 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1179648, AI_STATIC)

/* Array#265 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#266 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#267 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#268 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2359296, AI_STATIC)

/* Array#269 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#270 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#271 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#272 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2359296, AI_STATIC)

/* Array#273 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#274 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#275 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2359296, AI_STATIC)

/* Array#276 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#277 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#278 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#279 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2359296, AI_STATIC)

/* Array#280 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#281 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#282 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2359296, AI_STATIC)

/* Array#283 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#284 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#285 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#286 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_dense_MatMul0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12845056, AI_STATIC)

/* Array#287 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_dense_MatMul0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)

/* Array#288 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 27, AI_STATIC)

/* Array#289 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#290 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#291 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#292 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#293 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#294 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#295 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#296 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#297 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#298 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#299 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#300 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#301 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#302 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#303 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#304 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#305 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128, AI_STATIC)

/* Array#306 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1152, AI_STATIC)

/* Array#307 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#308 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#309 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#310 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#311 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#312 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#313 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#314 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#315 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#316 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#317 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#318 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#319 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)

/* Array#320 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4608, AI_STATIC)

/* Array#321 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4608, AI_STATIC)

/* Array#322 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4608, AI_STATIC)

/* Array#323 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4608, AI_STATIC)

/* Array#324 */
AI_ARRAY_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4608, AI_STATIC)

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_bias, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_bias_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_output, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_output_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_output0, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 25088, 1, 1), AI_STRIDE_INIT(4, 4, 4, 100352, 100352),
  1, &StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_output_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_scale, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_scale_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_bias, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_output, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 3), AI_STRIDE_INIT(4, 4, 4, 12, 36),
  1, &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_weights, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 3, 3, 3, 64), AI_STRIDE_INIT(4, 4, 12, 768, 2304),
  1, &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_Neg_10_output, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv1_prelu_Neg_10_output_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_Relu0_output, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv1_prelu_Relu0_output_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_Relu_10_output, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv1_prelu_Relu_10_output_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_add0_output, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv1_prelu_add0_output_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_bias, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_bias_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_output, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_output_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_scale, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_scale_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_bias, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_output, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_weights, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 64, 1, 1, 64), AI_STRIDE_INIT(4, 4, 256, 16384, 16384),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_output, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Neg_10_output, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Neg_10_output_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu0_output, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu0_output_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu_10_output, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu_10_output_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_add0_output, AI_STATIC,
  25, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_add0_output_array, NULL)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_output, AI_STATIC,
  26, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_output_array, NULL)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_scale, AI_STATIC,
  27, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_scale_array, NULL)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_output, AI_STATIC,
  29, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 112, 112), AI_STRIDE_INIT(4, 4, 4, 256, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  30, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #31 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  31, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 64), AI_STRIDE_INIT(4, 4, 256, 16384, 49152),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #32 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  32, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #33 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_output, AI_STATIC,
  33, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #34 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  34, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #35 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  35, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 64), AI_STRIDE_INIT(4, 4, 256, 16384, 49152),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #36 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_add_add0_output, AI_STATIC,
  36, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block1_add_add0_output_array, NULL)

/* Tensor #37 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  37, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #38 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_output, AI_STATIC,
  38, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #39 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  39, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #40 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Neg_10_output, AI_STATIC,
  40, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Neg_10_output_array, NULL)

/* Tensor #41 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu0_output, AI_STATIC,
  41, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu0_output_array, NULL)

/* Tensor #42 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu_10_output, AI_STATIC,
  42, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu_10_output_array, NULL)

/* Tensor #43 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_add0_output, AI_STATIC,
  43, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_add0_output_array, NULL)

/* Tensor #44 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_output, AI_STATIC,
  44, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_output_array, NULL)

/* Tensor #45 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_scale, AI_STATIC,
  45, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_scale_array, NULL)

/* Tensor #46 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  46, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #47 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_output, AI_STATIC,
  47, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #48 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  48, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #49 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  49, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 64), AI_STRIDE_INIT(4, 4, 256, 16384, 49152),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #50 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  50, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #51 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_output, AI_STATIC,
  51, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #52 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  52, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #53 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  53, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 64), AI_STRIDE_INIT(4, 4, 256, 16384, 49152),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #54 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_add_add0_output, AI_STATIC,
  54, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block2_add_add0_output_array, NULL)

/* Tensor #55 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  55, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #56 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_output, AI_STATIC,
  56, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #57 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  57, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #58 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Neg_10_output, AI_STATIC,
  58, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Neg_10_output_array, NULL)

/* Tensor #59 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu0_output, AI_STATIC,
  59, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu0_output_array, NULL)

/* Tensor #60 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu_10_output, AI_STATIC,
  60, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu_10_output_array, NULL)

/* Tensor #61 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_add0_output, AI_STATIC,
  61, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_add0_output_array, NULL)

/* Tensor #62 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_output, AI_STATIC,
  62, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_output_array, NULL)

/* Tensor #63 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_scale, AI_STATIC,
  63, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_scale_array, NULL)

/* Tensor #64 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  64, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #65 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_output, AI_STATIC,
  65, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #66 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  66, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #67 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  67, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 64), AI_STRIDE_INIT(4, 4, 256, 16384, 49152),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #68 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  68, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #69 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_output, AI_STATIC,
  69, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #70 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  70, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #71 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  71, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 64), AI_STRIDE_INIT(4, 4, 256, 16384, 49152),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #72 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_add_add0_output, AI_STATIC,
  72, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv2_block3_add_add0_output_array, NULL)

/* Tensor #73 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_bias, AI_STATIC,
  73, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #74 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_output, AI_STATIC,
  74, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #75 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  75, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #76 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_weights, AI_STATIC,
  76, 0x0,
  AI_SHAPE_INIT(4, 64, 1, 1, 128), AI_STRIDE_INIT(4, 4, 256, 32768, 32768),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #77 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  77, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #78 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_output, AI_STATIC,
  78, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 56, 56), AI_STRIDE_INIT(4, 4, 4, 256, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #79 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  79, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #80 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Neg_10_output, AI_STATIC,
  80, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Neg_10_output_array, NULL)

/* Tensor #81 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu0_output, AI_STATIC,
  81, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu0_output_array, NULL)

/* Tensor #82 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu_10_output, AI_STATIC,
  82, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu_10_output_array, NULL)

/* Tensor #83 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_add0_output, AI_STATIC,
  83, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_add0_output_array, NULL)

/* Tensor #84 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_bias, AI_STATIC,
  84, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_bias_array, NULL)

/* Tensor #85 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_output, AI_STATIC,
  85, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_output_array, NULL)

/* Tensor #86 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_scale, AI_STATIC,
  86, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_scale_array, NULL)

/* Tensor #87 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  87, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #88 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_output, AI_STATIC,
  88, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 56, 56), AI_STRIDE_INIT(4, 4, 4, 512, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #89 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  89, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 3, 3), AI_STRIDE_INIT(4, 4, 4, 256, 768),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #90 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  90, 0x0,
  AI_SHAPE_INIT(4, 64, 3, 3, 128), AI_STRIDE_INIT(4, 4, 256, 32768, 98304),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #91 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  91, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #92 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_output, AI_STATIC,
  92, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #93 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  93, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 3, 3), AI_STRIDE_INIT(4, 4, 4, 512, 1536),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #94 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  94, 0x0,
  AI_SHAPE_INIT(4, 128, 3, 3, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 196608),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #95 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_add_add0_output, AI_STATIC,
  95, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block1_add_add0_output_array, NULL)

/* Tensor #96 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  96, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #97 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_output, AI_STATIC,
  97, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #98 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  98, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #99 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Neg_10_output, AI_STATIC,
  99, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Neg_10_output_array, NULL)

/* Tensor #100 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu0_output, AI_STATIC,
  100, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu0_output_array, NULL)

/* Tensor #101 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu_10_output, AI_STATIC,
  101, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu_10_output_array, NULL)

/* Tensor #102 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_add0_output, AI_STATIC,
  102, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_add0_output_array, NULL)

/* Tensor #103 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_output, AI_STATIC,
  103, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_output_array, NULL)

/* Tensor #104 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_scale, AI_STATIC,
  104, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_scale_array, NULL)

/* Tensor #105 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  105, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #106 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_output, AI_STATIC,
  106, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #107 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  107, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 3, 3), AI_STRIDE_INIT(4, 4, 4, 512, 1536),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #108 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  108, 0x0,
  AI_SHAPE_INIT(4, 128, 3, 3, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 196608),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #109 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  109, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #110 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_output, AI_STATIC,
  110, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #111 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  111, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 3, 3), AI_STRIDE_INIT(4, 4, 4, 512, 1536),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #112 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  112, 0x0,
  AI_SHAPE_INIT(4, 128, 3, 3, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 196608),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #113 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_add_add0_output, AI_STATIC,
  113, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block2_add_add0_output_array, NULL)

/* Tensor #114 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  114, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #115 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_output, AI_STATIC,
  115, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #116 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  116, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #117 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Neg_10_output, AI_STATIC,
  117, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Neg_10_output_array, NULL)

/* Tensor #118 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu0_output, AI_STATIC,
  118, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu0_output_array, NULL)

/* Tensor #119 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu_10_output, AI_STATIC,
  119, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu_10_output_array, NULL)

/* Tensor #120 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_add0_output, AI_STATIC,
  120, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_add0_output_array, NULL)

/* Tensor #121 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_output, AI_STATIC,
  121, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_output_array, NULL)

/* Tensor #122 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_scale, AI_STATIC,
  122, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_scale_array, NULL)

/* Tensor #123 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  123, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #124 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_output, AI_STATIC,
  124, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #125 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  125, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 3, 3), AI_STRIDE_INIT(4, 4, 4, 512, 1536),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #126 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  126, 0x0,
  AI_SHAPE_INIT(4, 128, 3, 3, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 196608),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #127 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  127, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #128 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_output, AI_STATIC,
  128, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #129 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  129, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 3, 3), AI_STRIDE_INIT(4, 4, 4, 512, 1536),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #130 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  130, 0x0,
  AI_SHAPE_INIT(4, 128, 3, 3, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 196608),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #131 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_add_add0_output, AI_STATIC,
  131, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block3_add_add0_output_array, NULL)

/* Tensor #132 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  132, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #133 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_output, AI_STATIC,
  133, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #134 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  134, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #135 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Neg_10_output, AI_STATIC,
  135, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Neg_10_output_array, NULL)

/* Tensor #136 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu0_output, AI_STATIC,
  136, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu0_output_array, NULL)

/* Tensor #137 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu_10_output, AI_STATIC,
  137, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu_10_output_array, NULL)

/* Tensor #138 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_add0_output, AI_STATIC,
  138, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_add0_output_array, NULL)

/* Tensor #139 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_output, AI_STATIC,
  139, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_output_array, NULL)

/* Tensor #140 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_scale, AI_STATIC,
  140, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_scale_array, NULL)

/* Tensor #141 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  141, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #142 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_output, AI_STATIC,
  142, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #143 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  143, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 3, 3), AI_STRIDE_INIT(4, 4, 4, 512, 1536),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #144 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  144, 0x0,
  AI_SHAPE_INIT(4, 128, 3, 3, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 196608),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #145 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  145, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #146 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_output, AI_STATIC,
  146, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #147 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  147, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 3, 3), AI_STRIDE_INIT(4, 4, 4, 512, 1536),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #148 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  148, 0x0,
  AI_SHAPE_INIT(4, 128, 3, 3, 128), AI_STRIDE_INIT(4, 4, 512, 65536, 196608),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #149 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_add_add0_output, AI_STATIC,
  149, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv3_block4_add_add0_output_array, NULL)

/* Tensor #150 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_bias, AI_STATIC,
  150, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #151 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_output, AI_STATIC,
  151, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #152 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  152, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #153 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_weights, AI_STATIC,
  153, 0x0,
  AI_SHAPE_INIT(4, 128, 1, 1, 256), AI_STRIDE_INIT(4, 4, 512, 131072, 131072),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #154 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  154, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #155 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_output, AI_STATIC,
  155, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 28, 28), AI_STRIDE_INIT(4, 4, 4, 512, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #156 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  156, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #157 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Neg_10_output, AI_STATIC,
  157, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Neg_10_output_array, NULL)

/* Tensor #158 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu0_output, AI_STATIC,
  158, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu0_output_array, NULL)

/* Tensor #159 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu_10_output, AI_STATIC,
  159, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu_10_output_array, NULL)

/* Tensor #160 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_add0_output, AI_STATIC,
  160, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_add0_output_array, NULL)

/* Tensor #161 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_bias, AI_STATIC,
  161, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_bias_array, NULL)

/* Tensor #162 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_output, AI_STATIC,
  162, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_output_array, NULL)

/* Tensor #163 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_scale, AI_STATIC,
  163, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_scale_array, NULL)

/* Tensor #164 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  164, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #165 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_output, AI_STATIC,
  165, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 28, 28), AI_STRIDE_INIT(4, 4, 4, 1024, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #166 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  166, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 3, 3), AI_STRIDE_INIT(4, 4, 4, 512, 1536),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #167 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  167, 0x0,
  AI_SHAPE_INIT(4, 128, 3, 3, 256), AI_STRIDE_INIT(4, 4, 512, 131072, 393216),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #168 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  168, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #169 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_output, AI_STATIC,
  169, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #170 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  170, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 3, 3), AI_STRIDE_INIT(4, 4, 4, 1024, 3072),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #171 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  171, 0x0,
  AI_SHAPE_INIT(4, 256, 3, 3, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 786432),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #172 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_add_add0_output, AI_STATIC,
  172, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block1_add_add0_output_array, NULL)

/* Tensor #173 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  173, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #174 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_output, AI_STATIC,
  174, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #175 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  175, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #176 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Neg_10_output, AI_STATIC,
  176, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Neg_10_output_array, NULL)

/* Tensor #177 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu0_output, AI_STATIC,
  177, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu0_output_array, NULL)

/* Tensor #178 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu_10_output, AI_STATIC,
  178, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu_10_output_array, NULL)

/* Tensor #179 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_add0_output, AI_STATIC,
  179, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_add0_output_array, NULL)

/* Tensor #180 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_output, AI_STATIC,
  180, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_output_array, NULL)

/* Tensor #181 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_scale, AI_STATIC,
  181, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_scale_array, NULL)

/* Tensor #182 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  182, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #183 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_output, AI_STATIC,
  183, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #184 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  184, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 3, 3), AI_STRIDE_INIT(4, 4, 4, 1024, 3072),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #185 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  185, 0x0,
  AI_SHAPE_INIT(4, 256, 3, 3, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 786432),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #186 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  186, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #187 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_output, AI_STATIC,
  187, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #188 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  188, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 3, 3), AI_STRIDE_INIT(4, 4, 4, 1024, 3072),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #189 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  189, 0x0,
  AI_SHAPE_INIT(4, 256, 3, 3, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 786432),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #190 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_add_add0_output, AI_STATIC,
  190, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block2_add_add0_output_array, NULL)

/* Tensor #191 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  191, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #192 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_output, AI_STATIC,
  192, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #193 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  193, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #194 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Neg_10_output, AI_STATIC,
  194, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Neg_10_output_array, NULL)

/* Tensor #195 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu0_output, AI_STATIC,
  195, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu0_output_array, NULL)

/* Tensor #196 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu_10_output, AI_STATIC,
  196, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu_10_output_array, NULL)

/* Tensor #197 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_add0_output, AI_STATIC,
  197, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_add0_output_array, NULL)

/* Tensor #198 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_output, AI_STATIC,
  198, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_output_array, NULL)

/* Tensor #199 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_scale, AI_STATIC,
  199, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_scale_array, NULL)

/* Tensor #200 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  200, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #201 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_output, AI_STATIC,
  201, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #202 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  202, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 3, 3), AI_STRIDE_INIT(4, 4, 4, 1024, 3072),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #203 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  203, 0x0,
  AI_SHAPE_INIT(4, 256, 3, 3, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 786432),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #204 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  204, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #205 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_output, AI_STATIC,
  205, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #206 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  206, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 3, 3), AI_STRIDE_INIT(4, 4, 4, 1024, 3072),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #207 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  207, 0x0,
  AI_SHAPE_INIT(4, 256, 3, 3, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 786432),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #208 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_add_add0_output, AI_STATIC,
  208, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block3_add_add0_output_array, NULL)

/* Tensor #209 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  209, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #210 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_output, AI_STATIC,
  210, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #211 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  211, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #212 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Neg_10_output, AI_STATIC,
  212, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Neg_10_output_array, NULL)

/* Tensor #213 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu0_output, AI_STATIC,
  213, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu0_output_array, NULL)

/* Tensor #214 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu_10_output, AI_STATIC,
  214, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu_10_output_array, NULL)

/* Tensor #215 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_add0_output, AI_STATIC,
  215, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_add0_output_array, NULL)

/* Tensor #216 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_output, AI_STATIC,
  216, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_output_array, NULL)

/* Tensor #217 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_scale, AI_STATIC,
  217, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_scale_array, NULL)

/* Tensor #218 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  218, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #219 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_output, AI_STATIC,
  219, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #220 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  220, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 3, 3), AI_STRIDE_INIT(4, 4, 4, 1024, 3072),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #221 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  221, 0x0,
  AI_SHAPE_INIT(4, 256, 3, 3, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 786432),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #222 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  222, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #223 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_output, AI_STATIC,
  223, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #224 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  224, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 3, 3), AI_STRIDE_INIT(4, 4, 4, 1024, 3072),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #225 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  225, 0x0,
  AI_SHAPE_INIT(4, 256, 3, 3, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 786432),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #226 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_add_add0_output, AI_STATIC,
  226, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block4_add_add0_output_array, NULL)

/* Tensor #227 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  227, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #228 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_output, AI_STATIC,
  228, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #229 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  229, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #230 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Neg_10_output, AI_STATIC,
  230, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Neg_10_output_array, NULL)

/* Tensor #231 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu0_output, AI_STATIC,
  231, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu0_output_array, NULL)

/* Tensor #232 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu_10_output, AI_STATIC,
  232, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu_10_output_array, NULL)

/* Tensor #233 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_add0_output, AI_STATIC,
  233, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_add0_output_array, NULL)

/* Tensor #234 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_output, AI_STATIC,
  234, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_output_array, NULL)

/* Tensor #235 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_scale, AI_STATIC,
  235, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_scale_array, NULL)

/* Tensor #236 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  236, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #237 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_output, AI_STATIC,
  237, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #238 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  238, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 3, 3), AI_STRIDE_INIT(4, 4, 4, 1024, 3072),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #239 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  239, 0x0,
  AI_SHAPE_INIT(4, 256, 3, 3, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 786432),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #240 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  240, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #241 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_output, AI_STATIC,
  241, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #242 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  242, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 3, 3), AI_STRIDE_INIT(4, 4, 4, 1024, 3072),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #243 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  243, 0x0,
  AI_SHAPE_INIT(4, 256, 3, 3, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 786432),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #244 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_add_add0_output, AI_STATIC,
  244, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block5_add_add0_output_array, NULL)

/* Tensor #245 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  245, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #246 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_output, AI_STATIC,
  246, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #247 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  247, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #248 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Neg_10_output, AI_STATIC,
  248, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Neg_10_output_array, NULL)

/* Tensor #249 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu0_output, AI_STATIC,
  249, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu0_output_array, NULL)

/* Tensor #250 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu_10_output, AI_STATIC,
  250, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu_10_output_array, NULL)

/* Tensor #251 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_add0_output, AI_STATIC,
  251, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_add0_output_array, NULL)

/* Tensor #252 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_output, AI_STATIC,
  252, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_output_array, NULL)

/* Tensor #253 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_scale, AI_STATIC,
  253, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_scale_array, NULL)

/* Tensor #254 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  254, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #255 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_output, AI_STATIC,
  255, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #256 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  256, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 3, 3), AI_STRIDE_INIT(4, 4, 4, 1024, 3072),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #257 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  257, 0x0,
  AI_SHAPE_INIT(4, 256, 3, 3, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 786432),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #258 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  258, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #259 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_output, AI_STATIC,
  259, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #260 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  260, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 3, 3), AI_STRIDE_INIT(4, 4, 4, 1024, 3072),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #261 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  261, 0x0,
  AI_SHAPE_INIT(4, 256, 3, 3, 256), AI_STRIDE_INIT(4, 4, 1024, 262144, 786432),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #262 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_add_add0_output, AI_STATIC,
  262, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv4_block6_add_add0_output_array, NULL)

/* Tensor #263 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_bias, AI_STATIC,
  263, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #264 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_output, AI_STATIC,
  264, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #265 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  265, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #266 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_weights, AI_STATIC,
  266, 0x0,
  AI_SHAPE_INIT(4, 256, 1, 1, 512), AI_STRIDE_INIT(4, 4, 1024, 524288, 524288),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #267 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  267, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #268 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_output, AI_STATIC,
  268, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 14, 14), AI_STRIDE_INIT(4, 4, 4, 1024, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #269 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  269, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #270 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Neg_10_output, AI_STATIC,
  270, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Neg_10_output_array, NULL)

/* Tensor #271 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu0_output, AI_STATIC,
  271, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu0_output_array, NULL)

/* Tensor #272 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu_10_output, AI_STATIC,
  272, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu_10_output_array, NULL)

/* Tensor #273 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_add0_output, AI_STATIC,
  273, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_add0_output_array, NULL)

/* Tensor #274 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_bias, AI_STATIC,
  274, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_bias_array, NULL)

/* Tensor #275 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_output, AI_STATIC,
  275, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_output_array, NULL)

/* Tensor #276 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_scale, AI_STATIC,
  276, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_scale_array, NULL)

/* Tensor #277 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  277, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #278 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_output, AI_STATIC,
  278, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 14, 14), AI_STRIDE_INIT(4, 4, 4, 2048, 28672),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #279 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  279, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 3, 3), AI_STRIDE_INIT(4, 4, 4, 1024, 3072),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #280 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  280, 0x0,
  AI_SHAPE_INIT(4, 256, 3, 3, 512), AI_STRIDE_INIT(4, 4, 1024, 524288, 1572864),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #281 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  281, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #282 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_output, AI_STATIC,
  282, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #283 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  283, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 3, 3), AI_STRIDE_INIT(4, 4, 4, 2048, 6144),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #284 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  284, 0x0,
  AI_SHAPE_INIT(4, 512, 3, 3, 512), AI_STRIDE_INIT(4, 4, 2048, 1048576, 3145728),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #285 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_add_add0_output, AI_STATIC,
  285, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block1_add_add0_output_array, NULL)

/* Tensor #286 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  286, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #287 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_output, AI_STATIC,
  287, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #288 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  288, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #289 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Neg_10_output, AI_STATIC,
  289, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Neg_10_output_array, NULL)

/* Tensor #290 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu0_output, AI_STATIC,
  290, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu0_output_array, NULL)

/* Tensor #291 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu_10_output, AI_STATIC,
  291, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu_10_output_array, NULL)

/* Tensor #292 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_add0_output, AI_STATIC,
  292, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_add0_output_array, NULL)

/* Tensor #293 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_output, AI_STATIC,
  293, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_output_array, NULL)

/* Tensor #294 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_scale, AI_STATIC,
  294, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_scale_array, NULL)

/* Tensor #295 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  295, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #296 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_output, AI_STATIC,
  296, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #297 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  297, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 3, 3), AI_STRIDE_INIT(4, 4, 4, 2048, 6144),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #298 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  298, 0x0,
  AI_SHAPE_INIT(4, 512, 3, 3, 512), AI_STRIDE_INIT(4, 4, 2048, 1048576, 3145728),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #299 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  299, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #300 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_output, AI_STATIC,
  300, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #301 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  301, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 3, 3), AI_STRIDE_INIT(4, 4, 4, 2048, 6144),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #302 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  302, 0x0,
  AI_SHAPE_INIT(4, 512, 3, 3, 512), AI_STRIDE_INIT(4, 4, 2048, 1048576, 3145728),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #303 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_add_add0_output, AI_STATIC,
  303, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block2_add_add0_output_array, NULL)

/* Tensor #304 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_bias, AI_STATIC,
  304, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #305 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_output, AI_STATIC,
  305, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #306 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_scale, AI_STATIC,
  306, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_scale_array, NULL)

/* Tensor #307 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Neg_10_output, AI_STATIC,
  307, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Neg_10_output_array, NULL)

/* Tensor #308 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu0_output, AI_STATIC,
  308, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu0_output_array, NULL)

/* Tensor #309 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu_10_output, AI_STATIC,
  309, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu_10_output_array, NULL)

/* Tensor #310 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_add0_output, AI_STATIC,
  310, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_add0_output_array, NULL)

/* Tensor #311 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_output, AI_STATIC,
  311, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_output_array, NULL)

/* Tensor #312 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_scale, AI_STATIC,
  312, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_scale_array, NULL)

/* Tensor #313 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_bias, AI_STATIC,
  313, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #314 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_output, AI_STATIC,
  314, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #315 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  315, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 3, 3), AI_STRIDE_INIT(4, 4, 4, 2048, 6144),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #316 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_weights, AI_STATIC,
  316, 0x0,
  AI_SHAPE_INIT(4, 512, 3, 3, 512), AI_STRIDE_INIT(4, 4, 2048, 1048576, 3145728),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #317 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_bias, AI_STATIC,
  317, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_bias_array, NULL)

/* Tensor #318 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_output, AI_STATIC,
  318, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_output_array, NULL)

/* Tensor #319 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_scratch0, AI_STATIC,
  319, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 3, 3), AI_STRIDE_INIT(4, 4, 4, 2048, 6144),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_scratch0_array, NULL)

/* Tensor #320 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_weights, AI_STATIC,
  320, 0x0,
  AI_SHAPE_INIT(4, 512, 3, 3, 512), AI_STRIDE_INIT(4, 4, 2048, 1048576, 3145728),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_weights_array, NULL)

/* Tensor #321 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_add_add0_output, AI_STATIC,
  321, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 7, 7), AI_STRIDE_INIT(4, 4, 4, 2048, 14336),
  1, &StatefulPartitionedCall_ResNet34_conv5_block3_add_add0_output_array, NULL)

/* Tensor #322 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_dense_MatMul0_bias, AI_STATIC,
  322, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_dense_MatMul0_bias_array, NULL)

/* Tensor #323 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_dense_MatMul0_output, AI_STATIC,
  323, 0x0,
  AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 4, 4, 2048, 2048),
  1, &StatefulPartitionedCall_ResNet34_dense_MatMul0_output_array, NULL)

/* Tensor #324 */
AI_TENSOR_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_dense_MatMul0_weights, AI_STATIC,
  324, 0x0,
  AI_SHAPE_INIT(4, 25088, 512, 1, 1), AI_STRIDE_INIT(4, 4, 100352, 51380224, 51380224),
  1, &StatefulPartitionedCall_ResNet34_dense_MatMul0_weights_array, NULL)

/* Tensor #325 */
AI_TENSOR_OBJ_DECLARE(
  input_1_output, AI_STATIC,
  325, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 112, 112), AI_STRIDE_INIT(4, 4, 4, 12, 1344),
  1, &input_1_output_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_dense_MatMul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_dense_MatMul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_dense_MatMul0_weights, &StatefulPartitionedCall_ResNet34_dense_MatMul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_dense_MatMul0_layer, 162,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &StatefulPartitionedCall_ResNet34_dense_MatMul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_dense_MatMul0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_layer, 156,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_dense_MatMul0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block2_add_add0_output, &StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_add_add0_layer, 155,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv5_block3_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_layer, 154,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block3_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_add0_layer, 153,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu0_layer, 149,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_layer, 152,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu_10_layer, 151,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Neg_10_layer, 150,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_layer, 148,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_layer, 147,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block1_add_add0_output, &StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_add_add0_layer, 146,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv5_block2_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_layer, 145,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block2_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_add0_layer, 144,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu0_layer, 140,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_layer, 143,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu_10_layer, 142,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Neg_10_layer, 141,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_layer, 139,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_layer, 138,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_output, &StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_add_add0_layer, 137,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv5_block1_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_layer, 135,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block1_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_add0_layer, 134,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu0_layer, 130,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_layer, 133,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu_10_layer, 132,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Neg_10_layer, 131,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_layer, 129,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_layer, 128,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_layer, 136,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block5_add_add0_output, &StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_add_add0_layer, 127,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv4_block6_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_layer, 126,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block6_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_add0_layer, 125,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu0_layer, 121,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_layer, 124,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu_10_layer, 123,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Neg_10_layer, 122,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_layer, 120,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_layer, 119,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block4_add_add0_output, &StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_add_add0_layer, 118,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv4_block5_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_layer, 117,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block5_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_add0_layer, 116,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu0_layer, 112,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_layer, 115,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu_10_layer, 114,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Neg_10_layer, 113,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_layer, 111,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_layer, 110,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block3_add_add0_output, &StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_add_add0_layer, 109,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv4_block4_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_layer, 108,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block4_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_add0_layer, 107,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu0_layer, 103,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_layer, 106,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu_10_layer, 105,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Neg_10_layer, 104,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_layer, 102,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_layer, 101,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block2_add_add0_output, &StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_add_add0_layer, 100,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv4_block3_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_layer, 99,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block3_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_add0_layer, 98,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu0_layer, 94,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_layer, 97,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu_10_layer, 96,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Neg_10_layer, 95,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_layer, 93,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_layer, 92,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block1_add_add0_output, &StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_add_add0_layer, 91,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv4_block2_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_layer, 90,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block2_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_add0_layer, 89,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu0_layer, 85,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_layer, 88,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu_10_layer, 87,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Neg_10_layer, 86,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_layer, 84,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_layer, 83,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_output, &StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_add_add0_layer, 82,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv4_block1_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_layer, 80,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block1_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_add0_layer, 79,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu0_layer, 75,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_layer, 78,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu_10_layer, 77,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Neg_10_layer, 76,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_layer, 74,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_layer, 73,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_layer, 81,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block3_add_add0_output, &StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_add_add0_layer, 72,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv3_block4_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_layer, 71,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block4_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_add0_layer, 70,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu0_layer, 66,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_layer, 69,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu_10_layer, 68,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Neg_10_layer, 67,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_layer, 65,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_layer, 64,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block2_add_add0_output, &StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_add_add0_layer, 63,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv3_block3_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_layer, 62,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block3_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_add0_layer, 61,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu0_layer, 57,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_layer, 60,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu_10_layer, 59,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Neg_10_layer, 58,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_layer, 56,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_layer, 55,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block1_add_add0_output, &StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_add_add0_layer, 54,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv3_block2_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_layer, 53,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block2_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_add0_layer, 52,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu0_layer, 48,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_layer, 51,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu_10_layer, 50,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Neg_10_layer, 49,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_layer, 47,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_layer, 46,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_output, &StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_add_add0_layer, 45,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv3_block1_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_layer, 43,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block1_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_add0_layer, 42,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu0_layer, 38,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_layer, 41,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu_10_layer, 40,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Neg_10_layer, 39,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_layer, 37,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_layer, 36,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_layer, 44,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block2_add_add0_output, &StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_add_add0_layer, 35,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv2_block3_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_layer, 34,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block3_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_add0_layer, 33,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu0_layer, 29,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_layer, 32,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu_10_layer, 31,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Neg_10_layer, 30,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_layer, 28,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_layer, 27,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block1_add_add0_output, &StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_add_add0_layer, 26,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv2_block2_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_layer, 25,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block2_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_add0_layer, 24,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu0_layer, 20,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_layer, 23,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu_10_layer, 22,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Neg_10_layer, 21,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_layer, 19,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_add_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_layer, 18,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_add_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_output, &StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_add_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_add_add0_layer, 17,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv2_block1_add_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_layer, 15,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block1_add_add0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_add0_layer, 14,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu0_layer, 10,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_layer, 13,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu_10_layer, 12,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Neg_10_layer, 11,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_layer, 9,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_scale, &StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_layer, 8,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_layer, 16,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_add0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv1_prelu_Relu0_output, &StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv1_prelu_add0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_add0_layer, 7,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &StatefulPartitionedCall_ResNet34_conv1_prelu_add0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_Relu0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv1_prelu_Relu0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_Relu0_layer, 3,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv1_prelu_Relu0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv1_prelu_add0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_scale, &StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_layer, 6,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn,
  &StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv1_prelu_Relu0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_Relu_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv1_prelu_Relu_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_Relu_10_layer, 5,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &StatefulPartitionedCall_ResNet34_conv1_prelu_Relu_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_Neg_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv1_prelu_Neg_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_prelu_Neg_10_layer, 4,
  NL_TYPE, 0x0, NULL,
  nl, forward_neg,
  &StatefulPartitionedCall_ResNet34_conv1_prelu_Neg_10_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv1_prelu_Relu_10_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_weights, &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_layer, 2,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_chain,
  NULL, &StatefulPartitionedCall_ResNet34_conv1_prelu_Neg_10_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 136522752, 1, 1),
    136522752, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 7227648, 1, 1),
    7227648, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_FACEREC_IN_NUM, &input_1_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_FACEREC_OUT_NUM, &StatefulPartitionedCall_ResNet34_dense_MatMul0_output),
  &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_layer, 0x0f6cbd73, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 136522752, 1, 1),
      136522752, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 7227648, 1, 1),
      7227648, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_FACEREC_IN_NUM, &input_1_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_FACEREC_OUT_NUM, &StatefulPartitionedCall_ResNet34_dense_MatMul0_output),
  &StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_layer, 0x0f6cbd73, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_facerec_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_network_facerec_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    input_1_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 3863444);
    input_1_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 3863444);
    StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 4013972);
    StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 4013972);
    StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 7225344);
    StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 7225344);
    StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 3211264);
    StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 3211264);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 7225344);
    StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 7225344);
    StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 4014080);
    StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 4016384);
    StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 4016384);
    StatefulPartitionedCall_ResNet34_conv2_block1_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block1_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1605632);
    StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1605632);
    StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1607936);
    StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1607936);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 2410752);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 2410752);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 2410752);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 2410752);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1605632);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1605632);
    StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 2408448);
    StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 2408448);
    StatefulPartitionedCall_ResNet34_conv2_block2_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv2_block2_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1605632);
    StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1605632);
    StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1607936);
    StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1607936);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 2410752);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 2410752);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 2410752);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 2410752);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1605632);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1605632);
    StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 2408448);
    StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 2408448);
    StatefulPartitionedCall_ResNet34_conv2_block3_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv2_block3_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 803072);
    StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 803072);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1204480);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1204480);
    StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 2007296);
    StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 2007296);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 3612928);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 3612928);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 5218560);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 5218560);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 3612928);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 3612928);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 5218560);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 5218560);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1204480);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1204480);
    StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 4608);
    StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 4608);
    StatefulPartitionedCall_ResNet34_conv3_block1_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1204480);
    StatefulPartitionedCall_ResNet34_conv3_block1_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1204480);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 406016);
    StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 406016);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1605888);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1605888);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1605888);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1605888);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv3_block2_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block2_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 807424);
    StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 807424);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1208832);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1208832);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1208832);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1208832);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1204224);
    StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1204224);
    StatefulPartitionedCall_ResNet34_conv3_block3_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block3_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 807424);
    StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 807424);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1208832);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1208832);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1208832);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1208832);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 802816);
    StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1204224);
    StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1204224);
    StatefulPartitionedCall_ResNet34_conv3_block4_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv3_block4_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401920);
    StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401920);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 602624);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 602624);
    StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1004032);
    StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1004032);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1806848);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1806848);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 2609664);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 2609664);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1806848);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1806848);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 2609664);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 2609664);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 602624);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 602624);
    StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 9216);
    StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 9216);
    StatefulPartitionedCall_ResNet34_conv4_block1_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 602624);
    StatefulPartitionedCall_ResNet34_conv4_block1_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 602624);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 209920);
    StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 209920);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 803328);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 803328);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 803328);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 803328);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block2_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block2_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 410624);
    StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 410624);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 602112);
    StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 602112);
    StatefulPartitionedCall_ResNet34_conv4_block3_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block3_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 410624);
    StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 410624);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 602112);
    StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 602112);
    StatefulPartitionedCall_ResNet34_conv4_block4_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block4_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 410624);
    StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 410624);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 602112);
    StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 602112);
    StatefulPartitionedCall_ResNet34_conv4_block5_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block5_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 410624);
    StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 410624);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 611328);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 401408);
    StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 602112);
    StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 602112);
    StatefulPartitionedCall_ResNet34_conv4_block6_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv4_block6_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 201728);
    StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 201728);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 302080);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 302080);
    StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 502784);
    StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 502784);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 904192);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 904192);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1305600);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1305600);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 904192);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 904192);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 1305600);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 1305600);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 302080);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 302080);
    StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 18432);
    StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 18432);
    StatefulPartitionedCall_ResNet34_conv5_block1_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 302080);
    StatefulPartitionedCall_ResNet34_conv5_block1_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 302080);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 118784);
    StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 118784);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 402432);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 402432);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 402432);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 402432);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv5_block2_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block2_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 219136);
    StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 219136);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Neg_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Neg_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu_10_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 319488);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu_10_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 319488);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 319488);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_Relu0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 319488);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 200704);
    StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_scratch0_array.data = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_scratch0_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 301056);
    StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 301056);
    StatefulPartitionedCall_ResNet34_conv5_block3_add_add0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_conv5_block3_add_add0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 0);
    StatefulPartitionedCall_ResNet34_dense_MatMul0_output_array.data = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    StatefulPartitionedCall_ResNet34_dense_MatMul0_output_array.data_start = AI_PTR(g_network_facerec_activations_map[0] + 100352);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_facerec_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_network_facerec_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 0);
    StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 6912);
    StatefulPartitionedCall_ResNet34_conv1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 6912);
    StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 7168);
    StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 7168);
    StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 7424);
    StatefulPartitionedCall_ResNet34_conv1_prelu_mul0_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 7424);
    StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 7680);
    StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 7680);
    StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 24064);
    StatefulPartitionedCall_ResNet34_conv2_block1_0_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 24064);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 24320);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 24320);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 24576);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 24576);
    StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 24832);
    StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 24832);
    StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 172288);
    StatefulPartitionedCall_ResNet34_conv2_block1_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 172288);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 172544);
    StatefulPartitionedCall_ResNet34_conv2_block1_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 172544);
    StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 172800);
    StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 172800);
    StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 320256);
    StatefulPartitionedCall_ResNet34_conv2_block1_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 320256);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 320512);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 320512);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 320768);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 320768);
    StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 321024);
    StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 321024);
    StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 468480);
    StatefulPartitionedCall_ResNet34_conv2_block2_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 468480);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 468736);
    StatefulPartitionedCall_ResNet34_conv2_block2_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 468736);
    StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 468992);
    StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 468992);
    StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 616448);
    StatefulPartitionedCall_ResNet34_conv2_block2_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 616448);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 616704);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 616704);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 616960);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 616960);
    StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 617216);
    StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 617216);
    StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 764672);
    StatefulPartitionedCall_ResNet34_conv2_block3_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 764672);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 764928);
    StatefulPartitionedCall_ResNet34_conv2_block3_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 764928);
    StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 765184);
    StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 765184);
    StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 912640);
    StatefulPartitionedCall_ResNet34_conv2_block3_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 912640);
    StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 912896);
    StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 912896);
    StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 945664);
    StatefulPartitionedCall_ResNet34_conv3_block1_0_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 945664);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 946176);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 946176);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 946432);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 946432);
    StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 946688);
    StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 946688);
    StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 1241600);
    StatefulPartitionedCall_ResNet34_conv3_block1_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 1241600);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 1242112);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 1242112);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 1242624);
    StatefulPartitionedCall_ResNet34_conv3_block1_1_prelu_mul0_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 1242624);
    StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 1243136);
    StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 1243136);
    StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 1832960);
    StatefulPartitionedCall_ResNet34_conv3_block1_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 1832960);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 1833472);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 1833472);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 1833984);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 1833984);
    StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 1834496);
    StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 1834496);
    StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 2424320);
    StatefulPartitionedCall_ResNet34_conv3_block2_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 2424320);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 2424832);
    StatefulPartitionedCall_ResNet34_conv3_block2_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 2424832);
    StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 2425344);
    StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 2425344);
    StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 3015168);
    StatefulPartitionedCall_ResNet34_conv3_block2_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 3015168);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 3015680);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 3015680);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 3016192);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 3016192);
    StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 3016704);
    StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 3016704);
    StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 3606528);
    StatefulPartitionedCall_ResNet34_conv3_block3_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 3606528);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 3607040);
    StatefulPartitionedCall_ResNet34_conv3_block3_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 3607040);
    StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 3607552);
    StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 3607552);
    StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 4197376);
    StatefulPartitionedCall_ResNet34_conv3_block3_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 4197376);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 4197888);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 4197888);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 4198400);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 4198400);
    StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 4198912);
    StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 4198912);
    StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 4788736);
    StatefulPartitionedCall_ResNet34_conv3_block4_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 4788736);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 4789248);
    StatefulPartitionedCall_ResNet34_conv3_block4_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 4789248);
    StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 4789760);
    StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 4789760);
    StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 5379584);
    StatefulPartitionedCall_ResNet34_conv3_block4_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 5379584);
    StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 5380096);
    StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 5380096);
    StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 5511168);
    StatefulPartitionedCall_ResNet34_conv4_block1_0_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 5511168);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 5512192);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 5512192);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 5512704);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 5512704);
    StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 5513216);
    StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 5513216);
    StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 6692864);
    StatefulPartitionedCall_ResNet34_conv4_block1_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 6692864);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 6693888);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 6693888);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 6694912);
    StatefulPartitionedCall_ResNet34_conv4_block1_1_prelu_mul0_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 6694912);
    StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 6695936);
    StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 6695936);
    StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 9055232);
    StatefulPartitionedCall_ResNet34_conv4_block1_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 9055232);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 9056256);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 9056256);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 9057280);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 9057280);
    StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 9058304);
    StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 9058304);
    StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 11417600);
    StatefulPartitionedCall_ResNet34_conv4_block2_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 11417600);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 11418624);
    StatefulPartitionedCall_ResNet34_conv4_block2_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 11418624);
    StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 11419648);
    StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 11419648);
    StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 13778944);
    StatefulPartitionedCall_ResNet34_conv4_block2_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 13778944);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 13779968);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 13779968);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 13780992);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 13780992);
    StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 13782016);
    StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 13782016);
    StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 16141312);
    StatefulPartitionedCall_ResNet34_conv4_block3_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 16141312);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 16142336);
    StatefulPartitionedCall_ResNet34_conv4_block3_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 16142336);
    StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 16143360);
    StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 16143360);
    StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 18502656);
    StatefulPartitionedCall_ResNet34_conv4_block3_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 18502656);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 18503680);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 18503680);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 18504704);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 18504704);
    StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 18505728);
    StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 18505728);
    StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 20865024);
    StatefulPartitionedCall_ResNet34_conv4_block4_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 20865024);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 20866048);
    StatefulPartitionedCall_ResNet34_conv4_block4_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 20866048);
    StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 20867072);
    StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 20867072);
    StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 23226368);
    StatefulPartitionedCall_ResNet34_conv4_block4_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 23226368);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 23227392);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 23227392);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 23228416);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 23228416);
    StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 23229440);
    StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 23229440);
    StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 25588736);
    StatefulPartitionedCall_ResNet34_conv4_block5_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 25588736);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 25589760);
    StatefulPartitionedCall_ResNet34_conv4_block5_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 25589760);
    StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 25590784);
    StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 25590784);
    StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 27950080);
    StatefulPartitionedCall_ResNet34_conv4_block5_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 27950080);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 27951104);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 27951104);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 27952128);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 27952128);
    StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 27953152);
    StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 27953152);
    StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 30312448);
    StatefulPartitionedCall_ResNet34_conv4_block6_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 30312448);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 30313472);
    StatefulPartitionedCall_ResNet34_conv4_block6_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 30313472);
    StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 30314496);
    StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 30314496);
    StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 32673792);
    StatefulPartitionedCall_ResNet34_conv4_block6_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 32673792);
    StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 32674816);
    StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 32674816);
    StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 33199104);
    StatefulPartitionedCall_ResNet34_conv5_block1_0_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 33199104);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 33201152);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 33201152);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 33202176);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 33202176);
    StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 33203200);
    StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 33203200);
    StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 37921792);
    StatefulPartitionedCall_ResNet34_conv5_block1_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 37921792);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 37923840);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 37923840);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 37925888);
    StatefulPartitionedCall_ResNet34_conv5_block1_1_prelu_mul0_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 37925888);
    StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 37927936);
    StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 37927936);
    StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 47365120);
    StatefulPartitionedCall_ResNet34_conv5_block1_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 47365120);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 47367168);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 47367168);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 47369216);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 47369216);
    StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 47371264);
    StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 47371264);
    StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 56808448);
    StatefulPartitionedCall_ResNet34_conv5_block2_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 56808448);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 56810496);
    StatefulPartitionedCall_ResNet34_conv5_block2_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 56810496);
    StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 56812544);
    StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 56812544);
    StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 66249728);
    StatefulPartitionedCall_ResNet34_conv5_block2_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 66249728);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 66251776);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 66251776);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 66253824);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 66253824);
    StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 66255872);
    StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 66255872);
    StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 75693056);
    StatefulPartitionedCall_ResNet34_conv5_block3_2_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 75693056);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 75695104);
    StatefulPartitionedCall_ResNet34_conv5_block3_1_prelu_mul0_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 75695104);
    StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 75697152);
    StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 75697152);
    StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 85134336);
    StatefulPartitionedCall_ResNet34_conv5_block3_3_bn_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 85134336);
    StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_scale_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_scale_array.data = AI_PTR(g_network_facerec_weights_map[0] + 85136384);
    StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_scale_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 85136384);
    StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 85138432);
    StatefulPartitionedCall_ResNet34_batch_normalization_FusedBatchNormV30_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 85138432);
    StatefulPartitionedCall_ResNet34_dense_MatMul0_weights_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_dense_MatMul0_weights_array.data = AI_PTR(g_network_facerec_weights_map[0] + 85140480);
    StatefulPartitionedCall_ResNet34_dense_MatMul0_weights_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 85140480);
    StatefulPartitionedCall_ResNet34_dense_MatMul0_bias_array.format |= AI_FMT_FLAG_CONST;
    StatefulPartitionedCall_ResNet34_dense_MatMul0_bias_array.data = AI_PTR(g_network_facerec_weights_map[0] + 136520704);
    StatefulPartitionedCall_ResNet34_dense_MatMul0_bias_array.data_start = AI_PTR(g_network_facerec_weights_map[0] + 136520704);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_network_facerec_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_FACEREC_MODEL_NAME,
      .model_signature   = AI_NETWORK_FACEREC_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 4485266816,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0f6cbd73,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_network_facerec_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_FACEREC_MODEL_NAME,
      .model_signature   = AI_NETWORK_FACEREC_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 4485266816,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0f6cbd73,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_network_facerec_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_network_facerec_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_network_facerec_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_network_facerec_create(network, AI_NETWORK_FACEREC_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_network_facerec_data_params_get(&params) != true) {
    err = ai_network_facerec_get_error(*network);
    return err;
  }
#if defined(AI_NETWORK_FACEREC_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_NETWORK_FACEREC_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_network_facerec_init(*network, &params) != true) {
    err = ai_network_facerec_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_network_facerec_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_network_facerec_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_network_facerec_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_network_facerec_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= network_facerec_configure_weights(net_ctx, params);
  ok &= network_facerec_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_facerec_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_network_facerec_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_NETWORK_FACEREC_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

