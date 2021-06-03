#include "nuclei_sdk_hal.h"

uint8_t rxbuffer[10];
uint8_t txbuffer[] = "\nUART DMA receive and transmit example, please input 10 bytes:\n";
#define ARRAYNUM(arr_name)     (uint32_t)(sizeof(arr_name) / sizeof(*(arr_name)))

void uart_init();
void uart_send(int ch);

int main()
{
    dma_parameter_struct dma_init_struct;
    /* enable DMA1 */
    rcu_periph_clock_enable(RCU_DMA1);
    /* initialize UART3 */
    uart_init();

    /* deinitialize DMA Channel4(UART3_TX) */
    dma_deinit(DMA1, DMA_CH4);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)txbuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = ARRAYNUM(txbuffer) - 1;
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(UART3);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA1, DMA_CH4, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH4);
    /* enable DMA channel3 */
    dma_channel_enable(DMA1, DMA_CH4);

    /* USART DMA enable for transmission and reception */
    usart_dma_transmit_config(UART3, USART_DENT_ENABLE);
    usart_dma_receive_config(UART3, USART_DENR_ENABLE);

    /* wait DMA Channel transfer complete */
    while (RESET == dma_flag_get(DMA1, DMA_CH4, DMA_FLAG_FTF));
    while (1) {
        /* deinitialize DMA1 Channel2 (UART3_RX) */
        dma_deinit(DMA1, DMA_CH2);
        dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
        dma_init_struct.memory_addr = (uint32_t)rxbuffer;
        dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
        dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
        dma_init_struct.number = 10;
        dma_init_struct.periph_addr = (uint32_t)&USART_DATA(UART3);
        dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
        dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
        dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
        dma_init(DMA1, DMA_CH2, &dma_init_struct);
        /* configure DMA mode */
        dma_circulation_disable(DMA1, DMA_CH2);
        /* enable DMA channel4 */
        dma_channel_enable(DMA1, DMA_CH2);

        /* wait DMA channel transfer complete */
        while (RESET == dma_flag_get(DMA1, DMA_CH2, DMA_FLAG_FTF));

        for (int i = 0; i < 10; i++) {
            uart_send(rxbuffer[i]);
        }
        uart_send('\n');
    }
}

void uart_init()
{
    /* enable GPIO TX and RX clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_UART3);

    /* connect port to UART3_TX */
    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    /* connect port to UART3_RX */
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    /* UART configure */
    usart_deinit(UART3);
    usart_baudrate_set(UART3, 115200U);
    usart_word_length_set(UART3, USART_WL_8BIT);
    usart_stop_bit_set(UART3, USART_STB_1BIT);
    usart_parity_config(UART3, USART_PM_NONE);
    usart_hardware_flow_rts_config(UART3, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(UART3, USART_CTS_DISABLE);
    usart_receive_config(UART3, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
    usart_enable(UART3);
}


void uart_send(int ch)
{
    usart_data_transmit(UART3, (uint8_t)ch);
    while (usart_flag_get(UART3, USART_FLAG_TBE) == RESET);
}