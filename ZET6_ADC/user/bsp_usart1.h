#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#include<string.h>
#include "sys.h"

#define SENDBUFF_SIZE 30
void uart2_init(u32 bound);
void uart_init(u32 bound);
void USART1_Config(void);
void LED_Init(void);
void DMA_Config(void);
void motion(int a);
#endif 
