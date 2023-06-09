#ifndef __TIMER_H
#define __TIMER_H

/*
TIM2 CH1--PA0 CH2--PA1  CH3--PA2  CH4--PA3  
     /REMAP->1--PA15  2--PB3  3--PB10  4--PB11
TIM3 CH1--PA6 CH2--PA7  CH3--PB0  CH4--PB1  
     /REMAP->1--PC6  2--PC7  3--PC8  4--PC9
TIM4 CH1--PB6 CH2--PB7  CH3--PB8  CH4--PB9  
     /REMAP->1--PD12  2--PD13  3--PD14   4--PD15
TIM5 CH1--PA0 CH2--PA1  CH3--PA2  CH4--PA3 
*/

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
extern int time_flag;

/*
void TIM2_ENCODER(void);
void TIM4_ENCODER(void);
void TIM3_INIT(void);
void TIM5_Updata(void);
int Read_Encoder(u8 TIMX);
*/
void TIM3_Int_Init(u16 arr,u16 psc);
void creat(int n,int times);
void TIM3_INIT(void);
void TIM4_INIT(void);
//void TIM4_IRQHandler(void);
//void TIM2_IRQHandler(void);
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(u8 TIMX);
void TIM5_Updata(void);


#endif



/*
void TIM2_INIT(void)
{
	GPIO_InitTypeDef        GPIO_TIM2;
	TIM_TimeBaseInitTypeDef TIM2_TIMEBASE;
	TIM_OCInitTypeDef       TIM2_OCINIT;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	GPIO_TIM2.GPIO_Mode =GPIO_Mode_AF_PP;
    GPIO_TIM2.GPIO_Pin =GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 ;
	GPIO_TIM2.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA ,&GPIO_TIM2);

	TIM2_TIMEBASE.TIM_ClockDivision =0;
	TIM2_TIMEBASE.TIM_CounterMode =TIM_CounterMode_Up ;
	TIM2_TIMEBASE.TIM_Period = 1000-1;
	TIM2_TIMEBASE.TIM_Prescaler=72-1;
	TIM_TimeBaseInit(TIM2,&TIM2_TIMEBASE);

	TIM2_OCINIT.TIM_OCMode = TIM_OCMode_PWM1;
	TIM2_OCINIT.TIM_OCPolarity =TIM_OCPolarity_High ;
	
	TIM2_OCINIT.TIM_Pulse = 800;
	TIM_OC1PreloadConfig(TIM2 ,TIM_OCPreload_Enable);
    TIM2_OCINIT.TIM_OutputState =ENABLE ;
    TIM_OC1Init(TIM2,&TIM2_OCINIT);
	
    TIM2_OCINIT.TIM_Pulse =800 ;
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable );
    TIM2_OCINIT.TIM_OutputState =ENABLE ;
    TIM_OC2Init(TIM2,&TIM2_OCINIT);

	TIM2_OCINIT.TIM_Pulse =800;
	TIM2_OCINIT.TIM_OutputState =ENABLE ;
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC3Init(TIM2,& TIM2_OCINIT);

	TIM2_OCINIT.TIM_Pulse =800;
	TIM2_OCINIT.TIM_OutputState=ENABLE ;
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable); 
	TIM_OC4Init(TIM2,& TIM2_OCINIT);
   
    TIM_Cmd(TIM2 ,ENABLE );        
}
*/

/*   ������ us
1     477
2     425
3     379
4     357
5     318
6     284
7     253
*/
