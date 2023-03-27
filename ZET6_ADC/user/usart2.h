#ifndef  __USART2_H
#define __USART2_H

#include "stm32f10x.h"

void USART2_Init(void); //115200
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);
void Motor_Control(uint8_t *s); //直接发字符串控制   #000P1500T1000！

#endif