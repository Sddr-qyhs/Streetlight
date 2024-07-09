#include "ls1x.h"
#include "config.h"
#include "ls1x_gpio.h"
#include "ls1x_latimer.h"
#include "ls1c102_interrupt.h"
#include "source.h"

unsigned char switchLight=0;
unsigned char temp=0;
uint8_t str[20];

#define OK 1
#define NO 0
#define TEST    NO


#if TEST==NO
int main(void)
{
    ///��ʼ�����֣�
    uint16_t Value;//���ձ���
    uart1.Initialze(9600);//��ʼ������1������Ϊ9600
    Lux_Init();//���մ�������ʼ��
    gpio_pin_remap(GPIO_PIN_14,GPIO_FUNC_GPIO);//��ʼ��GPIO
    gpio_pin_remap(GPIO_PIN_15,GPIO_FUNC_GPIO);
    gpio_pin_remap(GPIO_PIN_16,GPIO_FUNC_GPIO);


    void uart_send()//���ڷ��ͺ��������͵�4Gģ�飩
    {
        if(gpio_get_pin(GPIO_PIN_40))
        {
            if(Value>0&&Value<10)
            {
                sprintf(str,"0000%dLUX" ,Value);
                uart1.SendString(str);
            }
            else if(Value>9&&Value<100)
            {
                sprintf(str,"000%dLUX" ,Value);
                uart1.SendString(str);
            }
            else if(Value>99&&Value<1000)
            {
                sprintf(str ,"00%dLUX" ,Value);
                uart1.SendString(str);
            }
            else if(Value>999&&Value<10000)
            {
                sprintf(str ,"0%dLUX" ,Value);
                uart1.SendString(str);
            }
            else if(Value>9999)
            {
                sprintf(str ,"%dLUX" ,Value);
                uart1.SendString(str);
            }
        }
    }
//��ѭ�����֣�
    while(1)
    {
        GetLux(&Value);//��ȡ����ǿ��(��λLUX)
        uart_send();//���ڷ��ͺ��������͵�4Gģ�飩
        switchLight=gpio_get_pin(GPIO_PIN_18);//��ȡ4Gģ��ش��ź�
        if(switchLight==1)//���4Gģ��ش��ź�Ϊ1·����������
        {
            switchLight=gpio_get_pin(GPIO_PIN_18);
            temp=gpio_get_pin(GPIO_PIN_13);//��ȡ�״�ģ���ź�
            if(Value<=9)//�������ǿ��С��9
            {
                if(temp==1)//��Χ����
                {
                    uart_send();
                    delay_ms(10);
                    right(3);//��·��3��
                    delay_ms(10);
                    gpio_write_pin(GPIO_PIN_16,1);//���������
                    delay_ms(20000);
                    temp=gpio_get_pin(GPIO_PIN_13);
                    
                }
                else if(temp==0)//��Χû��
                {
                    uart_send();
                    delay_ms(10);
                    right(1);//·��1��
                    delay_ms(10);
                
                
                    gpio_write_pin(GPIO_PIN_16,0);//�رչ����
                    delay_ms(10);
                    delay_s(10);
                    temp=gpio_get_pin(GPIO_PIN_13);

                }
            }

            GetLux(&Value);
            uart_send();
            delay_ms(10);
            temp=gpio_get_pin(GPIO_PIN_13);
            switchLight=gpio_get_pin(GPIO_PIN_18);
            if(Value>10&&Value<100)
            {
                ALIYUNswitch();
                if(temp==1)
                {
                    uart_send();
                    delay_ms(10);
                    right(2);
                    delay_ms(10);
                   
                    gpio_write_pin(GPIO_PIN_16,1);
                    delay_ms(10);
                    delay_ms(20000);
                    temp=gpio_get_pin(GPIO_PIN_13);
                }
                else if(temp==0)
                {
                uart_send();
                    delay_ms(10);
                    right(1);
                  
                    delay_ms(10);
                    delay_s(10);
                    temp=gpio_get_pin(GPIO_PIN_13);
                }
            }
            GetLux(&Value);
            uart_send();
            delay_ms(10);
            temp=gpio_get_pin(GPIO_PIN_13);
            switchLight=gpio_get_pin(GPIO_PIN_18);
            if(Value>=100)
            {
                if(temp==1)
                {
                    gpio_write_pin(GPIO_PIN_16,1);
                    delay_ms(10);
                    right(0);
                    uart_send();
                    delay_ms(10);
                    delay_s(5);
                }
                else if(temp==0)
                {
                    gpio_write_pin(GPIO_PIN_16,0);
                    delay_ms(10);
                    right(0);
                    GetLux(&Value);
                    uart_send();
                    delay_ms(10);
                    delay_s(5);
                }
                uart_send();
            }
        }
        else if (switchLight==1)
        {
            while(switchLight)
            {
                right(0);
            }
        }
    }
    return 0;
}
#else
int main(void)
{


    while(1)
    {

    }

    return 0;
}
#endif

