// See LICENSE for license details.
#include "nuclei_sdk_hal.h"
#include <stdio.h>

void user_key_exti_config();
void soc_timer_config();

/**
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
  */
int main(void)
{
    uint8_t timer1_intlevel=1;
    uint8_t timer2_intlevel=2;
    uint8_t exti_intlevel  =3;
    int32_t returnCode;

    /* Board Config */
    gd_rvstar_led_init(LED3);
    gd_rvstar_led_init(LED1);
    gd_rvstar_led_init(LED2);

    gd_rvstar_key_init(WAKEUP_KEY_GPIO_PORT,KEY_MODE_EXTI);

    /* Timer Config */
    soc_timer_config();

    /* EXIT config */
    user_key_exti_config();

    /* ECLIC config */
    returnCode = ECLIC_Register_IRQ(EXTI0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                    ECLIC_LEVEL_TRIGGER, exti_intlevel, 0, NULL);
    returnCode = ECLIC_Register_IRQ(TIMER2_IRQn, ECLIC_VECTOR_INTERRUPT,
                    ECLIC_LEVEL_TRIGGER, timer2_intlevel, 0, NULL);
    returnCode = ECLIC_Register_IRQ(TIMER1_IRQn, ECLIC_VECTOR_INTERRUPT,
                    ECLIC_LEVEL_TRIGGER, timer1_intlevel, 0, NULL);

    /* Enable interrupts in general */
    __enable_irq();

    /* Timer Start */
    timer_enable(TIMER1);
    delay_1ms(1000);
    timer_enable(TIMER2);

    /* RGB Control */
    while(1)
    {
    	/* set led to RED */
    	gd_rvstar_led_off(LED2);
    	gd_rvstar_led_off(LED1);
    	gd_rvstar_led_on(LED3);
    }

    return 0;
}


/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
void soc_timer_config()
{
    timer_parameter_struct timer_initpara;  
    
    /* ----------------------------------------------------------------------------
    TIMER1 Configuration:
    TIMER1CLK = SystemCoreClock/54000 = 2KHz.
    TIMER1CAR = 20000
    ---------------------------------------------------------------------------- */
    rcu_periph_clock_enable(RCU_TIMER1);

    timer_deinit(TIMER1);

    timer_update_source_config(TIMER1, TIMER_UPDATE_SRC_REGULAR);

    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 53999;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 20000;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER1, &timer_initpara);

    timer_interrupt_enable(TIMER1, TIMER_INT_UP);

    /* ----------------------------------------------------------------------------
    TIMER2 Configuration:
    TIMER2CLK = SystemCoreClock/54000 = 2KHz.
    TIMER2CAR = 20000
    ---------------------------------------------------------------------------- */
    rcu_periph_clock_enable(RCU_TIMER2);

    timer_deinit(TIMER2);

    timer_update_source_config(TIMER2, TIMER_UPDATE_SRC_REGULAR);

    /* TIMER2 configuration */
    timer_init(TIMER2, &timer_initpara);

    timer_interrupt_enable(TIMER2, TIMER_INT_UP);
}


/**
    \brief      configure the EXTI peripheral for user key
    \param[in]  none
    \param[out] none
    \retval     none
  */
void user_key_exti_config()
{
    /* enable the AF clock */
    rcu_periph_clock_enable(RCU_AF);

    /* connect EXTI line to key GPIO pin */
    gpio_exti_source_select(WAKEUP_KEY_EXTI_PORT_SOURCE, WAKEUP_KEY_EXTI_PIN_SOURCE);

    /* configure key EXTI line */
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_0);
}

/**
    \brief      EXTI line0 interrupt service routine
    \param[in]  none
    \param[out] none
    \retval     none
  */
void EXTI0_IRQHandler()
{

    if (SET == exti_interrupt_flag_get(WAKEUP_KEY_PIN)){

        if(RESET == gd_rvstar_key_state_get(KEY_WAKEUP)){

            /* clear EXTI lines interrupt flag */
            exti_interrupt_flag_clear(WAKEUP_KEY_PIN);

            /* set led to White */
            gd_rvstar_led_on(LED3);
            gd_rvstar_led_on(LED2);
            gd_rvstar_led_on(LED1);            

            delay_1ms(1000);
        }
    }

}

/**
    \brief      TIMER1 interrupt service routine
    \param[in]  none
    \param[out] none
    \retval     none
  */
__INTERRUPT void TIMER1_IRQHandler()
{
    uint16_t cnt;

    // save CSR context
    SAVE_IRQ_CSR_CONTEXT();

    if(SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)){
        /* clear update interrupt bit */
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);

    	for(cnt = 0; cnt < 5; cnt++)
    	{
        	/* set led to GREEN */
        	gd_rvstar_led_off(LED3);
        	gd_rvstar_led_off(LED2);
    		gd_rvstar_led_on(LED1);
    		delay_1ms(1000);
    	}
    }

    // restore CSR context
    RESTORE_IRQ_CSR_CONTEXT();
}


/**
    \brief      TIMER2 interrupt service routine
    \param[in]  none
    \param[out] none
    \retval     none
  */
__INTERRUPT void TIMER2_IRQHandler()
{
    if(SET == timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP)){
        /* clear update interrupt bit */
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);

        /* set led to BLUE */
        gd_rvstar_led_off(LED3);
        gd_rvstar_led_off(LED1);
        gd_rvstar_led_on(LED2);
        delay_1ms(2000);
    }
}