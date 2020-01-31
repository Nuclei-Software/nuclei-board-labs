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

void key_exti_init(void);
#define ECLIC_PRIGROUP_LEVEL3_PRIO1        3    /*!< 3 bits for level 1 bits for priority */

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* LED init */
    gd_eval_led_init(LED1);

    /* configure WKUP button pin as input */
    rcu_periph_clock_enable(WAKEUP_KEY_GPIO_CLK);
    gpio_init(WAKEUP_KEY_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, WAKEUP_KEY_PIN);

    /* key_exit init */
    key_exti_init();
    while(1);
}

/*!
    \brief      initialize the EXTI configuration of the key
    \param[in]  none
    \param[out] none
    \retval     none
*/
void key_exti_init(void)
{
    /* enable the AF clock */
    rcu_periph_clock_enable(RCU_AF);
    /* enable and set key EXTI interrupt to the specified priority */
    __RV_CSR_SET(mstatus, MSTATUS_MIE);
    __ECLIC_SetCfgNlbits(ECLIC_PRIGROUP_LEVEL3_PRIO1);
    __ECLIC_EnableIRQ(EXTI0_IRQn);
    __ECLIC_SetPriorityIRQ(EXTI0_IRQn,1);
    __ECLIC_SetLevelIRQ(EXTI0_IRQn,1);

    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(WAKEUP_KEY_EXTI_PORT_SOURCE, WAKEUP_KEY_EXTI_PIN_SOURCE);

    /* configure key EXTI line */
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_0);
}

/*!
    \brief      this function handles external lines 10 to 15 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI0_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(WAKEUP_KEY_PIN)){

        if(RESET == gpio_input_bit_get(WAKEUP_KEY_GPIO_PORT, WAKEUP_KEY_PIN)){
        	/* toggle RED led */
        	gd_eval_led_toggle(LED1);
        }
    }
    /* clear EXTI lines pending flag */
    exti_interrupt_flag_clear(WAKEUP_KEY_PIN);
}


