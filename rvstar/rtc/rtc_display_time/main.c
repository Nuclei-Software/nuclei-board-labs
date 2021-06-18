#include "nuclei_sdk_hal.h"
#include "rtc.h"
#include <stdio.h>

#define BKP_DATA_REG_NUM        42

extern __IO uint32_t timedisplay;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* COM0 configure */
    gd_com_init(GD32_COM0);

    /* ECLIC configure */
    ECLIC_Register_IRQ(RTC_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 0, NULL);
    __enable_irq();


    printf("\r\n This is a RTC demo...... \r\n");

    if (bkp_data_read(BKP_DATA_0) != 0xA5A5) {
        /* backup data register value is not correct or not yet programmed
        (when the first time the program is executed) */
        printf("\r\nThis is a RTC demo!\r\n");
        printf("\r\n\n RTC not yet configured....");

        /* RTC configuration */
        rtc_configuration();

        printf("\r\n RTC configured....");

        /* adjust time by values entred by the user on the hyperterminal */
        time_adjust();

        bkp_data_write(BKP_DATA_0, 0xA5A5);
    } else {
        /* check if the power on reset flag is set */
        if (rcu_flag_get(RCU_FLAG_PORRST) != RESET) {
            printf("\r\n\n Power On Reset occurred....");
        } else if (rcu_flag_get(RCU_FLAG_SWRST) != RESET) {
            /* check if the pin reset flag is set */
            printf("\r\n\n External Reset occurred....");
        }

        /* allow access to BKP domain */
        rcu_periph_clock_enable(RCU_PMU);
        pmu_backup_write_enable();

        printf("\r\n No need to configure RTC....");
        /* wait for RTC registers synchronization */
        rtc_register_sync_wait();

        /* enable the RTC second */
        rtc_interrupt_enable(RTC_INT_SECOND);

        /* wait until last write operation on RTC registers has finished */
        rtc_lwoff_wait();
    }

    /* clear reset flags */
    rcu_all_reset_flag_clear();

    /* display time in infinite loop */
    time_show();

    while (1) {
    }
}

/*!
    \brief      check if the backup data registers are clear or not
    \param[in]  none
    \param[out] none
    \retval     the number of data register
*/
uint32_t is_backup_register_clear(void)
{
    uint32_t temp = 0;

    for (temp = 0; temp < BKP_DATA_REG_NUM; temp++) {
        if (temp < 10) {
            /* check if the data of data register 0-9 is 0x0000 */
            if (0x0000 != BKP_DATA_GET(BKP_DATA0_9(temp))) {
                return temp + 1;
            }
        } else {
            /* check if the data of data register 10-41 is 0x0000 */
            if (0x0000 != BKP_DATA_GET(BKP_DATA10_41(temp))) {
                return temp + 1;
            }
        }
    }
    return 0;
}


void RTC_IRQHandler(void)
{
    if (rtc_flag_get(RTC_FLAG_SECOND) != RESET) {
        /* clear the RTC second interrupt flag*/
        rtc_flag_clear(RTC_FLAG_SECOND);

        /* enable time update */
        timedisplay = 1;

        /* wait until last write operation on RTC registers has finished */
        rtc_lwoff_wait();
        /* reset RTC counter when time is 23:59:59 */
        if (rtc_counter_get() == 0x00015180) {
            rtc_counter_set(0x0);
            /* wait until last write operation on RTC registers has finished */
            rtc_lwoff_wait();
        }
    }
}