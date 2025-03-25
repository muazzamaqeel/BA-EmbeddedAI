#!/bin/bash

# generate yolox model
stedgeai generate --no-inputs-allocation --no-outputs-allocation --model st_yolo_x_nano_480_1.0_0.25_3_st_int8.tflite --target stm32n6 --st-neural-art default@user_neuralart.json
cp st_ai_output/network_ecblobs.h .
cp st_ai_output/network.c .
cp st_ai_output/network_atonbuf.xSPI2.raw network_data.xSPI2.bin
arm-none-eabi-objcopy -I binary network_data.xSPI2.bin --change-addresses 0x70380000 -O ihex network_data.hex

# generate yolov2 model for nucleo
stedgeai generate --no-inputs-allocation --no-outputs-allocation --model nucleo/quantized_tiny_yolo_v2_224_.tflite --target stm32n6 --st-neural-art default@nucleo/user_neuralart.json
cp st_ai_output/network_ecblobs.h nucleo/.
cp st_ai_output/network.c nucleo/.
cp st_ai_output/network.c nucleo/.
cp st_ai_output/network_atonbuf.xSPI2.raw nucleo/network_data.xSPI2.bin
arm-none-eabi-objcopy -I binary nucleo/network_data.xSPI2.bin --change-addresses 0x70380000 -O ihex nucleo/network_data.hex
