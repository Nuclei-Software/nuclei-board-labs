#include "nuclei_sdk_hal.h"

void rcu_config(void);
void gpio_config(void);
void adc_config(void);
void dac_config(void);

uint16_t input_value;

int main()
{
    rcu_config();
    gpio_config();
    adc_config();
    dac_config();

    while (1) {
        input_value = adc_regular_data_read(ADC1);
        dac_data_set(DAC1, DAC_ALIGN_12B_R, input_value);
        dac_software_trigger_enable(DAC1);

        delay_1ms(50);
    }

}

void rcu_config(void)
{
    /* enable the clock of peripherals */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_ADC1);
    rcu_periph_clock_enable(RCU_DAC);
}

void gpio_config(void)
{
    /* config the GPIO as analog mode */
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
    /* once enabled the DAC, the corresponding GPIO pin is connected to the DAC converter automatically */
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
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

void dac_config(void)
{
    dac_deinit();
    /* configure the DAC1 */
    dac_trigger_source_config(DAC1, DAC_TRIGGER_SOFTWARE);
    dac_trigger_enable(DAC1);
    /* enable DAC1 */
    dac_enable(DAC1);
}