#include "timer.h"
uint32_t c=0;

/*
TIM2 CH1--PA0 CH2--PA1  CH3--PA2  CH4--PA3  
     /REMAP->1--PA15  2--PB3  3--PB10  4--PB11
TIM3 CH1--PA6 CH2--PA7  CH3--PB0  CH4--PB1  
     /REMAP->1--PC6  2--PC7  3--PC8  4--PC9
TIM4 CH1--PB6 CH2--PB7  CH3--PB8  CH4--PB9  
     /REMAP->1--PD12  2--PD13  3--PD14   4--PD15
TIM5 CH1--PA0 CH2--PA1  CH3--PA2  CH4--PA3 
*/

//float kp = 2;
//float ki = 0.2;
//float kd = 0.15;
//int target = 150;
//int err_now = 0;
//int spd_now = 0;
//int err_i = 0;
//int out= 0;
//int err_last =0;
//int err_last_last = 0;
void TIM3_INIT(void)
{
	GPIO_InitTypeDef        GPIO_TIM3;
	TIM_TimeBaseInitTypeDef TIM3_TIMEBASE;
	TIM_OCInitTypeDef       TIM3_OCINIT;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    GPIO_TIM3.GPIO_Mode =GPIO_Mode_AF_PP;
    GPIO_TIM3.GPIO_Pin =GPIO_Pin_6 |GPIO_Pin_7 ;
	  GPIO_TIM3.GPIO_Speed = GPIO_Speed_50MHz ;
	  GPIO_Init(GPIOA ,&GPIO_TIM3);
    GPIO_TIM3.GPIO_Mode =GPIO_Mode_AF_PP;
    GPIO_TIM3.GPIO_Pin =GPIO_Pin_0 |GPIO_Pin_1 ;
    GPIO_Init(GPIOB,&GPIO_TIM3);	
	
	TIM3_TIMEBASE.TIM_ClockDivision =0;
	TIM3_TIMEBASE.TIM_CounterMode =TIM_CounterMode_Up ;
	TIM3_TIMEBASE.TIM_Period = 7200-1;
	TIM3_TIMEBASE.TIM_Prescaler=0;
	TIM_TimeBaseInit(TIM3,&TIM3_TIMEBASE);
	
	TIM3_OCINIT.TIM_OCMode = TIM_OCMode_PWM1;
	TIM3_OCINIT.TIM_OCPolarity =TIM_OCPolarity_High ;
	
	TIM3_OCINIT.TIM_Pulse = 0;
	TIM_OC1PreloadConfig(TIM3 ,TIM_OCPreload_Enable);
    TIM3_OCINIT.TIM_OutputState =ENABLE ;
    TIM_OC1Init(TIM3,&TIM3_OCINIT);
	
    TIM3_OCINIT.TIM_Pulse =0 ;
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable );
    TIM3_OCINIT.TIM_OutputState =ENABLE ;
    TIM_OC2Init(TIM3,&TIM3_OCINIT);

	TIM3_OCINIT.TIM_Pulse =0;
	TIM3_OCINIT.TIM_OutputState =ENABLE ;
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC3Init(TIM3,& TIM3_OCINIT);

	TIM3_OCINIT.TIM_Pulse =0;
	TIM3_OCINIT.TIM_OutputState=ENABLE ;
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); 
	TIM_OC4Init(TIM3,& TIM3_OCINIT);
   
    TIM_Cmd(TIM3 ,ENABLE);   
}
int time_flag=0;
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		Control_task();	
		time_flag=1;
		
		}
}
//TIM4 CH1--PB6 CH2--PB7  CH3--PB8  CH4--PB9 
void TIM4_INIT(void)
{
	GPIO_InitTypeDef        GPIO_TIM4;
	TIM_TimeBaseInitTypeDef TIM4_TIMEBASE;
	TIM_OCInitTypeDef       TIM4_OCINIT;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);
	
	GPIO_TIM4.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_TIM4.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 ;
	GPIO_TIM4 .GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOB,&GPIO_TIM4);
  
	TIM4_TIMEBASE.TIM_ClockDivision = 0;
	TIM4_TIMEBASE.TIM_CounterMode = TIM_CounterMode_Up ;
	TIM4_TIMEBASE.TIM_Period = 2500-1;
	TIM4_TIMEBASE.TIM_Prescaler = 720-1;
	TIM_TimeBaseInit(TIM4 ,&TIM4_TIMEBASE);
  
  TIM4_OCINIT.TIM_OCMode = TIM_OCMode_PWM1 ;
	TIM4_OCINIT.TIM_OCPolarity = TIM_OCPolarity_High ;
	
	TIM4_OCINIT.TIM_Pulse = 2000;
	TIM4_OCINIT.TIM_OutputState = ENABLE ;
	TIM_OC1PreloadConfig(TIM4 ,TIM_OCPreload_Enable );
  TIM_OC1Init(TIM4,&TIM4_OCINIT); 
	
	TIM4_OCINIT.TIM_Pulse = 0;
	TIM4_OCINIT.TIM_OutputState = ENABLE ;
	TIM_OC2PreloadConfig(TIM4 ,TIM_OCPreload_Enable );
  TIM_OC2Init(TIM4,&TIM4_OCINIT); 
  
  TIM4_OCINIT.TIM_Pulse = 0;
	TIM4_OCINIT.TIM_OutputState = ENABLE ;
	TIM_OC3PreloadConfig(TIM4 ,TIM_OCPreload_Enable );
  TIM_OC3Init(TIM4,&TIM4_OCINIT);
	
	TIM4_OCINIT.TIM_Pulse = 0;
	TIM4_OCINIT.TIM_OutputState = ENABLE ;
	TIM_OC4PreloadConfig(TIM4 ,TIM_OCPreload_Enable );
  TIM_OC4Init(TIM4,&TIM4_OCINIT);	
	
	TIM_Cmd(TIM4 ,ENABLE ); 
	
  	
	
}
/*   ������ us
1     477
2     425
3     379
4     357
5     318
6     284
7     253
*/

