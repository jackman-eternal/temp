#ifndef __KALMAN_H
#define __KALMAN_H

#include "stm32f10x.h"
#include "usart.h"
typedef struct
{
	float Q;
	float R;
	float Pk_0;
	float Xk_0;
	float Kg;
}KALMAN;
//	float Q=0.0003;
//	float R=0.3;
//	float Pk_0 = 1;
//	float Xk_0 = 0;
//	float Kg = 0;
//Ò»Î¬¿¨¶ûÂüÂË²¨Æ÷
extern KALMAN kal_ADC;
void Kalman_Init(void);
float Kalman(float Zk);

#endif