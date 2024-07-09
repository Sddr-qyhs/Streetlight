#include "user_pulse.h"
#include "ls1x.h"
extern PMU_TypeDef *g_pmu;


void pulse_init(int num, uint32_t fre_d, int clk)// num 是脉冲编号， fre_d 是分频系数， clk 为1就选择8M时钟。
{
    uint32_t fre_d0 = 0x7d00;// 32K
    if(fre_d <= 65535 && fre_d >= 1)
    {
        fre_d0 = fre_d;
    }

    if(num == 1){
        PMU->PULSE1 &= ~(0x01<<17);// 脉冲输出失能
        if(clk)
            PMU->PULSE1 |= (0x01<<16);// 选择8M时钟。
        else
            PMU->PULSE1 &= ~(0x01<<16);// 选择32K时钟。
        PMU->PULSE1 |= fre_d0;// 32K分频。
        PMU->PULSE1 |= (0x01<<17);// 脉冲输出使能，在时钟和分频配置好后使能。
    }
    else if(num == 0)
    {
        PMU->PULSE0 &= ~(0x01<<17);// 脉冲输出失能
        if(clk)
            PMU->PULSE0 |= (0x01<<16);// 选择8M时钟。
        else
            PMU->PULSE0 &= ~(0x01<<16);// 选择32K时钟。
        PMU->PULSE0 |= fre_d0;// 32K分频。
        PMU->PULSE0 |= (0x01<<17);// 脉冲输出使能，在时钟和分频配置好后使能。
    }
}

void pulse_enable(int num)// num 是脉冲编号
{
    if(num == 1){
        PMU->PULSE1 |= (0x01<<17);// 脉冲输出使能，在时钟和分频配置好后使能。
    }
    else if(num == 0)
    {
        PMU->PULSE0 |= (0x01<<17);// 脉冲输出使能，在时钟和分频配置好后使能。
    }
}

void pulse_disable(int num)// num 是脉冲编号
{
    if(num == 1){
        PMU->PULSE1 &= ~(0x01<<17);// 脉冲输出失能
    }
    else if(num == 0)
    {
        PMU->PULSE0 &= ~(0x01<<17);// 脉冲输出失能
    }
}


