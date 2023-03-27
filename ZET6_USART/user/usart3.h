#ifndef  __USART3_H
#define  __USART3_H

#include "usart.h"
#include "usart2.h"
#include "stm32f10x.h"
#define buffer2_size 10        //其大小要比数据帧长度大
extern u8 rx3buff[buffer2_size];

void USART3_Init(uint32_t bound);       
int check(u8 *buffer);
extern int dist;

#endif



