// See LICENSE for license details.
#include "nuclei_sdk_hal.h"
#include <stdio.h>

void key_exti_init(void);

/**
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    int32_t returnCode;
    
    /* Board Config */
    gd_rvstar_led_init(LED3);
    gd_rvstar_key_init(WAKEUP_KEY_GPIO_PORT,KEY_MODE_EXTI);

    /* EXIT config */
    key_exti_init();

    /* ECLIC config */
    returnCode = ECLIC_Register_IRQ(EXTI0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                    ECLIC_LEVEL_TRIGGER, 1, 0, NULL);

    /* Enable interrupts in general */
    __enable_irq();

    while(1); 
    return 0;
}

/**
    \brief      initialize the EXTI configuration of the key
    \param[in]  none
    \param[out] none
    \retval     none
*/
void key_exti_init(void)
{
    /* enable the AF clock */
    rcu_periph_clock_enable(RCU_AF);

    /* connect key EXTI line to key GPIO pin */
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
void EXTI0_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(WAKEUP_KEY_PIN)){

        if(RESET == gd_rvstar_key_state_get(KEY_WAKEUP)){
            /* toggle RED led */
            gd_rvstar_led_toggle(LED3);
        }
    }
    /* clear EXTI lines pending flag */
    exti_interrupt_flag_clear(WAKEUP_KEY_PIN);
}


