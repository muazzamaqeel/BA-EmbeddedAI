/**
  ******************************************************************************
  * @file    face_recognition.h
  * @author  STEdgeAI
  * @date    2025-09-10 17:33:44
  * @brief   Minimal description of the generated c-implemention of the network
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
#ifndef LL_ATON_FACE_RECOGNITION_H
#define LL_ATON_FACE_RECOGNITION_H

/******************************************************************************/
#define LL_ATON_FACE_RECOGNITION_C_MODEL_NAME        "face_recognition"
#define LL_ATON_FACE_RECOGNITION_ORIGIN_MODEL_NAME   "facenet_512_int_quant"

/************************** USER ALLOCATED IOs ********************************/
#define LL_ATON_FACE_RECOGNITION_USER_ALLOCATED_INPUTS   (1)  // Number of input buffers not allocated by the compiler
#define LL_ATON_FACE_RECOGNITION_USER_ALLOCATED_OUTPUTS  (1)  // Number of output buffers not allocated by the compiler

/************************** INPUTS ********************************************/
#define LL_ATON_FACE_RECOGNITION_IN_NUM        (1)    // Total number of input buffers
// Input buffer 1 -- Input_0_out_0
#define LL_ATON_FACE_RECOGNITION_IN_1_ALIGNMENT   (32)
#define LL_ATON_FACE_RECOGNITION_IN_1_SIZE_BYTES  (76800)

/************************** OUTPUTS *******************************************/
#define LL_ATON_FACE_RECOGNITION_OUT_NUM        (1)    // Total number of output buffers
// Output buffer 1 -- Quantize_677_out_0
#define LL_ATON_FACE_RECOGNITION_OUT_1_ALIGNMENT   (32)
#define LL_ATON_FACE_RECOGNITION_OUT_1_SIZE_BYTES  (512)

#endif /* LL_ATON_FACE_RECOGNITION_H */
