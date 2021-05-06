#ifndef __OLED_I2C_H
#define __OLED_I2C_H

#include "nuclei_sdk_hal.h"

#define OLED_ADDRESS    0x78  // by adjusting the 0R resistance，the screen can switch address besides 0x78 or 0x7A(default: 0x78)

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr, uint8_t data);

void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);

void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);

#endif
