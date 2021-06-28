#include "nuclei_sdk_hal.h"

uint32_t millis(void);
uint32_t start = 0;
uint32_t now = 0;

int main()
{
    /* enable IRC40K */
    rcu_osci_on(RCU_IRC40K);

    /* wait till IRC40K is ready */
    while (SUCCESS != rcu_osci_stab_wait(RCU_IRC40K)) ;

    /* confiure FWDGT counter clock: 40KHz(IRC40K) / 64 = 0.625 KHz */
    fwdgt_config(625, FWDGT_PSC_DIV64);
    /* after 1 seconds to generate a reset */
    fwdgt_enable();

    gd_rvstar_led_init(LED1);
    gd_rvstar_led_init(LED2);
    gd_rvstar_led_init(LED3);

    gd_rvstar_led_on(LED3);
    delay_1ms(50);
    gd_rvstar_led_off(LED3);

    start = millis();

    gd_rvstar_key_init(KEY_WAKEUP, KEY_MODE_EXTI);
    __enable_irq();

    while (1) {
        now = millis();
        if (now - start >= 900) {
            fwdgt_counter_reload();
            start = now;
        }

        /* toggle green led */
        gd_rvstar_led_toggle(LED1);
        delay_1ms(100);
    }
}

void EXTI0_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(WAKEUP_KEY_EXTI_LINE)) {

        if (RESET == gd_rvstar_key_state_get(KEY_WAKEUP)) {
            gd_rvstar_led_off(LED1);
            gd_rvstar_led_on(LED2);

            while (1);
        }
    }
    /* clear EXTI lines pending flag */
    exti_interrupt_flag_clear(WAKEUP_KEY_EXTI_LINE);
}

/**
 * \brief Returns the number of milliseconds since the board began running the current program.
 *
 * \return Number of milliseconds since the program started (uint32_t)
 */
uint32_t millis(void)
{
    return (uint32_t)(SysTimer_GetLoadValue() * (4000.F / SystemCoreClock));
}