// See LICENSE for license details.
#include "nuclei_sdk_hal.h"

void led_config();
void led_on();
void led_off();

int main(void)
{
    /* init the green led */
    led_config();

    while (1) {
        /* turn on the led and delay 2s */
        led_on();
        delay_1ms(2000);

        /* turn off the led and delay 2s */
        led_off();
        delay_1ms(2000);
    }
}

void led_config()
{
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* init the PA1 pin with push-pull output mode */
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
}

void led_on()
{
    /* set the PA1 bit 0 to turn on the led */
    gpio_bit_reset(GPIOA, GPIO_PIN_1);
}

void led_off()
{
    /* set the PA1 bit 1 to turn on the led */
    gpio_bit_set(GPIOA, GPIO_PIN_1);
}