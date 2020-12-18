// See LICENSE for license details.
#include <stdio.h>
#include "hbird_sdk_hal.h"

void gpio_config();
void pwm_config();
void channel_config(uint8_t temp);



/**
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{

    printf("PWM LED Test\r\n");

    int cnt;

    /* GPIO Config */
    gpio_config();

    /* PWM Config */
    pwm_config();

    while(1)
    {
      /* set led status */

        for(cnt = 255; cnt >=0; cnt--)
        {
          channel_config(cnt);
          delay_1ms(10);
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
    // GPIO Init
    // Set GPIOA[0] as PWM Channel
    gpio_iof_config(GPIOA, IOF_PWM0_CH0_MASK);
}

/**
    \brief      configure the PWM peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pwm_config()
{
    PwmCounterCmd cmd;

    //Enable TIMER0
    pwm_timer_en(PWM_TIMER0);

    //Set PWM Count Cycle 
    pwm_timer_threshold_set(PWM_TIMER0, 0x03E80000);

  /*
   * PWM timer configuration
   * Arguments:
   *      - nTimer: number of timer
   *      - inputSource: Select counting source
   *                      0-31 gpio
   *                      32-35 out of timer 0
   *                      ...
   *                      44-47 out of timer 3
   *      - enableIn: count rules
   *                      ADV_TIMER_INMODE_AO         0x0 //event at each clock cycle
   *                      ADV_TIMER_INMODE_IN0        0x1 //event if input signal is 0
   *                      ADV_TIMER_INMODE_IN1        0x2 //event if input signal is 1
   *                      ADV_TIMER_INMODE_RISE       0x3 //event on input signal rising edge
   *                      ADV_TIMER_INMODE_FALL       0x4 //event on input signal falling edge
   *                      ADV_TIMER_INMODE_EDGE       0x5 //event on input signal rising or falling edge
   *                      ADV_TIMER_INMODE_ARM_RISE   0x6 //event on input signal rising edge when armed
   *                      ADV_TIMER_INMODE_ARM_FALL   0x7 //event on input signal falling edge when armed
   *      - FllorRTC:
   *                      ADV_TIMER_USEREF            0x1 //32KHz ref clk
   *                      ADV_TIMER_USESIG            0x0 //FLL
   *      - IncThenDec: action when reaching the TH:
   *                      ADV_TIMER_SAW:     /|/|/| (SAW)
   *                      ADV_TIMER_NOSAW:   /\/\/\ (NOSAW)
   *      - PreScaler
   */
  
    pwm_timer_conf(PWM_TIMER0, 0,          // choose the input source ? (if the rule is AO, this arg is not important)
                   ADV_TIMER_INMODE_AO,    // define the enable rules
                   ADV_TIMER_USESIG,       // FLL or RTC
                   ADV_TIMER_SAW,          // when reaching the TH: /|/|/| (SAW) or /\/\/\ (NOSAW)
                   15);                    // prescaler
  /*
   *  Configuration the 1 of 16 PWM channels:
   *  Arguments:
   *      - nTimer:   number of timer
   *      - channel:  number of channel
   *      - thCh:     threshold
   *      - actCh:    action of channel
   *                    ADV_TIMER_OP_SET    0       //   	000: Set
   *                    ADV_TIMER_OP_TOGRST 1       //   	001: Toggle then next is Clear
   *                    ADV_TIMER_OP_SETRST 2       //   	010: Set then Clear
   *                    ADV_TIMER_OP_TOG    3       //   	011: Toggle
   *                    ADV_TIMER_OP_RST    4       //   	100: Clear
   *                    ADV_TIMER_OP_TOGSET 5       //   	101: Toggle then next is Set
   *                    ADV_TIMER_OP_RSTSET 6       //   	110: Clear then Set
   *
   */
    pwm_timer_channel_config(PWM_TIMER0, PWM_TIMER_TH_CHANNEL0, 0x3E9, ADV_TIMER_OP_RSTSET);
    
    cmd = PWM_TIMER_CMD_START;
    pwm_timer_cmd(PWM_TIMER0, cmd);
}


/**
    \brief      configure the PWM channel
    \param[in]  temp: set value of channel between 0 to 255
    \param[out] none
    \retval     none
*/
void channel_config(uint8_t temp)
{
   uint32_t ch0;

   if(temp == 0) {
      ch0 = 0x3E9;
   } else if (temp == 255) {
      ch0 = 0x3E8;
   } else {
      ch0 = ((255-temp) * 1000) >> 8;
   }

   /* CH0 duty cycle set */
   pwm_timer_channel_config(PWM_TIMER0, PWM_TIMER_TH_CHANNEL0, ch0, ADV_TIMER_OP_RSTSET);

}
