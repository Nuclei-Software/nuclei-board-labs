#include "nuclei_sdk_hal.h"
#include "I2C_OLED.h"

int main(void)
{
    unsigned char i;
    extern const unsigned char BMP1[];

    I2C_Configuration();
    OLED_Init();

    while (1) {
        OLED_Fill(0xFF);                                    // light up the full screen
        delay_1ms(1000);
        OLED_Fill(0x00);                                    // light off the full screen
        delay_1ms(1000);
        for (i = 0; i < 5; i++) {
            OLED_ShowCN(22 + i * 16, 0, i);                 // show Chinese characters
        }
        delay_1ms(1000);
        OLED_ShowStr(0, 3, "Happy May Day", 2);             // show 8*16 characters
        OLED_ShowStr(0, 6, "From RV-STAR Board", 1);        // show 6*8 characters
        delay_1ms(1000);
        OLED_CLS();                                         // clear the screen
        OLED_OFF();                                         // OLED sleep
        delay_1ms(1000);
        OLED_ON();                                          // OLED wake up
        OLED_DrawBMP(0, 0, 128, 8, (unsigned char*)BMP1);   // show BMP picture
        delay_1ms(3000);
    }
}