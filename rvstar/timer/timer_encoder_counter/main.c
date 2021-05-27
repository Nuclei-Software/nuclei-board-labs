#include "nuclei_sdk_hal.h"
#include <stdio.h>

void encoder_init();

int main()
{
    encoder_init();
    gd_com_init(GD32_COM0);

    int counter = 0;
    while (1) {
        counter = timer_counter_read(TIMER2);
        printf("Counter: %d\n", counter);

        delay_1ms(500);
    }
}

void encoder_init()
{
    /* TIMER2_CH0 - PA6, TIMER2_CH1 - PA7 */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);

    rcu_periph_clock_enable(RCU_TIMER2);
    timer_deinit(TIMER2);

    /* initialize TIMER init parameter struct */
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);
    /* TIMER2 configuration */
    timer_initpara.prescaler         = 0;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 10000;  /* set auto-reload value */
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER2, &timer_initpara);

    /* select the encoder mode */
    timer_slave_mode_select(TIMER2, TIMER_ENCODER_MODE2);
    timer_counter_value_config(TIMER2, 5000);  /* config the initial value */

    timer_enable(TIMER2);
}