void creat(int n,int times)
{
	int i=1;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
  switch(n)
	{
		
			 case 1:
		  {
				for(i=1;i<=times;i++)
		    { 
//			   GPIO_ResetBits(GPIOA,GPIO_Pin_8);
//         delay_us(1000);delay_us(911);
//         GPIO_SetBits(GPIOA,GPIO_Pin_8); 
//     	   delay_us(1000);delay_us(911);
					
					GPIO_ResetBits(GPIOA,GPIO_Pin_8);
         delay_ms(1000);
         GPIO_SetBits(GPIOA,GPIO_Pin_8); 
     	   delay_ms(1000);
			  }				 
			  break;
		  }
		  case 2:
		  {
				for(i=1;i<=times;i++)
		    {
				 GPIO_ResetBits(GPIOA,GPIO_Pin_8);
          delay_us(1000);delay_us(702);
         GPIO_SetBits(GPIOA,GPIO_Pin_8); 
     	    delay_us(1000);delay_us(702);
			  }				 
			  break;
		  }
		  case 3:
		  {
				for(i=1;i<=times;i++)
		    {
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);
         delay_us(1000);delay_us(516);
        GPIO_SetBits(GPIOA,GPIO_Pin_8); 
     	   delay_us(1000);delay_us(516);	
				}	
			  break;
		  }
		  case 4:
		  {
				for(i=1;i<=times;i++)
		    {
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);
         delay_us(1000);delay_us(431);
        GPIO_SetBits(GPIOA,GPIO_Pin_8); 
     	   delay_us(1000);delay_us(431);
        }					
			  break;
		  }
		  case 5:
		  {
				for(i=1;i<=times;i++)
		    {
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);
         delay_us(1000);delay_us(275);
        GPIO_SetBits(GPIOA,GPIO_Pin_8); 
     	   delay_us(1000);delay_us(275);
				}					
			  break;
		  }
		  case 6:
		  {
				for(i=1;i<=times;i++)
		    {
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);
         delay_us(1000);delay_us(136);
        GPIO_SetBits(GPIOA,GPIO_Pin_8); 
     	  delay_us(1000);delay_us(136);
        }					
			  break;
		  }
		  case 7:
		  {
				for(i=1;i<=times;i++)
		    {
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);
        delay_us(1000);delay_us(12);
        GPIO_SetBits(GPIOA,GPIO_Pin_8); 
     	  delay_us(1000);delay_us(12);
        }					
			  break;
		  }
		  default :
		  {
			  break;
		  }
		
		}
	
}


/*
void TIM5_Updata(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimBaseStructrue;
	NVIC_InitTypeDef        NVIC_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE );
    
  NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQn;     //����6�ж�        
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռʽ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	
	TIM_TimBaseStructrue.TIM_Period=1000-1;//��ʱ���� �����ж�ʱ�䣺1ms�ж�һ��
	TIM_TimBaseStructrue.TIM_Prescaler=72-1;
	TIM_TimBaseStructrue.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���
	TIM_TimBaseStructrue.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5,&TIM_TimBaseStructrue);
	
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); 	
	TIM_Cmd(TIM5, ENABLE);  	
}

void TIM5_IRQHandler(void) 
{
	

}
*/

