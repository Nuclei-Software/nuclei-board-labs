#ifndef __LCD_H
#define __LCD_H		

#include <stdio.h>
#include "hbird_sdk_hal.h"

//LCD Config Parameter
typedef struct  
{										    
    uint16_t width;
    uint16_t height;
    uint16_t id;
    uint8_t  dir;
    uint16_t wramcmd;
    uint16_t setxcmd;
    uint16_t setycmd;
} LCD_Dev; 	  


#define LCD_RS_MASK      (1<<12) 

#define LCD_DIR_Mode  	  0
#define USE_HORIZONTAL    0

#define L2R_U2D  0
#define L2R_D2U  1
#define R2L_U2D  2
#define R2L_D2U  3

#define U2D_L2R  4
#define U2D_R2L  5
#define D2U_L2R  6
#define D2U_R2L  7	 

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0xF81F
#define GRED 		 0xFFE0
#define GBLUE		 0x07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 		 0xBC40
#define BRRED 		 0xFC07
#define GRAY  		 0x8430
#define DARKBLUE      	 0x01CF
#define LIGHTBLUE      	 0x7D7C
#define GRAYBLUE       	 0x5458
#define LIGHTGREEN     	 0x841F
#define LIGHTGRAY        0XEF5B
#define LGRAY 		 0xC618
#define GRAY0            0xEF7D   
#define GRAY1            0x8410
#define GRAY2            0x4208
#define LGRAYBLUE        0xA651
#define LBBLUE           0x2B12

#define BACK_COLOR       0xFFFF

void LCD_RS_SET(void);
void LCD_RS_CLR(void);
void LCD_WR_REG(uint16_t regval);
void LCD_WR_DATA8(uint8_t data);
void LCD_WR_DATA16(uint16_t data);
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_SoftRest(void);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_Fast_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_Scan_Dir(uint8_t dir);
void LCD_Display_Dir(uint8_t dir);
void LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);
void LCD_Init(void);
void LCD_Clear(uint16_t color);
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint16_t color, uint8_t mode);
uint32_t LCD_Pow(uint8_t m, uint8_t n);
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color);
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color, uint8_t mode);
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint16_t color, uint8_t *p);

#endif  
