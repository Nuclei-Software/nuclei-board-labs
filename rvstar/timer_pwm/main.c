// See LICENSE for license details.
#include "nuclei_sdk_hal.h"
#include <stdio.h>

void gpio_config();
void rgb_config(int temp1,int temp2,int temp3);
void timer_config();

/**
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{

    int cnt;

    /* GPIO Config */
    gpio_config();

    /* Timer Config */
    timer_config();

    while(1)
    {
    	/* set rgb_led status */

        for(cnt = 255; cnt >=0; cnt--)
        {
            rgb_config(cnt, 0, 0);
    	    delay_1ms(5);
        }
        for(cnt = 255; cnt >=0; cnt--)
        {
            rgb_config(0, cnt, 0);
    	    delay_1ms(5);
        }
        for(cnt = 255; cnt >=0; cnt--)
        {
            rgb_config(0, 0, cnt);
    	    delay_1ms(5);
        }
    }

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
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);

    /*Configure PA1 PA2 PA3(TIMER1 CH1 CH2 CH3) as alternate function*/
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, LEDG_PIN);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, LEDR_PIN);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, LEDB_PIN);
}

/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer_config()
{

	 timer_oc_parameter_struct timer_ocinitpara;
	 timer_parameter_struct timer_initpara;

	 rcu_periph_clock_enable(RCU_TIMER1);

	 timer_deinit(TIMER1);
	 /* initialize TIMER init parameter struct */
	 timer_struct_para_init(&timer_initpara);
	 /* TIMER1 configuration */
	 timer_initpara.prescaler         = 107;
	 timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	 timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	 timer_initpara.period            = 254;
	 timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	 timer_initpara.repetitioncounter = 0;
	 timer_init(TIMER1, &timer_initpara);

	 /* initialize TIMER channel output parameter struct */
	 timer_channel_output_struct_para_init(&timer_ocinitpara);
	 /* CH0, CH1 and CH2 configuration in PWM mode */
	 timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
	 timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
	 timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
	 timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
	 timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
	 timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

	 timer_channel_output_config(TIMER1,TIMER_CH_1,&timer_ocinitpara);
	 timer_channel_output_config(TIMER1,TIMER_CH_2,&timer_ocinitpara);
	 timer_channel_output_config(TIMER1,TIMER_CH_3,&timer_ocinitpara);


	 /* CH1 configuration in PWM mode0 */
	 timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_1,255);
	 timer_channel_output_mode_config(TIMER1,TIMER_CH_1,TIMER_OC_MODE_PWM0);
	 timer_channel_output_shadow_config(TIMER1,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);

	 /* CH2 configuration in PWM mode0 */
	 timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_2,255);
	 timer_channel_output_mode_config(TIMER1,TIMER_CH_2,TIMER_OC_MODE_PWM0);
	 timer_channel_output_shadow_config(TIMER1,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);

	 /* CH3 configuration in PWM mode0 */
	 timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_3,255);
	 timer_channel_output_mode_config(TIMER1,TIMER_CH_3,TIMER_OC_MODE_PWM0);
	 timer_channel_output_shadow_config(TIMER1,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);

     /* auto-reload preload enable */
	 timer_auto_reload_shadow_enable(TIMER1);
	 /* auto-reload preload enable */
	 timer_enable(TIMER1);
}

/**
    \brief      configure the rgb_led
    \param[in]  temp1:set value of red channel between 0 to 255
    \param[in]  temp2:set value of green channel between 0 to 255
    \param[in]  temp3:set value of blue channel between 0 to 255
    \param[out] none
    \retval     none
*/
void rgb_config(int temp1,int temp2,int temp3)
{
	 /* CH1 duty cycle set */
	 timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_1,(255-temp2));

	 /* CH2 duty cycle set */
	 timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_2,(255-temp1));

	 /* CH3 duty cycle set */
	 timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_3,(255-temp3));
}
