#include "pico/multicore.h"
#include "sys_config.h"
#include "wireless_trans.h"
#include "spi.h"
#include "adc.h"
#include "ppm.h"
#include "pwm.h"
#include "sbus.h"

// 根据两位拨码开关确定对应的工作模式
void (*TaskInit)() = PPMInit;
void (*TaskStart)() = PPMStart;

void HW_Int()
{
    SPIInit();
    WRTInit();
    AdcInit();
    //PPMInit();
    //PWMInit();
    //SBUSInit();
    TaskInit();
}


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    stdio_init_all();

    // TODO: 根据两位拨码开关确定对应的工作模式
    // if (gpio_get(xx_pin1) == 0 && gpio_get(xx_pin2) == 0) {
    //     TaskInit = PPMInit;
    //     TaskStart = PPMStart;
    // }
    // else if ()

    HW_Int();
    
    multicore_launch_core1(TaskStart);
    StartSendPacket();

    return 0;    
}
