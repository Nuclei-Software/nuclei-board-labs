#include "nuclei_sdk_hal.h"

/* CRC input value */
uint32_t val = (uint32_t)0x1234ABCD;
/* pre-calculated CRC value for input value */
uint32_t val_crc = (uint32_t)0x9B3CD6F8;

int main(void)
{
    /* init the built-in green led */
    gd_rvstar_led_init(LED1);
    /* enable the CRC clock */
    rcu_periph_clock_enable(RCU_CRC);

    /* reset the CRC to clear the register */
    crc_data_register_reset();

    /* compare the pre-calculated CRC value with the value calculated by hardware */
    if (val_crc == crc_single_data_calculate(val)) {
        gd_rvstar_led_on(LED1);
    } else {
        gd_rvstar_led_off(LED1);
    }
}