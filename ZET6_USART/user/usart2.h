#ifndef  __USART2_H
#define  __USART2_H

#include "usart.h"
#include "stm32f10x.h"
#define buffer_size 18        //其大小要比数据帧长度大，Co2模块发送一帧数据大小为18
extern uint8_t rx2buff[buffer_size];
extern int CO2,CO2_filter,CO2,co2;
void USART2_Init(uint32_t bound);       
void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);



#endif

