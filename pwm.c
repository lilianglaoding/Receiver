#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "sys_config.h"
#include "pwm.h"


uint16_t volatile pwmValues[CH_NUM * 2] = { 1000, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500  };

#define  THR_PIN           19
#define  RUD_PIN           18
#define  ELE_PIN           17
#define  AIL_PIN           16
#define  AUX5_PIN          20
#define  AUX6_PIN          21
#define  AUX7_PIN          28
#define  AUX8_PIN          29


void PWMInit()
{
    // Tell GPIO 0 and 1 they are allocated to the PWM
    gpio_set_function(THR_PIN, GPIO_FUNC_PWM);
    gpio_set_function(RUD_PIN, GPIO_FUNC_PWM);
    gpio_set_function(ELE_PIN, GPIO_FUNC_PWM);
    gpio_set_function(AIL_PIN, GPIO_FUNC_PWM);
    gpio_set_function(AUX5_PIN, GPIO_FUNC_PWM);
    gpio_set_function(AUX6_PIN, GPIO_FUNC_PWM);
    gpio_set_function(AUX7_PIN, GPIO_FUNC_PWM);
    gpio_set_function(AUX8_PIN, GPIO_FUNC_PWM);

    uint slice_num_thr = pwm_gpio_to_slice_num(THR_PIN);
    uint slice_num_rud = pwm_gpio_to_slice_num(RUD_PIN);
    uint slice_num_ail = pwm_gpio_to_slice_num(AIL_PIN);
    uint slice_num_ele = pwm_gpio_to_slice_num(ELE_PIN);
    uint slice_num_aux5 = pwm_gpio_to_slice_num(AUX5_PIN);
    uint slice_num_aux6 = pwm_gpio_to_slice_num(AUX6_PIN);
    uint slice_num_aux7 = pwm_gpio_to_slice_num(AUX7_PIN);
    uint slice_num_aux8 = pwm_gpio_to_slice_num(AUX8_PIN); 

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 249.f);
    pwm_config_set_wrap(&config, 9999);

    pwm_init(slice_num_thr, &config, false);
    pwm_init(slice_num_rud, &config, false);
    pwm_init(slice_num_ail, &config, false);
    pwm_init(slice_num_ele, &config, false);
    pwm_init(slice_num_aux5, &config, false);
    pwm_init(slice_num_aux6, &config, false);
    pwm_init(slice_num_aux7, &config, false);
    pwm_init(slice_num_aux8, &config, false);
}

static float map(float value,float fromLow,float fromHigh,float toLow,float toHigh)
{
	return ((value-fromLow)*(toHigh-toLow)/(fromHigh-fromLow)+toLow);
}

void PWMStart()
{
    while (1)
    {
        pwm_set_gpio_level(THR_PIN, map(pwmValues[0], 1000, 2000, 240, 1000));  // need to modity toLow
        pwm_set_gpio_level(RUD_PIN, map(pwmValues[1], 1000, 2000, 500, 1000));
        pwm_set_gpio_level(ELE_PIN, map(pwmValues[2], 1000, 2000, 500, 1000));
        pwm_set_gpio_level(AIL_PIN, map(pwmValues[3], 1000, 2000, 500, 1000));
        pwm_set_gpio_level(AUX5_PIN, map(pwmValues[4], 1000, 2000, 500, 1000));
        pwm_set_gpio_level(AUX6_PIN, map(pwmValues[5], 1000, 2000, 500, 1000));
        pwm_set_gpio_level(AUX7_PIN, map(pwmValues[6], 1000, 2000, 500, 1000));
        pwm_set_gpio_level(AUX8_PIN, map(pwmValues[7], 1000, 2000, 500, 1000));
    }
}