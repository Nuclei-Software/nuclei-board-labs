// See LICENSE for license details.
#include <stdio.h>
#include "hbird_sdk_hal.h"


int main(void)
{

    printf("GPIO Toggle Test\r\n");

    printf("Press Button U, LED0 will be changed\r\n");
   
    // GPIO Init
    // Set BUTTON_U as input, LED0 as output 
    gpio_enable_input(GPIOA, SOC_BUTTON_U_GPIO_MASK);
    gpio_enable_output(GPIOA, SOC_LED_0_GPIO_MASK);
    
    // Set LED0 value to 0, off
    gpio_write(GPIOA, SOC_LED_0_GPIO_MASK, 0);


    while (1) {

	// Check value of BUTTON_U, if pressed, change status of LED0
        if(gpio_read(GPIOA, SOC_BUTTON_U_GPIO_MASK) == SOC_BUTTON_U_GPIO_MASK) {
           gpio_toggle(GPIOA, SOC_LED_0_GPIO_MASK);
   	};

	delay_1ms(100);
    }
    return 0;
}

