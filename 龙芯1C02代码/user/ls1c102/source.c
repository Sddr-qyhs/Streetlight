/*
 * source.c
 *
 * created: 2024/4/30
 *  author:
 */
#include "source.h"
#include "ls1x_uart.h"
#include "ls1x_gpio.h"
#include "ls1c102_adc.h"
#include "ls1c102_ptimer.h"


//*******************UART_SendData**************************
void UART0_SendDataStr(uint8_t Data)
{
    Uart0_send(Data);
}
void UART1_SendDataStr(uint8_t Data)
{
    Uart1_send(Data);
}
void UART2_SendDataStr(uint8_t Data)
{
    Uart2_send(Data);
}
//****************************UART_Init********************
void UART0_InitStr(uint32_t buad)
{
    Uart0_init(buad);
}
void UART1_InitStr(uint32_t buad)
{
    Uart1_init(buad);
}
void UART2_InitStr(uint32_t buad)
{
    Uart2_init(buad);
}

//******************Send_String*****************************
void UART0_SendString(char *str)
{
    uint8_t i = 0;
    for(i = 0; str[i]!='\0'; i++)
    {
        uart0.SendData(str[i]);
    }
}
void UART0_SendStringNo0(char *str)
{
    uint8_t i = 0;
    for(i = 0; str[i]!='\0'; i++)
    {
        if(str[i] == 0)
        {
            break;
        }
        uart0.SendData(str[i]);
    }
}
void UART1_SendString(char *str)
{
    uint8_t i = 0;
    for(i = 0; str[i]!='\0'; i++)
    {
        uart1.SendData(str[i]);
    }
}
void UART2_SendString(char *str)
{
    uint8_t i = 0;
    for(i = 0; str[i]!='\0'; i++)
    {
        uart2.SendData(str[i]);
    }
}
void UART0_SendArry(uint8_t *Arry , uint8_t len)
{
    uint8_t i = 0;
    for(i = 0; i<len; i++)
    {
        uart0.SendData(Arry[i]);
    }
}
void UART1_SendArry(uint8_t *Arry, uint8_t len)
{
    uint8_t i = 0;
    for(i = 0; i<len; i++)
    {
        uart1.SendData(Arry[i]);
    }
}
struct UART0STR uart0 =
{
    .SendData          =           UART0_SendDataStr,
    .Initialze         =           UART0_InitStr,
    .SendString        =           UART0_SendString,
    .SendArry          =           UART0_SendArry,
    .SendStrNo0        =           UART0_SendStringNo0
};
struct UART1STR uart1 =
{
    .SendData          =           UART1_SendDataStr,
    .Initialze         =           UART1_InitStr,
    .SendString        =           UART1_SendString,
    .SendArry         =            UART1_SendArry
};
struct UART2STR uart2 =
{
    .SendData          =           UART2_SendDataStr,
    .Initialze         =           UART2_InitStr,
    .SendString        =           UART2_SendString
};

void QueInit(Que *Que)
{
    memset(&Que , 0 ,sizeof(Que));
    Que->rear = Que->front = 0;
}
int Que_isFull(Que *Que)
{
    return (((Que->rear + 1) % MAX_LEN) == Que->front);
}
int Que_isEmpty(Que *Que)
{
    return (Que->front == Que->rear);
}
int Que_Enqueue(Que *Que , uint8_t data)
{
    if(Que_isFull(Que))
    {
        return -1;
    }
    Que->dat[Que->rear] = data ;
    Que->rear = ((Que->rear + 1) % MAX_LEN);
    return 0;
}
int Que_Dequeue(Que *Que ,uint8_t *data)
{
    if(Que_isEmpty(Que))
    {
        return -1;
    }
    *data = Que->dat[Que->front];
    Que->dat[Que->front] = '\0';
    Que->front = ((Que->front + 1) % MAX_LEN);
    return 0;
}
void Lux_Init(void)
{
    I2C_InitTypeDef I2C_InitStructure;
    I2C_StructInit(&I2C_InitStructure);
    I2C_Init(I2C ,&I2C_InitStructure);
}
void GetLux(uint16_t *Lux)
{
    uint16_t Value = 0;

    I2C_GenerateSTART(I2C ,ENABLE);
    I2C_wait(I2C);
    I2C_SendData(I2C, 0x46);
    I2C_wait(I2C);
    I2C_SendData(I2C, 0x10);
    I2C_wait(I2C);
    I2C_GenerateSTOP(I2C ,ENABLE);
    I2C_wait(I2C);

    delay_ms(180);

    I2C_GenerateSTART(I2C ,ENABLE);
    I2C_wait(I2C);
    I2C_SendData(I2C, 0x47);
    I2C_wait(I2C);
    I2C_ReceiveData(I2C, ENABLE, DISABLE);
    I2C_wait(I2C);
    Value =  I2C_ReadRegister(I2C ,I2C_Register_DR);
    I2C_ReceiveData(I2C, DISABLE, ENABLE);
    I2C_wait(I2C);
    Value = ((Value << 8) | I2C_ReadRegister(I2C ,I2C_Register_DR));

    *Lux = Value;
}

void right(int num)
{
    
    switch (num)
    {
        case 3:
            gpio_get_pin(GPIO_PIN_14);
            delay_ms(10);
            gpio_get_pin(GPIO_PIN_15);
            delay_ms(10);
            gpio_write_pin(GPIO_PIN_37,1);//高档
            break;
        case 2:
            gpio_get_pin(GPIO_PIN_15);
            delay_ms(10);
            gpio_get_pin(GPIO_PIN_37);
            delay_ms(10);
            gpio_write_pin(GPIO_PIN_14,1);//中档

            break;
        case 1:
            gpio_get_pin(GPIO_PIN_14);
            delay_ms(10);
            gpio_get_pin(GPIO_PIN_37);
            delay_ms(10);
            gpio_write_pin(GPIO_PIN_15,1);//低档
            break;
        case 0:
            gpio_get_pin(GPIO_PIN_14);
            delay_ms(10);
            gpio_get_pin(GPIO_PIN_15);
            delay_ms(10);
            gpio_get_pin(GPIO_PIN_37);//低档
            break;
        default:
            break;
    }
}
void ALIYUNswitch(){
    if(gpio_get_pin(GPIO_PIN_18)==0)
    {
        right(2);
    }
    else if(gpio_get_pin(GPIO_PIN_18)==1)
    {
        right(0);
    }
}
void uartRead()
{
    if(UART_ReceiveData(UART0))
    {
        gpio_write_pin(GPIO_PIN_17,0);
        delay_ms(5000);
        gpio_write_pin(GPIO_PIN_17,1);
    }
}

// 新的函数用于发送uint16_t类型的数据
void UART_SendData16(uint16_t Data)
{
    // 先发送高位字节（MSB）
    UART_SendData(UART1,(Data >> 8));  // 右移8位，得到高位字节
    // 再发送低位字节（LSB）
    UART_SendData(UART1,(Data & 0xFF));  // 与0xFF进行位与操作，得到低位字节
}

