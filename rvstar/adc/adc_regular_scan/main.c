#include "nuclei_sdk_hal.h"
#include <stdio.h>

void rcu_config(void);
void gpio_config(void);
void adc_config(void);

uint16_t adc_value;

int main(void)
{
    rcu_config();
    gpio_config();
    adc_config();
    gd_com_init(GD32_COM0);

    while (1) {
        delay_1ms(1000);
        adc_value = adc_regular_data_read(ADC1);

        printf("\r\n ADC1 regular channel 10 data = %d \r\n", adc_value);
        printf("\r\n ***********************************\r\n");
    }
}

void rcu_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_ADC1);
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);
}

void gpio_config(void)
{
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
}

void adc_config(void)
{
    /* reset ADC */
    adc_deinit(ADC1);
    /* ADC mode config */
    adc_mode_config(ADC1, ADC_MODE_FREE);
    /* ADC scan function enable */
    adc_special_function_config(ADC1, ADC_SCAN_MODE, ENABLE);
    /* ADC contineous function enable */
    adc_special_function_config(ADC1, ADC_CONTINUOUS_MODE, ENABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(ADC1, ADC_REGULAR_CHANNEL, 1);

    adc_external_trigger_source_config(ADC1, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, ENABLE);
    delay_1ms(1);

    adc_enable(ADC1);
    adc_calibration_enable(ADC1);

    adc_software_trigger_enable(ADC1, ADC_REGULAR_CHANNEL);
    adc_regular_channel_config(ADC1, 0, ADC_CHANNEL_10, ADC_SAMPLETIME_55POINT5);
}