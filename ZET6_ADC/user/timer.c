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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
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
/*   半周期 us
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //时钟使能
	
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //时钟使能
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE );
    
  NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQn;     //串口6中断        
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占式优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//中断使能
	NVIC_Init(&NVIC_InitStructure);//中断初始化
	
	TIM_TimBaseStructrue.TIM_Period=1000-1;//暂时设置 计数中断时间：1ms中断一次
	TIM_TimBaseStructrue.TIM_Prescaler=72-1;
	TIM_TimBaseStructrue.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数
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
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器4的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PB端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = 0xffff; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//使用编码器模式3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
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
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  	
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = 0xffff; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//使用编码器模式3

  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
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
函数功能：TIM4中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
/*
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//溢出中断
	{    		
      	
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 
	
}
*/
/**************************************************************************
函数功能：TIM2中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
/*
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}

void TIM5_Updata(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimBaseStructrue;
	NVIC_InitTypeDef        NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE );
    
    NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQn;     //串口6中断        
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占式优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//中断使能
	NVIC_Init(&NVIC_InitStructure);//中断初始化
	
	TIM_TimBaseStructrue.TIM_Period=1000-1;//暂时设置 计数中断时间：1ms中断一次
	TIM_TimBaseStructrue.TIM_Prescaler=72-1;
	TIM_TimBaseStructrue.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
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
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
		
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = 0xffff; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);//使用编码器模式3
  
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 ;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge ;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI ;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
  TIM_ICInitStructure.TIM_ICFilter = 0x06;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
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
    
    NVIC_InitStructure.NVIC_IRQChannel =TIM6_IRQn;     //串口6中断        
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占式优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//中断使能
	NVIC_Init(&NVIC_InitStructure);//中断初始化
	
	TIM_TimBaseStructrue.TIM_Period=1000-1;//暂时设置 计数中断时间：5ms中断一次
	TIM_TimBaseStructrue.TIM_Prescaler=360-1;
	TIM_TimBaseStructrue.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数
	TIM_TimBaseStructrue.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5,&TIM_TimBaseStructrue);
	
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); 	
	TIM_Cmd(TIM5, ENABLE);  	
}

//以下2个中断是计数器溢出更新中断 我在初始化里面给的是65535次 但是我每隔5ms就读取了一次CNT 然后又将计数器清零了
//所以下面两个中断是不会发生的 除非你的电机在5ms内能够产生>=65535/4（我用的是双边沿检测）个脉冲。 
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&0X0001)//溢出中断
	{
	}
    TIM2->SR&=~(1<<0);//清除中断标志位
} 

void TIM4_IRQHandler(void)
{
	if(TIM4->SR&0X0001)//溢出中断
	{
	}
    TIM4->SR&=~(1<<0);//清除中断标志位
} 
//short取值 short型(2字节，32位系统下)，范围为-32768-32767
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
 
void TIM5_IRQHandler(void)								//定是时间不能太长否则计数器溢出了就会测量错误
{	
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  
	{     
 	   printf("TIM2=%f\r\n",(float)Read_Encoder(2));//打印出电机1的路程	
	}	   
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}

*/



//int velocity(int encoder_left,int encoder_right)
//{  
//      static float Velocity,Encoder_Least,Encoder,Movement;
//	  static float Encoder_Integral;
//   //=============速度PI控制器=======================//	
//		Encoder_Least =(Encoder_Left+Encoder_Right)-0;                    //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
//		Encoder *= 0.8;		                                                //===一阶低通滤波器       
//		Encoder += Encoder_Least*0.2;	                                    //===一阶低通滤波器    
//		Encoder_Integral +=Encoder;                                       //===积分出位移 积分时间：10ms
//		Encoder_Integral=Encoder_Integral-Movement;                       //===接收遥控器数据，控制前进后退
//		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===积分限幅
//		if(Encoder_Integral<-10000)  	Encoder_Integral=-10000;              //===积分限幅	
//		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;                          //===速度控制	
////		if(Turn_Off(Angle_Balance,Voltage)==1||Flag_Stop==1)   Encoder_Integral=0;      //===电机关闭后清除积分
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
