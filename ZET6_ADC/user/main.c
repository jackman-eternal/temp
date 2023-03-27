#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "math.h"
#include "adc.h"
#include "timer.h"
#include "fft.h"
#include "stdio.h"
#include "kalman.h"
#include "kalman_filter.h"
//#include "codetab.h"
#include "OLED.h"
#include "myiic.h"
#include "usart2.h"


#define units 0.015 //y�������λ

u8 tmp_buf[20];				//�ַ�������
uint8_t x;



void OLED_display(int data[]) //��ͼ����
{

	int i;
		for(i = 1; i < FFT_N/2; i ++)
		{
			OLED_DrawFw(i,(uint8_t)data[i]);
		}
		//��ʾ��������������һ����ʾ
		newCLS();
		}


int main(void)
{	
	float k_ADC,k2_ADC;
  int i,k;
	k=0;
	kalman1_state ADC;
//	uint16_t adcx;
	int flag=0;//ѭ��������־λ
	float temp;
	int max;
	int data[64]={0};
	struct compx s[FFT_N];
	uart_init(115200);
//	USART2_Init();
	NVIC_Configuration();
	delay_init();
//  DAC_Mode_Init();  
//	kalman1_init(&ADC,0,1);
  
//	uart2_init();
	
//	Kalman_Init();
	/*
  Adc_Init();
	IIC_Init();//TIM3_INIT();
  TIM3_Int_Init(999,71);//����1ms�����Ʋ���1KHZ
  TIM4_INIT(); 
  OLED_Init();
	//show_gif();
  */
//  	OLED_CLS();
//	  OLED_ShowCN(30,16,0);
//		OLED_ShowCN(46,16,1);
//		OLED_ShowCN(62,16,2);
     /*
				 for(x=0;x<128;x=(x+1)%128) 
		{
			OLED_DrawWave(x,accur*adcx);//���Ǹ������εĺ���
			k++;
			if(k==128*5)
			{
				break;
			}
		} 
		
		OLED_CLS();
		//������
		OLED_ShowCN(24,10,4);
		OLED_ShowCN(40,10,5);
		OLED_ShowCN(56,10,6);
		OLED_ShowCN(72,10,7);
		
		TIM4->CCR1 = 0;
	  */
	while(1) 
	  {	
 
	

		
//      k_ADC = Kalman(temp);
			
			/*
    		while(flag<FFT_N&&time_flag==1)
  		 {
				 temp=adcx*(3.3/4096);
  			s[flag].real = temp;
        s[flag].imag = 0;
  			flag++;
  			time_flag=0;
  		  }
   	    if(flag>=FFT_N)
		    { 
			    flag=0;
			    FFT(s);
				
			    for(i=0;i<FFT_N;i++)
			   {
				   data[i]=sqrt(s[i].real * s[i].real + s[i].imag * s[i].imag);
	         delay_ms(10); 
	 
			    }
				 //�����ֵ
					max = data[0];
					 for(i=1;i<FFT_N/2;i++)
			   { 
					 if(max>=data[i])
					 {
						 max = data[i];
					 }
			    }
				 //�Է��Ƚ��й�һ������
			if(max >= 0)
				for(i = 0; i < FFT_N/2; i ++)
				{
					data[i] = data[i]*63/max;
				}
		    	OLED_display(data);
		    }		
       */ 
			 
	}	 

}
/////////////////////////////////////
//	  while(1)
//	{	
//	   for(x=0;x<128;x=(x+1)%128)
//		{
//		
//			OLED_DrawWave(x,accur*adcx);//���Ǹ������εĺ���
//		}
//	}
//}
//  OLED_DrawBMP(0,0,127,8,(unsigned char *)BMP1) ;
//	delay_ms(150);
//	OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP2);//��è�˿���
	//OLED_Refresh();//����OLED�Դ�
//  for(k=63;k<126;k++)
//	{
//		for(i=63;i>=0;i--)
//		{
//			OLED_DrawPoint(k,i);
//      OLED_Refresh();			
//		}
//		
//	}

//	while(1) 
//	  {	
//			adcx=Get_Adc_Average(ADC_Channel_1,5);
//			temp=adcx*(3.3/4096);
//		//  k_ADC = Kalman(temp);
//	//	  k2_ADC = kalman1_filter(&ADC,temp);
//      delay_ms(1000); 		
//			
//   //   printf(" %.2f \t  %.2f \r\n",temp,k_ADC); 		  
//	//	  sprintf((char *)tmp_buf,"V:%.2f ",temp);//�ַ�����ʽ������
//	//	  OLED_ShowString(20,10,(u8 *)tmp_buf ,24); 
//		//	sprintf((char *)kad_buf,"KV:%.2f ",k_ADC);
//		//	OLED_ShowString(20,40,(u8 *)kad_buf ,24); 
//		 // OLED_Refresh();
//			temp*=10;
//		while(flag<128&&time_flag==1)
//		{
//			s[flag].real = temp;
//      s[flag].imag = 0;
//			flag++;
//			time_flag=0;
//		}
//		if(flag>=128)
//		{
//			flag=0;
//			FFT(s);
//			for(i=0;i<64;i++)
//			{
//				data[i]=sqrt(s[i].real * s[i].real + s[i].imag * s[i].imag);
//			}
//			OLED_display(data);
//		}

//	}	 
//}


//void TIM3_IRQHandler(void)   //TIM3�ж�
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
//		{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
//		time_flag=1;
//		}
//}

/*
//void OLED_display(int data[]) //��ͼ����
//{
//	int x,y;
//	OLED_CLS();//����


//	for(x=0;x<128;x+=2)
//	{
//		for(y=0;y<data[x/2]*units;y++)
//		{
//			OLED_DrawPoint(128-x,y);
//			OLED_DrawPoint(128-x+1,y);
//		//	  OLED_DrawFw(x,y);
////			OLED_DrawPoint(x,y);
////			OLED_DrawPoint(x+1,y);
//			OLED_Refresh();//������ʾ
//			
//		}
//	  }
//}




*/



