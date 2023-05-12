#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "adc.h"

uint8_t volatile batPct[BAT_NUM] = { 100, 100 };

#define BAT1_ADC_PIN      26
#define BAT2_ADC_PIN      27


void AdcInit()
{
    adc_init();
    adc_gpio_init(BAT1_ADC_PIN);
    adc_gpio_init(BAT2_ADC_PIN);
}

void AdcStart()
{
    for(uint16_t i = 0; i < BAT_NUM; i++)
	{
        adc_select_input(i);
        batPct[i] = (adc_read() - 1148) / 155 * 100;           // 2048-3.3  1303-2.1  2606-4.2 1148-1.85
        if (batPct[i] <= 0) {
            batPct[i] = 1;
        }
    }
}