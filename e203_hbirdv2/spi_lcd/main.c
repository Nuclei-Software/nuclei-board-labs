// See LICENSE for license details.
#include <stdio.h>
#include "lcd.h"
#include "hbird_sdk_hal.h"

void gpio_config();
void spi_config();
void lcd_display();

extern LCD_Dev lcddev;

/**
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{

    printf("SPI LCD Test Start\r\n");

    int cnt;

    /* GPIO Config */
    gpio_config();

    /* SPI Config */
    spi_config();

    /* LCD Init */
    LCD_Init();
  
    /* LCD Display*/
    lcd_display();

    printf("SPI LCD Test End\r\n");
    
    return 0;   
}


/**
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config()
{
    // GPIO Init
    // Set GPIOA[8], GPIOA[9], GPIOA[10] GPIOA[11] as SPI Interface
    // Set GPIOA[18] as Output port to control LCD D/C
    gpio_iof_config(GPIOA, IOF_SPI_MASK);
    gpio_enable_output(GPIOA, LCD_RS_MASK);
}

/**
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config()
{
    spi_setup_clk(SPI1, 0x4);
}


/**
    \brief      display demo string in LCD 
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lcd_display()
{
    LCD_ShowString(0, 0, lcddev.width, lcddev.height,      24, RED,   "Hello World"); 
    LCD_ShowString(0, 25, lcddev.width,(lcddev.height-25), 16, GREEN, "DDR200T Dev Board"); 
    LCD_ShowString(0, 41, lcddev.width,(lcddev.height-41), 16, GREEN, "Hummingbirdv2 MCU"); 
    LCD_ShowString(0, 57, lcddev.width,(lcddev.height-57), 16, GREEN, "E203 RISC-V Core"); 
    LCD_ShowString(0, 73, lcddev.width,(lcddev.height-73), 16, BLUE,  "Nuclei System Technology"); 
}
