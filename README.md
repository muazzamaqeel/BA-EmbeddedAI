# AI-Based Face Recognition on low power Micro-Controller
## Description
This repository contains the customized version of the Embedded AI face recognition system developed for my bachelor thesis. The software runs fully on the STM32N6570 Discovery Kit and provides local face detection, face recognition, PIN verification and a small user interface. All components run offline on the microcontroller with low power usage.


### Thesis Link : [Thesis Doc. Link](https://drive.google.com/file/d/1A0Kr7CJ2QaIpqrQ-EYLE8pFxNI2Hbk2f/view)

## Graphical User Interface
<p align="left">
  <img src="https://github.com/user-attachments/assets/a2b326f1-37af-4aa7-ac6c-edb7c6938e9d" height="300px" />
  <img src="https://github.com/user-attachments/assets/ba39e637-9eac-4995-8034-acff899ecaaa" height="300px" />
</p>


## Steps to Execute the Embedded-AI Software

### Pre-requirements
#### Required Tools
- **STM32CubeIDE**
- **STM32CubeProgrammer**
  - External Loader Path:  
    `C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\ExternalLoader\MX66UW1G45G_STM32N6570-DK.stldr`

#### Script flashing (Tested with Git Bash)
- Navigate to the scripts directory:

```bash
cd /c/STM32_FaceRecognition/BA-EmbeddedAI/Scripts
./Full_Board_Cleanup.sh
./Software_Flashing.sh
./User_Interface_Flashing.sh
```

## Software Architecture Diagrams
### State Tranistion Diagram
<p align="left">
<img width="1040" height="839" alt="DetailLevel" src="https://github.com/user-attachments/assets/c955c286-c861-4894-862b-ef7e108f6757" />
</p>

### Software Package Diagram
<p align="left">
<img width="1189" height="668" alt="PackageDiagram" src="https://github.com/user-attachments/assets/39684016-8e20-4522-b9ca-c43cd7b38eeb" />
</p>

















This is my customized version of the project.  
For the original documentation, see [README.original.md](README.original.md).
