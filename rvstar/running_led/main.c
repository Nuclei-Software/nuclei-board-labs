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
#define USB_VOE_PORT                     GPIOD
#define USB_VOE_CLK                      RCU_GPIOD
#define RVSTAR_COM                         UART4
#define USB_VOE_PIN                      GPIO_PIN_3   // RV_STAR
void led_R(void);
void led_G(void);
void led_B(void);
void led_W(void);
void test(void);
/*!
    \brief      turn on selected led
    \param[in]  lednum: specify the led to be turned on
      \arg        LEDR
      \arg        LEDG
      \arg        LEDB
    \param[out] none
    \retval     none
*/
void gd_rvstar_led_on(led_typedef_enum lednum)
{
    GPIO_BC(lednum) = lednum;
}

/*!
    \brief      turn off selected led
    \param[in]  lednum: specify the led to be turned off
      \arg        LEDR
      \arg        LEDG
      \arg        LEDB
    \param[out] none
    \retval     none
*/
void gd_rvstar_led_off(led_typedef_enum lednum)
{
	GPIO_BOP(lednum) = lednum;
}
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* configure USART */
    gd_eval_com_init(RVSTAR_COM);

	/* LED init */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);

    /* configure WKUP button pin as input */
    rcu_periph_clock_enable(WAKEUP_KEY_GPIO_CLK);
    gpio_init(WAKEUP_KEY_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, WAKEUP_KEY_PIN);

    /* configure USB OTG VBUS_OE pin as output */
    rcu_periph_clock_enable(USB_VOE_CLK);
    GPIO_BOP(USB_VOE_PORT) = USB_VOE_PIN;
    gpio_init(USB_VOE_PORT, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, USB_VOE_PIN);

    test();
}

void test(void)
{

	uint16_t cnt = 0;

	printf("\r\n USART printf & LED & Key example \r\n");
	printf("\r\n Please press the PA0-WKUP button, the LED color will be changed\r\n");

    while(1){

    	/* check whether the button is pressed */

        if(SET ==  gpio_input_bit_get(GPIOA, GPIO_PIN_0)){
            switch((cnt%4)){
				case 0: led_R(); break;
				case 1: led_G(); break;
				case 2: led_B(); break;
				case 3: led_W(); break;
            }

            if(cnt!=3) cnt++; else cnt=0;
            delay_1ms(500);
        }

    }

}

void led_R(void)
{
    gd_rvstar_led_off(LED2);
    gd_rvstar_led_off(LED3);
    gd_rvstar_led_on(LED1);
    printf("\r\n LED Red\r\n");
}

void led_G(void)
{
    gd_rvstar_led_off(LED1);
    gd_rvstar_led_off(LED3);
    gd_rvstar_led_on(LED2);
    printf("\r\n LED Green\r\n");
}

void led_B(void)
{
    gd_rvstar_led_off(LED2);
    gd_rvstar_led_off(LED1);
    gd_rvstar_led_on(LED3);
    printf("\r\n LED Blue\r\n");
}

void led_W(void)
{
    gd_rvstar_led_on(LED3);
    gd_rvstar_led_on(LED2);
    gd_rvstar_led_on(LED1);
    printf("\r\n LED White\r\n");
}

/* retarget the C library printf function to the USART */
int _put_char(int ch)
{
    usart_data_transmit(RVSTAR_COM, (uint8_t) ch );
    while ( usart_flag_get(RVSTAR_COM, USART_FLAG_TBE)== RESET){
    }

    return ch;
}
