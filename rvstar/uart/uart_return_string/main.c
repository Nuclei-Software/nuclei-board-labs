#include "nuclei_sdk_hal.h"

#define RX_BUFFER_SIZE 64

void uart_init();
void uart_send(uint8_t c);
uint8_t uart_read();
uint8_t rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t rb_head = 0;
volatile uint8_t rb_tail = 0;


int main()
{
    uart_init();

    ECLIC_Register_IRQ(UART4_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 0, NULL);
    __enable_irq();

    usart_interrupt_enable(UART4, USART_INT_RBNE);

    while (1) {
        if (rb_head != rb_tail && rx_buffer[(rb_head + RX_BUFFER_SIZE - 1) % RX_BUFFER_SIZE] == '\n') {
            uart_send(uart_read());
        }
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

void uart_send(uint8_t c)
{
    usart_data_transmit(UART4, c);
    while (SET != usart_flag_get(UART4, USART_FLAG_TBE));
}

uint8_t uart_read()
{
    if (rb_head != rb_tail) {
        uint8_t c = rx_buffer[rb_tail];
        rb_tail = (rb_tail + 1) % RX_BUFFER_SIZE;
        return c;
    } else {
        return -1;
    }
}

void UART4_IRQHandler(void)
{
    if (RESET != usart_interrupt_flag_get(UART4, USART_INT_FLAG_RBNE)) {
        char c = usart_data_receive(UART4);
        uint8_t i = (rb_head + 1) % RX_BUFFER_SIZE;
        if (i != rb_tail) {
            rx_buffer[rb_head] = c;
            rb_head = i;
        }
    }
}