#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include<stdio.h>

void uart2_init(void);
void LED_Init(void); 
void motion(int a)	;  	
void uart_init(u32 bound);
//void DMA_Config(void);
//extern uint8_t RxBuffer[8];



#endif


