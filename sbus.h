#ifndef _SBUS_H_
#define _SBUS_H_

#include "sys_config.h"

#define SBUS_PACKET_LENGTH          25

extern uint8_t volatile sbusValues[SBUS_PACKET_LENGTH];
uint8_t isSignalLoss;

void SBUSInit();
void SBUSStart();

#endif