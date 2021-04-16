// See LICENSE for license details.
#include "nuclei_sdk_hal.h"

void led_config();
void led_on();
void led_off();

void key_config();
bit_status key_get_status();

/* record the status of the led */
bit_status led_status = 0;

int main(void)
{
    led_config();
    key_config();
    led_off();

    while (1) {
        /* the first key press detected */
        if (key_get_status() == SET) {
            /* delay 100ms for jitters elimination */
            delay_1ms(100);
            /* check again that the key is pressed */
            if (key_get_status() == SET) {
                switch (led_status) {
                    case 0:
                        led_on();
                        break;
                    case 1:
                        led_off();
                        break;
                    default:
                        break;
                }
                /* switch the status of the led */
                led_status = !led_status;
            }
        }
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

void key_config()
{
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable AF clock */
    rcu_periph_clock_enable(RCU_AF);
    /* init the PA0 pin with floating input mode */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
}

bit_status key_get_status()
{
    /* get the bit status(SET or RESET) of PA0 */
    return gpio_input_bit_get(GPIOA, GPIO_PIN_0);
}