#ifndef _ADC_H_
#define _ADC_H_

#include "pico/stdlib.h"

#define BAT_NUM          2


extern uint8_t volatile batPct[2];

void AdcInit();
void AdcStart();


#endif