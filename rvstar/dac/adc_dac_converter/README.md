# Introduction

This is an DAC example for RVSTAR board which uses the PC0(ADC1_CH10) to read analog values and uses the PA5(DAC1) to output voltages of these values.

# How to use

## Connect the peripherals
To observe the phenomenon of this program, you need connect a rotation potentiometer to the PC0 and a LED to the PA5.

## Build and run using Nuclei SDK

Here are the steps build and run in Nuclei SDK using command line.

~~~shell
# clone nuclei-sdk repo and cd to it
git clone https://github.com/Nuclei-Software/nuclei-sdk
cd nuclei-sdk
# Setup nuclei tools environment following https://doc.nucleisys.com/nuclei_sdk/quickstart.html#setup-tools-and-environment
# Assume you are in Windows, and have create and setup the setup_config.bat file
# source the nuclei tool environment
setup.bat
# clone nuclei-board-labs
git clone https://github.com/Nuclei-Software/nuclei-board-labs
cd nuclei-board-labs/
# cd to this example
cd rvstar/dac/adc_dac_converter
# Build this example for nuclei rvstar board
## clean this example first
make SOC=gd32vf103 BOARD=gd32vf103v_rvstar clean
## build this example
make SOC=gd32vf103 BOARD=gd32vf103v_rvstar all
## connect rvstar board using USB dongle, and make sure driver is setup correctly
## see https://rvmcu.com/column-topic-id-464.html
## upload this example
make SOC=gd32vf103 BOARD=gd32vf103v_rvstar upload
## debug this example
make SOC=gd32vf103 BOARD=gd32vf103v_rvstar debug
~~~

After start-up, the led will change its brightness when you rotate the potentiometer.

**You can also refer to this guide for more details:**

* [GD32VF103V RV-STAR Kit Support for Nuclei SDK](https://doc.nucleisys.com/nuclei_sdk/design/board/gd32vf103v_rvstar.html#design-board-gd32vf103v-rvstar)

## Build and run using other tools

If you are not familar with command line, you can also create projects using Nuclei Studio and PlatformIO IDE, put this example code in the IDE and then build, upload and debug on it.

### Create a project

You can create a project in Nuclei Studio IDE or PlatformIO IDE.

### Copy the source code

Copy the main.c file from this directory and replace the existing application code in the IDE project directory.

### Build and Upload

Connect RV-STAR board to your PC, build and upload the project and after start-up, the led will change its brightness when you rotate the potentiometer.

## Reference

* [RVMCU课堂 - 手把手教你玩转RVSTAR——Nuclei Studio+蜂鸟调试器篇](https://rvmcu.com/column-topic-id-455.html)
* [RVMCU课堂 - 手把手教你玩转RVSTAR——PlatformIO篇](https://rvmcu.com/column-topic-id-452.html)
* [Nuclei Studio User Guide 中文手册](https://nucleisys.com/upload/files/doc/nucleistudio/Nuclei_Studio_User_Guide.pdf)
* [PlatformIO User Guide](https://docs.platformio.org/page/platforms/nuclei.html)
