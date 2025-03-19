# Deploy Your TFLite Model

To run your own object detection model, follow these steps:

- [1. Generate C-Model from TFLite Model](#1-generate-c-model-from-tflite-model)
- [2. Program Your Network Data](#2-program-your-network-data)
- [3. Provide NN Information in `app_config.h`](#3-provide-nn-information-in-app_configh)
  - [Post Process Type](#post-process-type)
  - [Configure Post Process Parameters Through User File](#configure-post-process-parameters-through-user-file)
  - [NN Size Info](#nn-size-info)
  - [Class Labels](#class-labels)
  - [Display Delay](#display-delay)
- [4. Build Application](#4-build-application)
- [5. Run Application](#5-run-application)
  - [Dev Mode](#dev-mode)
  - [Boot from Flash](#boot-from-flash)

## 1. Generate C-Model from TFLite Model

To generate the `network.c`, `network_ecblobs.h` and the file containing network parameters, you must install STM32Cube.AI

1. Add `<folderInstall>/Utilities/<your_os>/` to your path to have `stedgeai` known by your bash.

2. Add `<stm32cubeide_folderInstall>/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-<plugin_version>/tools/bin` to your path to have `arm-none-eabi-objcopy` known by your bash.

```bash
cd Model
stedgeai generate --no-inputs-allocation --no-outputs-allocation --model quantized_tiny_yolo_v2_224_.tflite --target stm32n6 --st-neural-art default@user_neuralart.json
cp st_ai_output/network_ecblobs.h .
cp st_ai_output/network.c .
cp st_ai_output/network_atonbuf.xSPI2.raw network_data.xSPI2.bin
arm-none-eabi-objcopy -I binary network_data.xSPI2.bin --change-addresses 0x70380000 -O ihex network_data.hex
```

You can find the following script at [Model/generate-n6-model.sh](../Model/generate-n6-model.sh)

## 2. Program Your Network Data

Now you can program your network data in external flash.

```bash
export DKEL="<STM32CubeProgrammer_N6 Install Folder>/bin/ExternalLoader/MX66UW1G45G_STM32N6570-DK.stldr"

# Weights and parameters
STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el $DKEL -hardRst -w Model/network_data.hex
```

## 3. Provide NN Information in `app_config.h`

You need to edit the last lines of [Inc/app_config.h](../Inc/app_config.h) to make the app compatible with your network.

### Post Process Type

This application supports four types of object detection post-processing. You can select one of these. Edit `POSTPROCESS_TYPE` with one of these values.

```c
#define POSTPROCESS_OD_YOLO_V2_UF       (10) /* Yolov2 postprocessing; Input model: uint8; output: float32         */
#define POSTPROCESS_OD_YOLO_V5_UU       (11) /* Yolov5 postprocessing; Input model: uint8; output: uint8           */
#define POSTPROCESS_OD_YOLO_V8_UF       (12) /* Yolov8 postprocessing; Input model: uint8; output: float32         */
#define POSTPROCESS_OD_YOLO_V8_UI       (13) /* Yolov8 postprocessing; Input model: uint8; output: int8            */
```

### NN Size Info

Edit your `NN_WIDTH` and `NN_HEIGHT`.

__WARNING:__ `NN_WIDTH * NN_BPP` must be a multiple of 16.

__WARNING:__ Only RGB888 format has been tested.

### Class Labels

Modify the `NB_CLASSES` and `classes_table` with your own class labels and number of classes.

### Display Delay

Using `DISPLAY_DELAY` allows delaying the displayed image so that the displayed information from the model is aligned with the displayed image. Adjust this value according to the model inference time.

### Configure Post Process Parameters Through User File

To change your post-process parameters, you need to edit the [Inc/postprocess_conf.h](../Inc/postprocess_conf.h).

Documentation of the post-processing library is available at [Postprocess lib](../Lib/lib_vision_models_pp/lib_vision_models_pp/README.md).

You can edit IOU thresholds, anchor values, and other parameters.

Some of these parameters need to be aligned with your model (number of classes, for example).

### Configure Output Buffer Size

Edit `NN_BUFFER_OUT_SIZE` so it has the size in bytes of your output tensor.

## 4. Build Application

Once your network data has been programmed (step 2) and network details configured (step 3), you can build your application by following:

[Application build and run](../README.md#application-build-and-run---dev-mode)

## 5. Run Application

### Dev Mode

See the [Application build and run](../README.md#application-build-and-run---dev-mode) section for more details.

### Boot from Flash

See [Program app in external flash](../README.md#application-build-and-run---boot-from-flash) to program your firmware.
