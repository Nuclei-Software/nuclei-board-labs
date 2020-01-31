/*!
    \file  main.c
    \brief running led
    
    \version 2019-6-5, V1.0.0, firmware for GD32VF103
*/

/*
    Copyright (c) 2019, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "nuclei_sdk_soc.h"
#include "nuclei_sdk_hal.h"
#include <stdio.h>

void gpio_config(void);
void rgb_config(int temp1,int temp2,int temp3);

/* configure the GPIO ports */
void gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);

    /*Configure PA1 PA2 PA3(TIMER1 CH1 CH2 CH3) as alternate function*/
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, LEDG_PIN);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, LEDR_PIN);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, LEDB_PIN);
}

/* configure the rgb_led peripheral */
void rgb_config(int temp1,int temp2,int temp3)
{

	 timer_oc_parameter_struct timer_ocinitpara;
	 timer_parameter_struct timer_initpara;

	 rcu_periph_clock_enable(RCU_TIMER1);

	 timer_deinit(TIMER1);
	 /* initialize TIMER init parameter struct */
	 timer_struct_para_init(&timer_initpara);
	 /* TIMER1 configuration */
	 timer_initpara.prescaler         = 108;
	 timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	 timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	 timer_initpara.period            = 255;
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

	 /* CH1 configuration in PWM mode1,duty cycle (256-temp2)/256) */
	 timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_1,(255-temp2));
	 timer_channel_output_mode_config(TIMER1,TIMER_CH_1,TIMER_OC_MODE_PWM0);
	 timer_channel_output_shadow_config(TIMER1,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);

	 /* CH2 configuration in PWM mode1,duty cycle (256-temp1)/256) */
	 timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_2,(255-temp1));
	 timer_channel_output_mode_config(TIMER1,TIMER_CH_2,TIMER_OC_MODE_PWM0);
	 timer_channel_output_shadow_config(TIMER1,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);

	 /* CH3 configuration in PWM mode1,duty cycle (256-temp3)/256) */
	 timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_3,(255-temp3));
	 timer_channel_output_mode_config(TIMER1,TIMER_CH_3,TIMER_OC_MODE_PWM0);
	 timer_channel_output_shadow_config(TIMER1,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);

	 /* auto-reload preload enable */
	 timer_auto_reload_shadow_enable(TIMER1);
	 /* auto-reload preload enable */
	 timer_enable(TIMER1);

}

int main(void)
{
	/* configure the GPIO ports */
    gpio_config();

    while(1)
    {
    	/* configure the rgb_led peripheral */
    	rgb_config(255, 255, 0);
    	delay_1ms(100);
        rgb_config(0, 255, 0);
        delay_1ms(100);
    }
}
