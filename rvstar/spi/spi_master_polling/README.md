# Introduction

This is a SPI example that uses RVSTAR board as Master to transfer string data to Arduino Uno as Slave, which will print received data on the Serial Monitor.

# How to use

## Upload codes to Arduino
You should first build and upload the SPI Slave program to Arduino Uno board via Arduino IDE or PlatformIO IDE, here's the program codes:
```C++
#include "Arduino.h"
#include "SPI.h"

char buff[64];
volatile byte index = 0;
volatile boolean process = false;

void setup() {
  Serial.begin(115200);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);  // turn on SPI in Slave mode
  SPI.attachInterrupt();
}

void loop() {
  if(process) {
    process = false;
    Serial.println(buff);
  }
}

ISR(SPI_STC_vect) {
  byte c = SPDR;  // read byte form SPI Data Register
  if(index < sizeof(buff)) {
    buff[index++] = c;
    if(c == '\r') {
      index = 0;
      process = true;
    }
  }
}
```

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
cd rvstar/spi/spi_master_polling
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

You should connect RV-STAR and Arduino Uno like this:
|RVSTAR | Arduino |
| -- | -- |
| SCK(PA5) | SCK(D13) |
| MISO(PA6) | MISO(D12) |
| MOSI(PA7) | MOSI(D11) |
| GND | GND |

Then you can use a serial terminal tool like Minicom or CoolTerm to connect the board with baudrate 115200. After that, you will see "Hello World" printed in the terminal every 2 seconds.

**You can also refer to this guide for more details:**

* [GD32VF103V RV-STAR Kit Support for Nuclei SDK](https://doc.nucleisys.com/nuclei_sdk/design/board/gd32vf103v_rvstar.html#design-board-gd32vf103v-rvstar)

## Build and run using other tools

If you are not familar with command line, you can also create projects using Nuclei Studio and PlatformIO IDE, put this example code in the IDE and then build, upload and debug on it.

### Create a project

You can create a project in Nuclei Studio IDE or PlatformIO IDE.

### Copy the source code

Copy all the *.c and *.h files from this directory and replace the existing application code in the IDE project directory.

### Build and Upload

Connect RVSTAR board with Arduino, and then you can use a serial terminal tool like Minicom or CoolTerm to connect the board with baudrate 115200. After that, you will see "Hello World" printed in the terminal every 2 seconds.

## Reference

* [RVMCU课堂 - 手把手教你玩转RVSTAR——Nuclei Studio+蜂鸟调试器篇](https://rvmcu.com/column-topic-id-455.html)
* [RVMCU课堂 - 手把手教你玩转RVSTAR——PlatformIO篇](https://rvmcu.com/column-topic-id-452.html)
* [Nuclei Studio User Guide 中文手册](https://nucleisys.com/upload/files/doc/nucleistudio/Nuclei_Studio_User_Guide.pdf)
* [PlatformIO User Guide](https://docs.platformio.org/page/platforms/nuclei.html)
