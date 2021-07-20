#include "nuclei_sdk_hal.h"

void gd_rvstar_led_blink(int led);

int main()
{
    gd_rvstar_led_init(LED1);
    gd_rvstar_led_init(LED3);
    gd_rvstar_key_init(KEY_WAKEUP, KEY_MODE_EXTI);
    rcu_periph_clock_enable(RCU_PMU);

    gd_rvstar_led_on(LED3);

    pmu_to_deepsleepmode(PMU_LDO_LOWPOWER, WFI_CMD);

    gd_rvstar_led_off(LED3);
    while (1) {
        gd_rvstar_led_blink(LED1);
    }
}

void gd_rvstar_led_blink(int led)
{
    gd_rvstar_led_on(led);
    delay_1ms(1000);
    gd_rvstar_led_off(led);
    delay_1ms(1000);
}

void EXTI0_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(WAKEUP_KEY_EXTI_LINE)) {
        /* clear EXTI lines pending flag */
        exti_interrupt_flag_clear(WAKEUP_KEY_EXTI_LINE);
    }
}