/*
void Encoder_Init_TIM2(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ�ܶ�ʱ��4��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PB�˿�ʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = 0xffff; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM2,0x7fff);
  TIM_SetCounter(TIM2,0);
  TIM_Cmd(TIM2, ENABLE); 
}


void Encoder_Init_TIM4(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ�ܶ�ʱ��4��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  	
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = 0xffff; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3

  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  //Reset counter
 // TIM_SetCounter(TIM4,0x7fff);
  TIM_SetCounter(TIM4,0);
  TIM_Cmd(TIM4, ENABLE); 
}

int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 2:
	   {
		   Encoder_TIM = TIM2->CNT -0x7fff;
		   TIM2->CNT = 0x7fff;
		   break ;
	   }
	   case 4:
	   {
		   Encoder_TIM = TIM4->CNT -0x7fff;
		   TIM4->CNT = 0x7fff;
		   break ;
	   }
	   default: Encoder_TIM=0x7fff;
	 }
		return Encoder_TIM;
}
*/

/**************************************************************************
�������ܣ�TIM4�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
/*
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//����ж�
	{    		
      	
	}				   
	TIM4->SR&=~(1<<0);//����жϱ�־λ 
	
}
*/
/**************************************************************************
�������ܣ�TIM2�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
/*
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//����ж�
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	    
}

void TIM5_Updata(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimBaseStructrue;
	NVIC_InitTypeDef        NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE );
    
    NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQn;     //����6�ж�        
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռʽ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	
	TIM_TimBaseStructrue.TIM_Period=1000-1;//��ʱ���� �����ж�ʱ�䣺1ms�ж�һ��
	TIM_TimBaseStructrue.TIM_Prescaler=72-1;
	TIM_TimBaseStructrue.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���
	TIM_TimBaseStructrue.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5,&TIM_TimBaseStructrue);
	
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); 	
	TIM_Cmd(TIM5, ENABLE);  	
}

void TIM5_IRQHandler(void)								
{	
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  
	{  		
//		bianma  = Read_Encoder(2);
//		
//		spd_now = bianma ;
//		err_now = target -spd_now ;
//        out += kp*(err_now -err_last )+ki*err_now +kd*(err_now +err_last-2*err_last_last );
//		err_last_last = err_last ;
//		err_last  = err_now ;
//		if(out>200) out = out - 40;
//		TIM3->CCR1 = out *28+1268;
		
	}	   
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}
*/

