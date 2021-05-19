#include "nuclei_sdk_hal.h"

void spi_config();

int main()
{
    /* configure SPI */
    spi_config();
    /* SPI enable */
    spi_enable(SPI0);

    while (1) {
        char c;
        for (const char* p = "Hello World\n\r"; c = *p; p++) {
            while (RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE));
            spi_i2s_data_transmit(SPI0, c);
            while (RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE));
        }
        delay_1ms(2000);
    }

}

void spi_config()
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_SPI0);

    /* configure SPI0 GPIO : SCK/PA5, MISO/PA6, MOSI/PA7 */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    spi_parameter_struct spi_init_struct;
    /* deinitialize SPI and the parameters */
    spi_i2s_deinit(SPI0);

    spi_struct_para_init(&spi_init_struct);
    /* configure SPI0 parameters */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_128;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);
}