/**
  ******************************************************************************
  * @file    network.h
  * @date    2025-08-27T19:19:17+0200
  * @brief   ST.AI Tool Automatic Code Generator for Embedded NN computing
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
#ifndef STAI_NETWORK_DETAILS_H
#define STAI_NETWORK_DETAILS_H

#include "stai.h"
#include "layers.h"

const stai_network_details g_network_details = {
  .tensors = (const stai_tensor[99]) {
   { .size_bytes = 150528, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 3, 112, 112}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "input_1_output" },
   { .size_bytes = 150528, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 112, 112, 3}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "input_1_Transpose_output" },
   { .size_bytes = 1605632, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 56, 56, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_517_output" },
   { .size_bytes = 1605632, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 56, 56, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_339_output" },
   { .size_bytes = 1605632, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 56, 56, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_520_output" },
   { .size_bytes = 1605632, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 56, 56, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_343_output" },
   { .size_bytes = 1605632, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 56, 56, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_523_output" },
   { .size_bytes = 1605632, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 56, 56, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_347_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_526_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_351_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_529_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_532_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_357_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_535_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_361_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_538_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_364_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_541_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_368_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_544_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_372_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_547_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_375_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_550_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_379_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_553_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_383_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_556_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_386_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_559_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_390_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_562_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_394_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_565_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 128}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_397_output" },
   { .size_bytes = 802816, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_568_output" },
   { .size_bytes = 802816, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 28, 28, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_401_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_571_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_405_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_574_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_577_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_411_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_580_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_415_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_583_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_418_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_586_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_422_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_589_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_426_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_592_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_429_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_595_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_433_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_598_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_437_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_601_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_440_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_604_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_444_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_607_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_448_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_610_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_451_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_613_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_455_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_616_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_459_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_619_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_462_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_622_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_466_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_625_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_470_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_628_output" },
   { .size_bytes = 200704, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_473_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 512}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_631_output" },
   { .size_bytes = 401408, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 14, 14, 512}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_477_output" },
   { .size_bytes = 100352, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 512}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_634_output" },
   { .size_bytes = 100352, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 512}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_481_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_637_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_640_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_487_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_643_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_491_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_646_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_494_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_649_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_498_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_652_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_502_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_655_output" },
   { .size_bytes = 50176, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 256}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_505_output" },
   { .size_bytes = 100352, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 512}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_658_output" },
   { .size_bytes = 100352, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 512}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_509_output" },
   { .size_bytes = 12544, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 64}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_661_output" },
   { .size_bytes = 12544, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 7, 7, 64}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_513_output" },
   { .size_bytes = 12544, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {4, (const int32_t[4]){1, 64, 7, 7}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_514_to_chlast_output" },
   { .size_bytes = 2048, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {2, (const int32_t[2]){1, 512}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_515_output" }
  },
  .nodes = (const stai_node_details[98]){
    {.id = 2, .type = AI_LAYER_TRANSPOSE_TYPE, .input_tensors = {1, (const int32_t[1]){0}}, .output_tensors = {1, (const int32_t[1]){1}} }, /* input_1_Transpose */
    {.id = 1, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){1}}, .output_tensors = {1, (const int32_t[1]){2}} }, /* node_517 */
    {.id = 2, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){2}}, .output_tensors = {1, (const int32_t[1]){3}} }, /* node_339 */
    {.id = 3, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){3}}, .output_tensors = {1, (const int32_t[1]){4}} }, /* node_520 */
    {.id = 4, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){4}}, .output_tensors = {1, (const int32_t[1]){5}} }, /* node_343 */
    {.id = 5, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){5}}, .output_tensors = {1, (const int32_t[1]){6}} }, /* node_523 */
    {.id = 6, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){6}}, .output_tensors = {1, (const int32_t[1]){7}} }, /* node_347 */
    {.id = 7, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){7}}, .output_tensors = {1, (const int32_t[1]){8}} }, /* node_526 */
    {.id = 8, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){8}}, .output_tensors = {1, (const int32_t[1]){9}} }, /* node_351 */
    {.id = 9, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){9}}, .output_tensors = {1, (const int32_t[1]){10}} }, /* node_529 */
    {.id = 10, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){10}}, .output_tensors = {1, (const int32_t[1]){11}} }, /* node_532 */
    {.id = 11, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){11}}, .output_tensors = {1, (const int32_t[1]){12}} }, /* node_357 */
    {.id = 12, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){12}}, .output_tensors = {1, (const int32_t[1]){13}} }, /* node_535 */
    {.id = 13, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){13}}, .output_tensors = {1, (const int32_t[1]){14}} }, /* node_361 */
    {.id = 14, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){14}}, .output_tensors = {1, (const int32_t[1]){15}} }, /* node_538 */
    {.id = 15, .type = AI_LAYER_ELTWISE_TYPE, .input_tensors = {2, (const int32_t[2]){10, 15}}, .output_tensors = {1, (const int32_t[1]){16}} }, /* node_364 */
    {.id = 16, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){16}}, .output_tensors = {1, (const int32_t[1]){17}} }, /* node_541 */
    {.id = 17, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){17}}, .output_tensors = {1, (const int32_t[1]){18}} }, /* node_368 */
    {.id = 18, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){18}}, .output_tensors = {1, (const int32_t[1]){19}} }, /* node_544 */
    {.id = 19, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){19}}, .output_tensors = {1, (const int32_t[1]){20}} }, /* node_372 */
    {.id = 20, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){20}}, .output_tensors = {1, (const int32_t[1]){21}} }, /* node_547 */
    {.id = 21, .type = AI_LAYER_ELTWISE_TYPE, .input_tensors = {2, (const int32_t[2]){16, 21}}, .output_tensors = {1, (const int32_t[1]){22}} }, /* node_375 */
    {.id = 22, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){22}}, .output_tensors = {1, (const int32_t[1]){23}} }, /* node_550 */
    {.id = 23, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){23}}, .output_tensors = {1, (const int32_t[1]){24}} }, /* node_379 */
    {.id = 24, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){24}}, .output_tensors = {1, (const int32_t[1]){25}} }, /* node_553 */
    {.id = 25, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){25}}, .output_tensors = {1, (const int32_t[1]){26}} }, /* node_383 */
    {.id = 26, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){26}}, .output_tensors = {1, (const int32_t[1]){27}} }, /* node_556 */
    {.id = 27, .type = AI_LAYER_ELTWISE_TYPE, .input_tensors = {2, (const int32_t[2]){22, 27}}, .output_tensors = {1, (const int32_t[1]){28}} }, /* node_386 */
    {.id = 28, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){28}}, .output_tensors = {1, (const int32_t[1]){29}} }, /* node_559 */
    {.id = 29, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){29}}, .output_tensors = {1, (const int32_t[1]){30}} }, /* node_390 */
    {.id = 30, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){30}}, .output_tensors = {1, (const int32_t[1]){31}} }, /* node_562 */
    {.id = 31, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){31}}, .output_tensors = {1, (const int32_t[1]){32}} }, /* node_394 */
    {.id = 32, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){32}}, .output_tensors = {1, (const int32_t[1]){33}} }, /* node_565 */
    {.id = 33, .type = AI_LAYER_ELTWISE_TYPE, .input_tensors = {2, (const int32_t[2]){28, 33}}, .output_tensors = {1, (const int32_t[1]){34}} }, /* node_397 */
    {.id = 34, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){34}}, .output_tensors = {1, (const int32_t[1]){35}} }, /* node_568 */
    {.id = 35, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){35}}, .output_tensors = {1, (const int32_t[1]){36}} }, /* node_401 */
    {.id = 36, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){36}}, .output_tensors = {1, (const int32_t[1]){37}} }, /* node_571 */
    {.id = 37, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){37}}, .output_tensors = {1, (const int32_t[1]){38}} }, /* node_405 */
    {.id = 38, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){38}}, .output_tensors = {1, (const int32_t[1]){39}} }, /* node_574 */
    {.id = 39, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){39}}, .output_tensors = {1, (const int32_t[1]){40}} }, /* node_577 */
    {.id = 40, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){40}}, .output_tensors = {1, (const int32_t[1]){41}} }, /* node_411 */
    {.id = 41, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){41}}, .output_tensors = {1, (const int32_t[1]){42}} }, /* node_580 */
    {.id = 42, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){42}}, .output_tensors = {1, (const int32_t[1]){43}} }, /* node_415 */
    {.id = 43, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){43}}, .output_tensors = {1, (const int32_t[1]){44}} }, /* node_583 */
    {.id = 44, .type = AI_LAYER_ELTWISE_TYPE, .input_tensors = {2, (const int32_t[2]){39, 44}}, .output_tensors = {1, (const int32_t[1]){45}} }, /* node_418 */
    {.id = 45, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){45}}, .output_tensors = {1, (const int32_t[1]){46}} }, /* node_586 */
    {.id = 46, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){46}}, .output_tensors = {1, (const int32_t[1]){47}} }, /* node_422 */
    {.id = 47, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){47}}, .output_tensors = {1, (const int32_t[1]){48}} }, /* node_589 */
    {.id = 48, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){48}}, .output_tensors = {1, (const int32_t[1]){49}} }, /* node_426 */
    {.id = 49, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){49}}, .output_tensors = {1, (const int32_t[1]){50}} }, /* node_592 */
    {.id = 50, .type = AI_LAYER_ELTWISE_TYPE, .input_tensors = {2, (const int32_t[2]){45, 50}}, .output_tensors = {1, (const int32_t[1]){51}} }, /* node_429 */
    {.id = 51, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){51}}, .output_tensors = {1, (const int32_t[1]){52}} }, /* node_595 */
    {.id = 52, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){52}}, .output_tensors = {1, (const int32_t[1]){53}} }, /* node_433 */
    {.id = 53, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){53}}, .output_tensors = {1, (const int32_t[1]){54}} }, /* node_598 */
    {.id = 54, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){54}}, .output_tensors = {1, (const int32_t[1]){55}} }, /* node_437 */
    {.id = 55, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){55}}, .output_tensors = {1, (const int32_t[1]){56}} }, /* node_601 */
    {.id = 56, .type = AI_LAYER_ELTWISE_TYPE, .input_tensors = {2, (const int32_t[2]){51, 56}}, .output_tensors = {1, (const int32_t[1]){57}} }, /* node_440 */
    {.id = 57, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){57}}, .output_tensors = {1, (const int32_t[1]){58}} }, /* node_604 */
    {.id = 58, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){58}}, .output_tensors = {1, (const int32_t[1]){59}} }, /* node_444 */
    {.id = 59, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){59}}, .output_tensors = {1, (const int32_t[1]){60}} }, /* node_607 */
    {.id = 60, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){60}}, .output_tensors = {1, (const int32_t[1]){61}} }, /* node_448 */
    {.id = 61, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){61}}, .output_tensors = {1, (const int32_t[1]){62}} }, /* node_610 */
    {.id = 62, .type = AI_LAYER_ELTWISE_TYPE, .input_tensors = {2, (const int32_t[2]){57, 62}}, .output_tensors = {1, (const int32_t[1]){63}} }, /* node_451 */
    {.id = 63, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){63}}, .output_tensors = {1, (const int32_t[1]){64}} }, /* node_613 */
    {.id = 64, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){64}}, .output_tensors = {1, (const int32_t[1]){65}} }, /* node_455 */
    {.id = 65, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){65}}, .output_tensors = {1, (const int32_t[1]){66}} }, /* node_616 */
    {.id = 66, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){66}}, .output_tensors = {1, (const int32_t[1]){67}} }, /* node_459 */
    {.id = 67, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){67}}, .output_tensors = {1, (const int32_t[1]){68}} }, /* node_619 */
    {.id = 68, .type = AI_LAYER_ELTWISE_TYPE, .input_tensors = {2, (const int32_t[2]){63, 68}}, .output_tensors = {1, (const int32_t[1]){69}} }, /* node_462 */
    {.id = 69, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){69}}, .output_tensors = {1, (const int32_t[1]){70}} }, /* node_622 */
    {.id = 70, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){70}}, .output_tensors = {1, (const int32_t[1]){71}} }, /* node_466 */
    {.id = 71, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){71}}, .output_tensors = {1, (const int32_t[1]){72}} }, /* node_625 */
    {.id = 72, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){72}}, .output_tensors = {1, (const int32_t[1]){73}} }, /* node_470 */
    {.id = 73, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){73}}, .output_tensors = {1, (const int32_t[1]){74}} }, /* node_628 */
    {.id = 74, .type = AI_LAYER_ELTWISE_TYPE, .input_tensors = {2, (const int32_t[2]){69, 74}}, .output_tensors = {1, (const int32_t[1]){75}} }, /* node_473 */
    {.id = 75, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){75}}, .output_tensors = {1, (const int32_t[1]){76}} }, /* node_631 */
    {.id = 76, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){76}}, .output_tensors = {1, (const int32_t[1]){77}} }, /* node_477 */
    {.id = 77, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){77}}, .output_tensors = {1, (const int32_t[1]){78}} }, /* node_634 */
    {.id = 78, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){78}}, .output_tensors = {1, (const int32_t[1]){79}} }, /* node_481 */
    {.id = 79, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){79}}, .output_tensors = {1, (const int32_t[1]){80}} }, /* node_637 */
    {.id = 80, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){80}}, .output_tensors = {1, (const int32_t[1]){81}} }, /* node_640 */
    {.id = 81, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){81}}, .output_tensors = {1, (const int32_t[1]){82}} }, /* node_487 */
    {.id = 82, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){82}}, .output_tensors = {1, (const int32_t[1]){83}} }, /* node_643 */
    {.id = 83, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){83}}, .output_tensors = {1, (const int32_t[1]){84}} }, /* node_491 */
    {.id = 84, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){84}}, .output_tensors = {1, (const int32_t[1]){85}} }, /* node_646 */
    {.id = 85, .type = AI_LAYER_ELTWISE_TYPE, .input_tensors = {2, (const int32_t[2]){80, 85}}, .output_tensors = {1, (const int32_t[1]){86}} }, /* node_494 */
    {.id = 86, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){86}}, .output_tensors = {1, (const int32_t[1]){87}} }, /* node_649 */
    {.id = 87, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){87}}, .output_tensors = {1, (const int32_t[1]){88}} }, /* node_498 */
    {.id = 88, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){88}}, .output_tensors = {1, (const int32_t[1]){89}} }, /* node_652 */
    {.id = 89, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){89}}, .output_tensors = {1, (const int32_t[1]){90}} }, /* node_502 */
    {.id = 90, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){90}}, .output_tensors = {1, (const int32_t[1]){91}} }, /* node_655 */
    {.id = 91, .type = AI_LAYER_ELTWISE_TYPE, .input_tensors = {2, (const int32_t[2]){86, 91}}, .output_tensors = {1, (const int32_t[1]){92}} }, /* node_505 */
    {.id = 92, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){92}}, .output_tensors = {1, (const int32_t[1]){93}} }, /* node_658 */
    {.id = 93, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){93}}, .output_tensors = {1, (const int32_t[1]){94}} }, /* node_509 */
    {.id = 94, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){94}}, .output_tensors = {1, (const int32_t[1]){95}} }, /* node_661 */
    {.id = 95, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){95}}, .output_tensors = {1, (const int32_t[1]){96}} }, /* node_513 */
    {.id = 96, .type = AI_LAYER_TRANSPOSE_TYPE, .input_tensors = {1, (const int32_t[1]){96}}, .output_tensors = {1, (const int32_t[1]){97}} }, /* node_514_to_chlast */
    {.id = 98, .type = AI_LAYER_DENSE_TYPE, .input_tensors = {1, (const int32_t[1]){97}}, .output_tensors = {1, (const int32_t[1]){98}} } /* node_515 */
  },
  .n_nodes = 98
};
#endif