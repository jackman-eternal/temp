#ifndef  __USART2_H
#define  __USART2_H

#include "usart.h"
#include "stm32f10x.h"
#define buffer_size 18        //���СҪ������֡���ȴ�Co2ģ�鷢��һ֡���ݴ�СΪ18
extern uint8_t rx2buff[buffer_size];
extern int CO2,CO2_filter,CO2,co2;
void USART2_Init(uint32_t bound);       
void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);



#endif