/*
void TIM2_ENCODER(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM2_TIMEBASE;
	TIM_ICInitTypeDef  TIM2_ICINIT;
	GPIO_InitTypeDef TIM2_GPIO;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE );

	TIM2_GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	TIM2_GPIO.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 ;
	GPIO_Init(GPIOA ,&TIM2_GPIO);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
    TIM2_TIMEBASE.TIM_Prescaler =0;
	TIM2_TIMEBASE.TIM_Period =0xffff;
	TIM2_TIMEBASE.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM2_TIMEBASE.TIM_CounterMode = TIM_CounterMode_Up ;
	TIM_TimeBaseInit(TIM2,&TIM2_TIMEBASE);
    
    TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12 ,TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);

    TIM2_ICINIT.TIM_Channel = TIM_Channel_1  ;	
    TIM2_ICINIT.TIM_ICFilter = 0X06;
    TIM2_ICINIT.TIM_ICPolarity = TIM_ICPolarity_BothEdge ;
	TIM2_ICINIT.TIM_ICPrescaler = TIM_ICPSC_DIV1 ;
    TIM2_ICINIT.TIM_ICSelection = TIM_ICSelection_DirectTI ;
   	TIM_ICInit(TIM2 ,&TIM2_ICINIT);
	TIM_SetCounter(TIM2,0); 
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2,ENABLE ); 
}

void TIM4_ENCODER(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ�ܶ�ʱ��4��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
		
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
  TIM_TimeBaseStructure.TIM_Period = 0xffff; //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3
  
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 ;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge ;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI ;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
  TIM_ICInitStructure.TIM_ICFilter = 0x06;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  TIM_SetCounter(TIM4,0);
  TIM_Cmd(TIM4, ENABLE); 
}

//TIM3 CH1--PA6 CH2--PA7  CH3--PB0  CH4--PB1 
void TIM3_INIT(void)
{
	GPIO_InitTypeDef        GPIO_TIM3;
	TIM_TimeBaseInitTypeDef TIM3_TIMEBASE;
	TIM_OCInitTypeDef       TIM3_OCINIT;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    GPIO_TIM3.GPIO_Mode =GPIO_Mode_AF_PP;
    GPIO_TIM3.GPIO_Pin =GPIO_Pin_6 |GPIO_Pin_7 ;
	GPIO_TIM3.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA ,&GPIO_TIM3);
    GPIO_TIM3.GPIO_Mode =GPIO_Mode_AF_PP;
    GPIO_TIM3.GPIO_Pin =GPIO_Pin_0 |GPIO_Pin_1 ;
    GPIO_Init(GPIOB,&GPIO_TIM3);	
	
	TIM3_TIMEBASE.TIM_ClockDivision =0;
	TIM3_TIMEBASE.TIM_CounterMode =TIM_CounterMode_Up ;
	TIM3_TIMEBASE.TIM_Period = 250-1;
	TIM3_TIMEBASE.TIM_Prescaler=720-1;
	TIM_TimeBaseInit(TIM3,&TIM3_TIMEBASE);
	
	TIM3_OCINIT.TIM_OCMode = TIM_OCMode_PWM1;
	TIM3_OCINIT.TIM_OCPolarity =TIM_OCPolarity_High ;
	
	TIM3_OCINIT.TIM_Pulse = 125;
	TIM_OC1PreloadConfig(TIM3 ,TIM_OCPreload_Enable);
    TIM3_OCINIT.TIM_OutputState =ENABLE ;
    TIM_OC1Init(TIM3,&TIM3_OCINIT);
	
    TIM3_OCINIT.TIM_Pulse =80 ;
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable );
    TIM3_OCINIT.TIM_OutputState =ENABLE ;
    TIM_OC2Init(TIM3,&TIM3_OCINIT);

	TIM3_OCINIT.TIM_Pulse =80;
	TIM3_OCINIT.TIM_OutputState =ENABLE ;
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC3Init(TIM3,& TIM3_OCINIT);

	TIM3_OCINIT.TIM_Pulse =80;
	TIM3_OCINIT.TIM_OutputState=ENABLE ;
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); 
	TIM_OC4Init(TIM3,& TIM3_OCINIT);
   
    TIM_Cmd(TIM3 ,ENABLE );   
}

void TIM5_Updata(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimBaseStructrue;
	NVIC_InitTypeDef        NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE );
    
    NVIC_InitStructure.NVIC_IRQChannel =TIM6_IRQn;     //����6�ж�        
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռʽ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	
	TIM_TimBaseStructrue.TIM_Period=1000-1;//��ʱ���� �����ж�ʱ�䣺5ms�ж�һ��
	TIM_TimBaseStructrue.TIM_Prescaler=360-1;
	TIM_TimBaseStructrue.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���
	TIM_TimBaseStructrue.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5,&TIM_TimBaseStructrue);
	
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); 	
	TIM_Cmd(TIM5, ENABLE);  	
}

//����2���ж��Ǽ�������������ж� ���ڳ�ʼ�����������65535�� ������ÿ��5ms�Ͷ�ȡ��һ��CNT Ȼ���ֽ�������������
//�������������ж��ǲ��ᷢ���� ������ĵ����5ms���ܹ�����>=65535/4�����õ���˫���ؼ�⣩�����塣 
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&0X0001)//����ж�
	{
	}
    TIM2->SR&=~(1<<0);//����жϱ�־λ
} 

void TIM4_IRQHandler(void)
{
	if(TIM4->SR&0X0001)//����ж�
	{
	}
    TIM4->SR&=~(1<<0);//����жϱ�־λ
} 
//shortȡֵ short��(2�ֽڣ�32λϵͳ��)����ΧΪ-32768-32767
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
	     case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
//		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
		 default: Encoder_TIM=0;
	 }
		return Encoder_TIM;
}
 
void TIM5_IRQHandler(void)								//����ʱ�䲻��̫���������������˾ͻ��������
{	
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  
	{     
 	   printf("TIM2=%f\r\n",(float)Read_Encoder(2));//��ӡ�����1��·��	
	}	   
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}

*/



//int velocity(int encoder_left,int encoder_right)
//{  
//      static float Velocity,Encoder_Least,Encoder,Movement;
//	  static float Encoder_Integral;
//   //=============�ٶ�PI������=======================//	
//		Encoder_Least =(Encoder_Left+Encoder_Right)-0;                    //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
//		Encoder *= 0.8;		                                                //===һ�׵�ͨ�˲���       
//		Encoder += Encoder_Least*0.2;	                                    //===һ�׵�ͨ�˲���    
//		Encoder_Integral +=Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
//		Encoder_Integral=Encoder_Integral-Movement;                       //===����ң�������ݣ�����ǰ������
//		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===�����޷�
//		if(Encoder_Integral<-10000)  	Encoder_Integral=-10000;              //===�����޷�	
//		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;                          //===�ٶȿ���	
////		if(Turn_Off(Angle_Balance,Voltage)==1||Flag_Stop==1)   Encoder_Integral=0;      //===����رպ��������
//	    return Velocity;
//}


//void TIM5_IRQHandler(void)						
//{	
//	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  
//	{    
//       Encoder_Left = Read_Encoder(2);	
//	   printf("TIM2 = %d \r\n ",Encoder_Left) ;
//	}	   
//	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
//}
