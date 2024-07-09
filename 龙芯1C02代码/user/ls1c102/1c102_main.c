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
    ///初始化部分：
    uint16_t Value;//光照变量
    uart1.Initialze(9600);//初始化串口1波特率为9600
    Lux_Init();//光照传感器初始化
    gpio_pin_remap(GPIO_PIN_14,GPIO_FUNC_GPIO);//初始化GPIO
    gpio_pin_remap(GPIO_PIN_15,GPIO_FUNC_GPIO);
    gpio_pin_remap(GPIO_PIN_16,GPIO_FUNC_GPIO);


    void uart_send()//串口发送函数（发送到4G模块）
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
//主循环部分：
    while(1)
    {
        GetLux(&Value);//获取光照强度(单位LUX)
        uart_send();//串口发送函数（发送到4G模块）
        switchLight=gpio_get_pin(GPIO_PIN_18);//获取4G模块回传信号
        if(switchLight==1)//如果4G模块回传信号为1路灯正常运行
        {
            switchLight=gpio_get_pin(GPIO_PIN_18);
            temp=gpio_get_pin(GPIO_PIN_13);//读取雷达模块信号
            if(Value<=9)//如果光照强度小于9
            {
                if(temp==1)//周围有人
                {
                    uart_send();
                    delay_ms(10);
                    right(3);//打开路灯3档
                    delay_ms(10);
                    gpio_write_pin(GPIO_PIN_16,1);//启动广告屏
                    delay_ms(20000);
                    temp=gpio_get_pin(GPIO_PIN_13);
                    
                }
                else if(temp==0)//周围没人
                {
                    uart_send();
                    delay_ms(10);
                    right(1);//路灯1档
                    delay_ms(10);
                
                
                    gpio_write_pin(GPIO_PIN_16,0);//关闭广告屏
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

