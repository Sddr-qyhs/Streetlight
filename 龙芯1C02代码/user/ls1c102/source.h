/*
 * source.h
 *
 * created: 2024/4/30
 *  author: 
 */

#ifndef _SOURCE_H
#define _SOURCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ls1x.h"
#include "ls1c102_adc.h"
#include "ls1x_uart.h"
#include "ls1c102_i2c.h"

#include <string.h>

//*********************************************************UART***************************************
struct UART0STR{
    void (*SendData)(uint8_t Data);
    void (*Initialze)(uint32_t buad);
    void (*SendString)(char *str);
    void (*SendArry)(uint8_t *Arry , uint8_t len);
    void (*SendStrNo0)(char *str);
};
struct UART1STR{
    void (*SendData)(uint8_t Data);
    void (*Initialze)(uint32_t buad);
    void (*SendString)(char *str);
    void (*SendArry)(uint8_t *Arry , uint8_t len);
};
struct UART2STR{
    void (*SendData)(uint8_t Data);
    void (*Initialze)(uint32_t buad);
    void (*SendString)(char *str);
};

extern struct UART0STR uart0;
extern struct UART1STR uart1;
extern struct UART2STR uart2;
#define MAX_LEN     128
typedef struct {
    uint8_t dat[MAX_LEN];
    uint8_t front;
    uint8_t rear;
}Que ;
void QueInit(Que *Que);
int Que_isFull(Que *Que);
int Que_isEmpty(Que *Que);
int Que_Enqueue(Que *Que , uint8_t data);
int Que_Dequeue(Que *Que ,uint8_t *data);
void Lux_Init(void);
void GetLux(uint16_t *Lux);
void right(int num);
void UART_SendData16(uint16_t Data);
void uartRead();
void ALIYUNswitch();
#ifdef __cplusplus
}
#endif

#endif // _SOURCE_H

