#include "nuclei_sdk_hal.h"

void uart_init();
void led_init();
void led_off();
void led_on(uint8_t color);

volatile char led_color;

int main()
{
    uart_init();
    led_init();

    ECLIC_Register_IRQ(UART4_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 0, NULL);
    __enable_irq();

    usart_interrupt_enable(UART4, USART_INT_RBNE);

    while (1) {
        led_on(led_color);
    }
}


void uart_init()
{
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_UART4);

    // UART4_TX - PC12
    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    // UART4_RX - PD2
    gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    usart_deinit(UART4);
    usart_baudrate_set(UART4, 115200U);
    usart_word_length_set(UART4, USART_WL_8BIT);
    usart_stop_bit_set(UART4, USART_STB_1BIT);
    usart_parity_config(UART4, USART_PM_NONE);
    usart_hardware_flow_rts_config(UART4, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(UART4, USART_CTS_DISABLE);
    usart_receive_config(UART4, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART4, USART_TRANSMIT_ENABLE);
    usart_enable(UART4);
}

void led_init()
{
    gd_led_init(LED1);
    gd_led_init(LED2);
    gd_led_init(LED3);
}

void led_off()
{
    gd_led_off(LED1);
    gd_led_off(LED2);
    gd_led_off(LED3);
}

void led_on(uint8_t color)
{
    led_off();
    switch (color) {
        case 'r':
            gd_led_on(LED3);
            break;
        case 'g':
            gd_led_on(LED1);
            break;
        case 'b':
            gd_led_on(LED2);
            break;
        default:
            break;
    }
}

void UART4_IRQHandler(void)
{
    if (RESET != usart_interrupt_flag_get(UART4, USART_INT_FLAG_RBNE)) {
        led_color = usart_data_receive(UART4);
    }
}