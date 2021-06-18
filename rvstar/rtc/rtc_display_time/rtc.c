#include "rtc.h"

/* enter the second interruption,set the second interrupt flag to 1 */
__IO uint32_t timedisplay = 0;

/*!
    \brief      configure the RTC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_configuration(void)
{
    /* enable PMU and BKPI clocks */
    rcu_periph_clock_enable(RCU_BKPI);
    rcu_periph_clock_enable(RCU_PMU);
    /* allow access to BKP domain */
    pmu_backup_write_enable();

    /* reset backup domain */
    bkp_deinit();

    /* enable LXTAL */
    rcu_osci_on(RCU_LXTAL);
    /* wait till LXTAL is ready */
    rcu_osci_stab_wait(RCU_LXTAL);

    /* select RCU_LXTAL as RTC clock source */
    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);

    /* enable RTC Clock */
    rcu_periph_clock_enable(RCU_RTC);

    /* wait for RTC registers synchronization */
    rtc_register_sync_wait();

    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();

    /* enable the RTC second interrupt*/
    rtc_interrupt_enable(RTC_INT_SECOND);

    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();

    /* set RTC prescaler: set RTC period to 1s */
    rtc_prescaler_set(32767);

    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
}

/*!
    \brief      return the time entered by user, using Hyperterminal
    \param[in]  none
    \param[out] none
    \retval     current time of RTC counter value
*/
uint32_t time_regulate(void)
{
    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;

    printf("\r\n==============Time Settings=====================================");
    printf("\r\n  Please Set Hours");

    while (tmp_hh == 0xFF) {
        tmp_hh = usart_scanf(23);
    }
    printf(":  %d", tmp_hh);
    printf("\r\n  Please Set Minutes");
    while (tmp_mm == 0xFF) {
        tmp_mm = usart_scanf(59);
    }
    printf(":  %d", tmp_mm);
    printf("\r\n  Please Set Seconds");
    while (tmp_ss == 0xFF) {
        tmp_ss = usart_scanf(59);
    }
    printf(":  %d", tmp_ss);

    /* return the value  store in RTC counter register */
    return ((tmp_hh * 3600 + tmp_mm * 60 + tmp_ss));
}

/*!
    \brief      adjust time
    \param[in]  none
    \param[out] none
    \retval     none
*/
void time_adjust(void)
{
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    /* change the current time */
    rtc_counter_set(time_regulate());
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
}

/*!
    \brief      display the current time
    \param[in]  timeVar: RTC counter value
    \param[out] none
    \retval     none
*/
void time_display(uint32_t timevar)
{
    uint32_t thh = 0, tmm = 0, tss = 0;

    /* compute  hours */
    thh = timevar / 3600;
    /* compute minutes */
    tmm = (timevar % 3600) / 60;
    /* compute seconds */
    tss = (timevar % 3600) % 60;

    printf(" Time: %0.2d:%0.2d:%0.2d\r\n", thh, tmm, tss);
}

/*!
    \brief      show the current time (HH:MM:SS) on the Hyperterminal
    \param[in]  none
    \param[out] none
    \retval     none
*/
void time_show(void)
{
    printf("\n\r");

    /* infinite loop */
    while (1) {
        /* if 1s has paased */
        if (timedisplay == 1) {
            /* display current time */
            time_display(rtc_counter_get());
            timedisplay = 0;
        }
    }
}

/*!
    \brief      get numeric values from the hyperterminal
    \param[in]  value: input value from the hyperterminal
    \param[out] none
    \retval     input value in BCD mode
*/
uint8_t usart_scanf(uint32_t value)
{
    uint32_t index = 0;
    uint32_t tmp[2] = {0, 0};

    while (index < 2) {
        /* loop until RBNE = 1 */
        while (usart_flag_get(UART4, USART_FLAG_RBNE) == RESET);
        tmp[index++] = (usart_data_receive(UART4));

        if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39)) {
            printf("\n\rPlease enter valid number between 0 and 9\n");
            index--;
        }
    }
    /* calculate the Corresponding value */
    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
    /* check */
    if (index > value) {
        printf("\n\rPlease enter valid number between 0 and %d\n", value);
        return 0xFF;
    }
    return index;
}
