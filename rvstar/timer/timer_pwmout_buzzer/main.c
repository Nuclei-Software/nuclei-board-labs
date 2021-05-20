#include "nuclei_sdk_hal.h"
#include "note.h"

void buzzer_init();
void buzzer_on(int freq);
void buzzer_off();

int notes[] = {
    NTD1, NTD1, NTD5, NTD5,
    NTD6, NTD6, NTD5,
    NTD4, NTD4, NTD3, NTD3,
    NTD2, NTD2, NTD1
};

int beats[] = {
    1, 1, 1, 1,
    1, 1, 2,
    1, 1, 1, 1,
    1, 1, 2
};

int main()
{
    buzzer_init();
    int length = sizeof(notes) / sizeof(notes[0]);

    while (1) {
        for (int i = 0; i < length; i++) {
            buzzer_on(notes[i]);
            delay_1ms(500 * beats[i]);
            buzzer_off();
        }
        delay_1ms(1000);
    }
}

void buzzer_init()
{
    /* init PB1(TIMER2_CH3) pin */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);

    rcu_periph_clock_enable(RCU_TIMER2);

    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
}

void buzzer_on(int freq)
{
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    timer_deinit(TIMER2);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER2 configuration */
    timer_initpara.prescaler         = 107;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 1000000 / freq;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2, &timer_initpara);

    /* initialize TIMER channel output parameter struct */
    timer_channel_output_struct_para_init(&timer_ocinitpara);
    /* CH3 configuration in PWM mode */
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER2, TIMER_CH_3, &timer_ocinitpara);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
    timer_enable(TIMER2);

    /* CH3 configuration in PWM mode1,duty cycle 50% */
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_3, 500000 / freq);
    timer_channel_output_mode_config(TIMER2, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);
}

void buzzer_off()
{
    timer_deinit(TIMER2);
}