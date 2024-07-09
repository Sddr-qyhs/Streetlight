#include "user_pulse.h"
#include "ls1x.h"
extern PMU_TypeDef *g_pmu;


void pulse_init(int num, uint32_t fre_d, int clk)// num �������ţ� fre_d �Ƿ�Ƶϵ���� clk Ϊ1��ѡ��8Mʱ�ӡ�
{
    uint32_t fre_d0 = 0x7d00;// 32K
    if(fre_d <= 65535 && fre_d >= 1)
    {
        fre_d0 = fre_d;
    }

    if(num == 1){
        PMU->PULSE1 &= ~(0x01<<17);// �������ʧ��
        if(clk)
            PMU->PULSE1 |= (0x01<<16);// ѡ��8Mʱ�ӡ�
        else
            PMU->PULSE1 &= ~(0x01<<16);// ѡ��32Kʱ�ӡ�
        PMU->PULSE1 |= fre_d0;// 32K��Ƶ��
        PMU->PULSE1 |= (0x01<<17);// �������ʹ�ܣ���ʱ�Ӻͷ�Ƶ���úú�ʹ�ܡ�
    }
    else if(num == 0)
    {
        PMU->PULSE0 &= ~(0x01<<17);// �������ʧ��
        if(clk)
            PMU->PULSE0 |= (0x01<<16);// ѡ��8Mʱ�ӡ�
        else
            PMU->PULSE0 &= ~(0x01<<16);// ѡ��32Kʱ�ӡ�
        PMU->PULSE0 |= fre_d0;// 32K��Ƶ��
        PMU->PULSE0 |= (0x01<<17);// �������ʹ�ܣ���ʱ�Ӻͷ�Ƶ���úú�ʹ�ܡ�
    }
}

void pulse_enable(int num)// num ��������
{
    if(num == 1){
        PMU->PULSE1 |= (0x01<<17);// �������ʹ�ܣ���ʱ�Ӻͷ�Ƶ���úú�ʹ�ܡ�
    }
    else if(num == 0)
    {
        PMU->PULSE0 |= (0x01<<17);// �������ʹ�ܣ���ʱ�Ӻͷ�Ƶ���úú�ʹ�ܡ�
    }
}

void pulse_disable(int num)// num ��������
{
    if(num == 1){
        PMU->PULSE1 &= ~(0x01<<17);// �������ʧ��
    }
    else if(num == 0)
    {
        PMU->PULSE0 &= ~(0x01<<17);// �������ʧ��
    }
}


