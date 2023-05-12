#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "sys_config.h"
#include "pwm.h"


#define PWM_CHANNEL_MIN             1000
#define PWM_CHANNEL_MAX             2000
#define SBUS_MIN_OFFSET             173
#define SBUS_MID_OFFSET             992
#define SBUS_MAX_OFFSET             1811
#define SBUS_CHANNEL_NUMBER         16
#define SBUS_PACKET_LENGTH          25
#define SBUS_FRAME_HEADER           0x0f
#define SBUS_FRAME_FOOTER           0x00
#define SBUS_FRAME_FOOTER_V2        0x04
#define SBUS_STATE_FAILSAFE         0x08
#define SBUS_STATE_SIGNALLOSS       0x04
#define SBUS_UPDATE_RATE            10

uint8_t volatile sbusValues[SBUS_PACKET_LENGTH] = { 0 };
uint8_t stateByte = 0x00;
uint8_t isSignalLoss = 0;

#define UART_ID uart0
#define BAUD_RATE 100000
#define DATA_BITS 8
#define STOP_BITS 2
#define PARITY    UART_PARITY_NONE
#define UART_TX_PIN 12


void SBUSInit()
{
	uart_init(UART_ID, BAUD_RATE);
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
	uart_set_hw_flow(UART_ID, false, false);
	uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
	uart_set_fifo_enabled(UART_ID, false);
}

static float map(float value,float fromLow,float fromHigh,float toLow,float toHigh)
{
	return ((value-fromLow)*(toHigh-toLow)/(fromHigh-fromLow)+toLow);
}


void SBUSStart()
{
	while (1) {
        static uint16_t output[SBUS_CHANNEL_NUMBER] = {0};
        for (uint8_t i = 0; i < SBUS_CHANNEL_NUMBER; i++) {
            output[i] = (int)map(pwmValues[i], PWM_CHANNEL_MIN, PWM_CHANNEL_MAX, SBUS_MIN_OFFSET, SBUS_MAX_OFFSET);
        }
    
        if (isSignalLoss) {
            stateByte |= SBUS_STATE_SIGNALLOSS;
        }

        uint8_t isFailsafe = 0x00;
        if (isFailsafe) {
            stateByte |= SBUS_STATE_FAILSAFE;
        }

        sbusValues[0] = SBUS_FRAME_HEADER;
        sbusValues[1] = (uint8_t) (output[0] & 0x07FF);
        sbusValues[2] = (uint8_t) ((output[0] & 0x07FF)>>8 | (output[1] & 0x07FF)<<3);
        sbusValues[3] = (uint8_t) ((output[1] & 0x07FF)>>5 | (output[2] & 0x07FF)<<6);
        sbusValues[4] = (uint8_t) ((output[2] & 0x07FF)>>2);
        sbusValues[5] = (uint8_t) ((output[2] & 0x07FF)>>10 | (output[3] & 0x07FF)<<1);
        sbusValues[6] = (uint8_t) ((output[3] & 0x07FF)>>7 | (output[4] & 0x07FF)<<4);
        sbusValues[7] = (uint8_t) ((output[4] & 0x07FF)>>4 | (output[5] & 0x07FF)<<7);
        sbusValues[8] = (uint8_t) ((output[5] & 0x07FF)>>1);
        sbusValues[9] = (uint8_t) ((output[5] & 0x07FF)>>9 | (output[6] & 0x07FF)<<2);
        sbusValues[10] = (uint8_t) ((output[6] & 0x07FF)>>6 | (output[7] & 0x07FF)<<5);
        sbusValues[11] = (uint8_t) ((output[7] & 0x07FF)>>3);
        sbusValues[12] = (uint8_t) ((output[8] & 0x07FF));
        sbusValues[13] = (uint8_t) ((output[8] & 0x07FF)>>8 | (output[9] & 0x07FF)<<3);
        sbusValues[14] = (uint8_t) ((output[9] & 0x07FF)>>5 | (output[10] & 0x07FF)<<6);  
        sbusValues[15] = (uint8_t) ((output[10] & 0x07FF)>>2);
        sbusValues[16] = (uint8_t) ((output[10] & 0x07FF)>>10 | (output[11] & 0x07FF)<<1);
        sbusValues[17] = (uint8_t) ((output[11] & 0x07FF)>>7 | (output[12] & 0x07FF)<<4);
        sbusValues[18] = (uint8_t) ((output[12] & 0x07FF)>>4 | (output[13] & 0x07FF)<<7);
        sbusValues[19] = (uint8_t) ((output[13] & 0x07FF)>>1);
        sbusValues[20] = (uint8_t) ((output[13] & 0x07FF)>>9 | (output[14] & 0x07FF)<<2);
        sbusValues[21] = (uint8_t) ((output[14] & 0x07FF)>>6 | (output[15] & 0x07FF)<<5);
        sbusValues[22] = (uint8_t) ((output[15] & 0x07FF)>>3);
        sbusValues[23] = stateByte;
        sbusValues[24] = SBUS_FRAME_FOOTER;

		uart_write_blocking(UART_ID, sbusValues, SBUS_PACKET_LENGTH);
	}
}