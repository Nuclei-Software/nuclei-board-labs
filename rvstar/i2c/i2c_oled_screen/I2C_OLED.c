#include "I2C_OLED.h"
#include "fonts.h"

void I2C_Configuration(void)
{
    uint32_t GPIO_SDA, GPIO_SCL;
    uint32_t GPIO_PIN_SDA, GPIO_PIN_SCL;

    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_I2C1);

    GPIO_SDA = GPIOB;
    GPIO_PIN_SDA = GPIO_PIN_11;
    GPIO_SCL = GPIOB;
    GPIO_PIN_SCL = GPIO_PIN_10;

    gpio_init(GPIO_SCL, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_SCL);
    gpio_init(GPIO_SDA, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_SDA);

    i2c_clock_config(I2C1, 400000, I2C_DTCY_2);
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x30);
    i2c_enable(I2C1);
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);
}

void I2C_WriteByte(uint8_t addr, uint8_t data)
{
    /* wait until I2C bus is idle */
    while (i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);
    /* wait until SBSEND bit is set */
    while (!i2c_flag_get(I2C1, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C1, 0x78, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set*/
    while (!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);

    /* send a addr byte */
    i2c_data_transmit(I2C1, addr);
    /* wait until the transmission data register is empty*/
    while (!i2c_flag_get(I2C1, I2C_FLAG_TBE));

    /* send a data byte */
    i2c_data_transmit(I2C1, data);
    /* wait until the transmission data register is empty*/
    while (!i2c_flag_get(I2C1, I2C_FLAG_TBE));

    /* send a stop condition to I2C bus*/
    i2c_stop_on_bus(I2C1);
    /* wait until stop condition generate */
    while (I2C_CTL0(I2C1) & 0x0200);
}

void WriteCmd(unsigned char I2C_Command)  // Write command
{
    I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(unsigned char I2C_Data)     // Write data
{
    I2C_WriteByte(0x40, I2C_Data);
}

void OLED_Init(void)
{
    delay_1ms(100);  // this delay is important

    WriteCmd(0xAE);  // display off
    WriteCmd(0x20);  // Set Memory Addressing Mode
    WriteCmd(0x10);  // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(0xb0);  // Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8);  // Set COM Output Scan Direction
    WriteCmd(0x00);  // ---set low column address
    WriteCmd(0x10);  // ---set high column address
    WriteCmd(0x40);  // --set start line address
    WriteCmd(0x81);  // --set contrast control register
    WriteCmd(0xff);  // adjust the luminance 0x00~0xff
    WriteCmd(0xa1);  // --set segment re-map 0 to 127
    WriteCmd(0xa6);  // --set normal display
    WriteCmd(0xa8);  // --set multiplex ratio(1 to 64)
    WriteCmd(0x3F);  //
    WriteCmd(0xa4);  // 0xa4, Output follows RAM content;0xa5,Output ignores RAM content
    WriteCmd(0xd3);  // -set display offset
    WriteCmd(0x00);  // -not offset
    WriteCmd(0xd5);  // --set display clock divide ratio/oscillator frequency
    WriteCmd(0xf0);  // --set divide ratio
    WriteCmd(0xd9);  // --set pre-charge period
    WriteCmd(0x22);  //
    WriteCmd(0xda);  // --set com pins hardware configuration
    WriteCmd(0x12);
    WriteCmd(0xdb);  // --set vcomh
    WriteCmd(0x20);  // 0x20, 0.77xVcc
    WriteCmd(0x8d);  // --set DC-DC enable
    WriteCmd(0x14);  //
    WriteCmd(0xaf);  // --turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y)  // set origin coordinates
{
    WriteCmd(0xb0 + y);
    WriteCmd(((x & 0xf0) >> 4) | 0x10);
    WriteCmd((x & 0x0f) | 0x01);
}

void OLED_Fill(unsigned char fill_Data)  // fill the full screen
{
    unsigned char m, n;
    for (m = 0; m < 8; m++) {
        WriteCmd(0xb0 + m); // page0-page1
        WriteCmd(0x00);     // low column start address
        WriteCmd(0x10);     // high column start address
        for (n = 0; n < 128; n++) {
            WriteDat(fill_Data);
        }
    }
}

void OLED_CLS(void)  // clear the screen
{
    OLED_Fill(0x00);
}

/*!
 *  \brief      wake up OLED from sleep
 *  \param[in]  none
 *  \param[out] none
 *  \retval     none
 */
void OLED_ON(void)
{
    WriteCmd(0X8D);  // set charge pump
    WriteCmd(0X14);  // open charge pump
    WriteCmd(0XAF);  // OLED wake up
}

/*!
 *  \brief      let the OLED sleep
 *  \param[in]  none
 *  \param[out] none
 *  \retval     none
 */
void OLED_OFF(void)
{
    WriteCmd(0X8D);  // set charge pump
    WriteCmd(0X10);  // close charge pump
    WriteCmd(0XAE);  // OLED sleep
}

/*!
 *  \brief      show the characters
 *  \param[in]  x: X-coordinate of the start point
 *  \arg        0~127
 *  \param[in]  y:YX-coordinate of the start point
 *  \arg        0~7
 *  \param[in]  ch[]: the text array to show
 *  \param[in]  TextSize: the size of the text
 *  \arg        1: 6*8
 *  \arg        2: 8*16
 *  \param[out] none
 *  \retval     none
 */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
    unsigned char c = 0, i = 0, j = 0;
    switch (TextSize) {
        case 1: {
            while (ch[j] != '\0') {
                c = ch[j] - 32;
                if (x > 126) {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x, y);
                for (i = 0; i < 6; i++)
                { WriteDat(F6x8[c][i]); }
                x += 6;
                j++;
            }
        }
        break;
        case 2: {
            while (ch[j] != '\0') {
                c = ch[j] - 32;
                if (x > 120) {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x, y);
                for (i = 0; i < 8; i++)
                { WriteDat(F8X16[c * 16 + i]); }
                OLED_SetPos(x, y + 1);
                for (i = 0; i < 8; i++)
                { WriteDat(F8X16[c * 16 + i + 8]); }
                x += 8;
                j++;
            }
        }
        break;
    }
}

/*!
 *  \brief      show the Chinese characters(16*16)
 *  \param[in]  x: X-coordinate of the start point
 *  \arg        0~127
 *  \param[in]  y: Y-coordinate of the start point
 *  \arg        0~7
 *  \param[in]  N: the index of the Chinese character
 *  \param[out] none
 *  \retval     none
 */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
    unsigned char wm = 0;
    unsigned int  adder = 32 * N;
    OLED_SetPos(x, y);
    for (wm = 0; wm < 16; wm++) {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(x, y + 1);
    for (wm = 0; wm < 16; wm++) {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
}

/*!
 *  \brief      draw the BMP picture
 *  \param[in]  x0: X-coordinate of the start point
 *  \arg        0~127
 *  \param[in]  y0: Y-coordinate of the start point
 *  \arg        0~7
 *  \param[in]  x1: X-coordinate of the end point
 *  \arg        1~128
 *  \param[in]  y1: Y-coordinate of the end point
 *  \arg        1~8
 *  \param[in]  BMP[]: the array of the BMP picture
 *  \param[out] none
 *  \retval     none
 */
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
    { y = y1 / 8; }
    else
    { y = y1 / 8 + 1; }
    for (y = y0; y < y1; y++) {
        OLED_SetPos(x0, y);
        for (x = x0; x < x1; x++) {
            WriteDat(BMP[j++]);
        }
    }
}
