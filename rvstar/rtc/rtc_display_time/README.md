# Introduction

This is an RTC example for RVSTAR board that explains how to configure the RTC module. In this example, RTC peripheral is configured to display current time. Select LXTAL as RTC clock source. And the Serial Terminal should be connected to the RV-STAR board via COM0(UART4).

# How to use

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
cd rvstar/rtc/rtc_display_time
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

After start-up, the program will check if the BKP data register is written to a key value. If the value is incorrect, the program will ask to set the time on the Serial Terminal. Then the current time will be displayed on the Serial Terminal. The RTC module is in the Backup Domain, which is not reset by the system reset.

**You can also refer to this guide for more details:**

* [GD32VF103V RV-STAR Kit Support for Nuclei SDK](https://doc.nucleisys.com/nuclei_sdk/design/board/gd32vf103v_rvstar.html#design-board-gd32vf103v-rvstar)

## Build and run using other tools

If you are not familar with command line, you can also create projects using Nuclei Studio and PlatformIO IDE, put this example code in the IDE and then build, upload and debug on it.

### Create a project

You can create a project in Nuclei Studio IDE or PlatformIO IDE.

### Copy the source code

Copy the main.c file from this directory and replace the existing application code in the IDE project directory.

### Build and Upload

Connect RV-STAR board to your PC, build and upload the project and after start-up, the program will check if the BKP data register is written to a key value. If the value is incorrect, the program will ask to set the time on the Serial Terminal. Then the current time will be displayed on the Serial Terminal. The RTC module is in the Backup Domain, which is not reset by the system reset.

## Reference

* [RVMCU课堂 - 手把手教你玩转RVSTAR——Nuclei Studio+蜂鸟调试器篇](https://rvmcu.com/column-topic-id-455.html)
* [RVMCU课堂 - 手把手教你玩转RVSTAR——PlatformIO篇](https://rvmcu.com/column-topic-id-452.html)
* [Nuclei Studio User Guide 中文手册](https://nucleisys.com/upload/files/doc/nucleistudio/Nuclei_Studio_User_Guide.pdf)
* [PlatformIO User Guide](https://docs.platformio.org/page/platforms/nuclei.html)
