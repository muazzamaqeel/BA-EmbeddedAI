# x-cube-n6-ai-people-detection-tracking Application

Computer Vision application to enable the deployment of object detection models on the STM32N6570-DK or NUCLEO-N657X0-Q board.

This application is prebuilt with a people detection model, "YoloX" for STM32N6570-DK and "TinyYOLOv2" for NUCLEO-N657X0-Q.

This top readme gives an overview of the app. Additional documentation is available in the [Doc](./Doc/) folder.

![Screenshot of application running](_htmresc/screenshot.png)

## Doc Folder Content

- [Application Overview](Doc/Application-Overview.md)
- [Boot Overview](Doc/Boot-Overview.md)
- [Deploy Your TFLite Model](Doc/Deploy-your-tflite-Model.md)
- [Camera Build Options](Doc/Build-Options.md)

## Features Demonstrated in This Example

- Multi-threaded application flow (FreeRTOS)
- Tracking and box filtering
- NPU accelerated quantized AI model inference
- Dual DCMIPP pipes
- DCMIPP crop, decimation, downscale
- LTDC dual-layer implementation
- DCMIPP ISP usage
- Dev mode
- Boot from External Flash

## Hardware Support

### MB1939 STM32N6570-DK

  - The board should be connected to the onboard ST-LINK debug adapter CN6 with a __USB-C to USB-C cable to ensure sufficient power__
  - OTP fuses are set in this example for xSPI IOs to get the maximum speed (200MHz) on xSPI interfaces.

![Board](_htmresc/ImageBoard.JPG)

STM32N6570-DK board with MB1854B IMX335.

### MB1940 NUCLEO-N657X0-Q

  - The board should be connected to the onboard ST-LINK debug adapter CN10 with a __USB-C to USB-C cable to ensure sufficient power__
  - An additional USB cable to connect USB (CN8) to the host computer for UVC flavor
  - OTP fuses are set in this example for xSPI IOs to get the maximum speed (200MHz) on xSPI interfaces
  - X-NUCLEO-GFX01M2 for SPI flavor

- 3 Cameras are supported:
  - MB1854B IMX335 (Default Camera provided with the MB1939 STM32N6570-DK board)
  - STEVAL-55G1MBI VD55G1 Camera module (Use the CSI-2 cable provided with the camera module)
  - STEVAL-66GYMAI VD66GY Camera module (Use the CSI-2 cable provided with the camera module)

## Tools Version

