// See LICENSE for license details.

#include "main.h"

/**
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* USART(Printf) init */
    log_init();

    /* LED init */
    LedInit(LED1_PORT, LED1_PIN);  // LED6
    LedInit(LED2_PORT, LED2_PIN);  // LED7
    LedInit(LED3_PORT, LED3_PIN);  // LED8

    LedOff(LED1_PORT, LED1_PIN);
    LedOff(LED2_PORT, LED2_PIN);
    LedOff(LED3_PORT, LED3_PIN);

    /* Key init */
    KeyInputInit(KEY_INPUT_PORT1, KEY_INPUT_PIN1);   // WKUP Key
    KeyInputInit(KEY_INPUT_PORT2, KEY_INPUT_PIN2);   // TMP  Key
    KeyInputInit(KEY_INPUT_PORT3, KEY_INPUT_PIN3);   // USER Key


    printf("\r\n USART printf & LED & Key example \r\n");
    printf("\r\n Press the USER button, the status of LED6 will be changed \r\n");
    printf("\r\n Press the WKUP button, the status of LED7 will be changed \r\n");
    printf("\r\n Press the TMP  button, the status of LED8 will be changed \r\n");

    while(1) {

        if(RESET == KeyReadValue(KEY_INPUT_PORT3, KEY_INPUT_PIN3)) {
        	delay_ms(100);
        	if(RESET == KeyReadValue(KEY_INPUT_PORT3, KEY_INPUT_PIN3)) LedBlink(LED1_PORT, LED1_PIN);
        }

        if(RESET == KeyReadValue(KEY_INPUT_PORT1, KEY_INPUT_PIN1)) {
        	delay_ms(100);
        	if(RESET == KeyReadValue(KEY_INPUT_PORT1, KEY_INPUT_PIN1)) LedBlink(LED2_PORT, LED2_PIN);
        }

        if(RESET == KeyReadValue(KEY_INPUT_PORT2, KEY_INPUT_PIN2)) {
        	delay_ms(100);
        	if(RESET == KeyReadValue(KEY_INPUT_PORT2, KEY_INPUT_PIN2)) LedBlink(LED3_PORT, LED3_PIN);
        }
    }

    return 0;
}
