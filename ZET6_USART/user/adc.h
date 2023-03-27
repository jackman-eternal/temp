#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
//#include "timer.h"
extern int32_t adcx;
extern u32 tick_ms;
extern u8 flag;
#define accur 0.015295   //accur=18*3.3/4096��3.3/4096����ADC�������ȣ�18��Ϊ���ò���ת��һ���ܹ���ʾ���ʵ�λ�ӣ�
//extern   __IO uint32_t ADC_ConValue;
//extern   __IO uint16_t ADC_Value;  //ת���ĵ�ѹֵͨ��DMA����SRAM
extern   __IO uint16_t ADC_Value[2]; //��ͨ��ģʽ����sram�ж�����������
u16 Get_Adc_Average(u8 ch,u8 times);
u16 Get_Adc(u8 ch) ;
void Adc_Init(void);
void ADC1_Init(void);        //��ͨ��
void ADC1_Multi_Init(void);  //��ͨ��
void ADC12_Reg_Init(void);   //˫��ADC
void Control_task(void);
#endif

//void conver(void);
/*
void conver(void)
{
	 ADC_PH_Temp1 = (float)ADC_Value[0]*3.3/4096;
	 ADC_TU_Temp2 = (float)ADC_Value[1]*3.3/4096;
	 PH = -5.7541*ADC_PH_Temp1+16.654;
	if(PH<=0.0)
      {
		  PH=0.0;
	  }
	if(PH>=14.0)
      {
	      PH=14.0;
	  }
	   TU=-865.68*ADC_TU_Temp2+3291.3;
	  if(TU<=0)
      {
	      TU=0;
	  }
	  if(TU>=3000)
      {
		  TU=3000;
	  }
	  
}
*/

