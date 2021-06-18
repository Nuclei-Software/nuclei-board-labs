#ifndef __RTC_H
#define __RTC_H

#include "nuclei_sdk_hal.h"
#include <stdio.h>

void rtc_configuration(void);
uint32_t time_regulate(void);
void time_adjust(void);
void time_display(uint32_t timevar);
void time_show(void);
uint8_t usart_scanf(uint32_t value);

#endif /* __RTC_H */

