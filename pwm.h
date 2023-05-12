#ifndef _PWM_H_
#define _PWM_H_

#include "sys_config.h"

extern uint16_t volatile pwmValues[CH_NUM * 2];

void PWMInit();
void PWMStart();


#endif