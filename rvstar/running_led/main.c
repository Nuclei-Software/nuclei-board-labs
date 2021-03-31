// See LICENSE for license details.
#include "nuclei_sdk_hal.h"

#include <stdio.h>

void led_R(void);
void led_G(void);
void led_B(void);
void led_W(void);
void test(void);
/**
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* LED init */
    gd_rvstar_led_init(LED1);
    gd_rvstar_led_init(LED2);
    gd_rvstar_led_init(LED3);

    /* configure WKUP button pin as input */
    gd_rvstar_key_init(WAKEUP_KEY_GPIO_PORT,KEY_MODE_GPIO);

    test();

    return 0;
}
/**
    \brief      start to read key status and change led
    \param[in]  none
    \param[out] none
    \retval     none
*/
void test(void)
{

    uint16_t cnt = 0;
    
    printf("\r\n USART printf & LED & Key example \r\n");
    printf("\r\n Please press the PA0-WKUP button, the LED color will be changed\r\n");

    while(1){

    	/* check whether the button is pressed */
        if(SET ==  gd_rvstar_key_state_get(KEY_WAKEUP)){
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
/**
    \brief      turn on green light
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_G(void)
{
    gd_rvstar_led_off(LED2);
    gd_rvstar_led_off(LED3);
    gd_rvstar_led_on(LED1);
    printf("\r\n LED Green\r\n");
}
/**
    \brief      turn on blue light
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_B(void)
{
    gd_rvstar_led_off(LED1);
    gd_rvstar_led_off(LED3);
    gd_rvstar_led_on(LED2);
    printf("\r\n LED Blue\r\n");
}
/**
    \brief      turn on red light
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_R(void)
{
    gd_rvstar_led_off(LED2);
    gd_rvstar_led_off(LED1);
    gd_rvstar_led_on(LED3);
    printf("\r\n LED Red\r\n");
}
/**
    \brief      turn on white light
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_W(void)
{
    gd_rvstar_led_on(LED3);
    gd_rvstar_led_on(LED2);
    gd_rvstar_led_on(LED1);
    printf("\r\n LED White\r\n");
}
