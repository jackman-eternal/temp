#ifndef  __USART4_H
#define  __USART4_H

#include "usart.h"
#include "usart2.h"
#include "stm32f10x.h"
#define buffer3_size 8        //���СҪ������֡���ȴ�
extern uint8_t rx4buff[buffer3_size];

void USART4_Init(uint32_t bound);       
u16 CRC16(u8 *bpMsgStartAddress, u8 bDataLen);
extern float temper;

#endif