- IAR Embedded Workbench for Arm (**EWARM 9.40.1**) + N6 patch ([**EWARMv9_STM32N6xx_V1.0.0**](STM32Cube_FW_N6/Utilities/PC_Software/EWARMv9_STM32N6xx_V1.0.0.zip))
- [STM32CubeIDE](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-ides/stm32cubeide.html) (**STM32CubeIDE 1.17.0**)
- [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) (**v2.18.0**)
- [STEdgeAI](https://www.st.com/en/development-tools/stedgeai-core.html) (**v2.1.0**)

## Boot Modes

The STM32N6 does not have any internal flash. To retain your firmware after a reboot, you must program it in the external flash.
Alternatively, you can load your firmware directly from SRAM (dev mode). However, in dev mode, if you turn off the board, your program will be lost.

__Boot modes:__
- Dev mode (BOOT0 and BOOT1 to the right): load firmware from debug session in RAM, program firmware in external flash
- Boot from flash (BOOT0 and BOOT1 to the left)

## Console parameters

You can see application messages by attaching a console application to the ST-Link console output. Use the following console parameters:
- Baud rate of 115200 bps.
- No parity.
- One stop bit.

## Quickstart Using Prebuilt Binaries

### Flash Prebuilt Binaries

Three binaries must be programmed into the board's external flash using the following procedure:

  1. Set both switches to the right side.
  2. Program `Binary/ai_fsbl.hex` (To be done once) (First stage boot loader).
  3. Program `Binary/network_data-dk.hex` (parameters of the networks; To be changed only when the network is changed). For MB1940 NUCLEO-N657X0-Q
     program `Binary/network_data-nucleo.hex`
  4. Program `Binary/x-cube-n6-ai-people-detection-tracking-dk.hex` (firmware application) for STM32N6570-DK. For MB1940 NUCLEO-N657X0-Q select either
     `Binary/x-cube-n6-ai-people-detection-tracking-nucleo-uvc.hex` or `Binary/x-cube-n6-ai-people-detection-tracking-nucleo-spi.hex`
  5. Set both switches to the left side.
  6. Power cycle the board.

### How to Program Hex Files Using STM32CubeProgrammer UI

See [How to Program Hex Files STM32CubeProgrammer](Doc/Program-Hex-Files-STM32CubeProgrammer.md).

### How to Program Hex Files Using Command Line

Make sure to have the STM32CubeProgrammer bin folder added to your path.

```bash
export DKEL="<STM32CubeProgrammer_N6 Install Folder>/bin/ExternalLoader/MX66UW1G45G_STM32N6570-DK.stldr"

# First Stage Boot Loader
STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el $DKEL -hardRst -w Binary/ai_fsbl.hex

# Network Parameters and Biases
STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el $DKEL -hardRst -w Binary/network_data-dk.hex

# Application Firmware
STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el $DKEL -hardRst -w Binary/x-cube-n6-ai-people-detection-tracking-dk.hex
```

## Quickstart Using Source Code

Before building and running the application, you have to program `network_data.hex` (model weights and biases).

This step only has to be done once unless you change the AI model.
See [Quickstart Using Prebuilt Binaries](#quickstart-using-prebuilt-binaries) for details.

More information about boot modes is available at [Boot Overview](Doc/Boot-Overview.md).

### Application Build and Run - Dev Mode

__Make sure to have both switches to the right side.__
Intructions below are for STM32N6570-DK. For NUCLEO-N657X0-Q you have to select one of the two nucleo project according to your use case.

#### STM32CubeIDE

Double click on `STM32CubeIDE/STM32N6570-DK/.project` to open the project in STM32CubeIDE. Build and run with the build and run buttons.

#### IAR EWARM

Double click on `EWARM/STM32N6570-DK/x-cube-n6-ai-people-detection-tracking-dk.eww` to open the project in IAR IDE. Build and run with the build and run buttons.

#### Makefile

Before running the commands below, be sure to have the commands in your PATH.

1. Build the project using the provided `Makefile`:

```bash
make -j8
```

2. Open a GDB server connected to the STM32 target:

```bash
ST-LINK_gdbserver -p 61234 -l 1 -d -s -cp <path-to-stm32cubeprogramer-bin-dir> -m 1 -g
```

3. In a separate terminal session, launch a GDB session to load the firmware image into the device memory:

```bash
$ arm-none-eabi-gdb build/Project.elf
(gdb) target remote :61234
(gdb) monitor reset
(gdb) load
(gdb) continue
```

### Application Build and Run - Boot from Flash

__Make sure to have both switches to the right side.__
Intructions below are for STM32N6570-DK. For NUCLEO-N657X0-Q you have to select one of the two nucleo project according to your use case.

#### STM32CubeIDE

Double click on `STM32CubeIDE/STM32N6570-DK/.project` to open project in STM32CubeIDE. Build with build button.

#### IAR EWARM

Double click on `EWARM/STM32N6570-DK/x-cube-n6-ai-people-detection-tracking-dk.eww` to open project in IAR IDE. Build with build button.

#### Makefile

Before running the commands below, be sure to have them in your PATH.

1. Build project using the provided `Makefile`:

```bash
make -j8
```

Once your app is built with Makefile, STM32CubeIDE, or EWARM, you must add a signature to the bin file:
```bash
STM32_SigningTool_CLI -bin build/Project.bin -nk -t ssbl -hv 2.3 -o build/Project_sign.bin
```

You can program the signed bin file at the address `0x70100000`.

```bash
export DKEL="<STM32CubeProgrammer_N6 Install Folder>/bin/ExternalLoader/MX66UW1G45G_STM32N6570-DK.stldr"

# Adapt build path to your IDE
STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el $DKEL -hardRst -w build/Project_sign.bin 0x70100000
```

Note: Only the App binary needs to be programmed if the FSBL and network_data.hex were previously programmed.

__Set both switches to the left side.__

Do a power cycle to boot from the external flash.

## Known Issues and Limitations

- (NN_WIDTH * NN_BPP) must be a multiple of 16.
- (LCD_BG_WIDTH * 2) must be a multiple of 16.
