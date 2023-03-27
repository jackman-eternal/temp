#include "usart3.h"
u8 rx3buff[buffer2_size];//定义串口3的接收缓冲区
int j=0;                      //记录数据
int dist;                             
void USART3_Init(uint32_t bound)//串口参数初始函数，定义IO,中断和串口信息
{
	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVUART_InitStructre;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART3，GPIOB时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//USART3_TX   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//USART3_RX	  
  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);     
	                            //中断配置 
	NVUART_InitStructre.NVIC_IRQChannel = USART3_IRQn;
	NVUART_InitStructre.NVIC_IRQChannelCmd = ENABLE;
	NVUART_InitStructre.NVIC_IRQChannelPreemptionPriority = 2;
	NVUART_InitStructre.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVUART_InitStructre);
                              //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3,&USART_InitStructure);    //初始化串口
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//开启串口接受中断
  USART_Cmd(USART3,ENABLE);                   //使能串口
}

void USART3_IRQHandler(void)  //串口中断服务函数
{
	if(USART_GetITStatus(USART3 ,USART_IT_RXNE )==SET )//有8位数据到来时进中断
	{
	  USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		rx3buff[j++] = USART_ReceiveData(USART3);        //缓存区开始接收数据
		if(j>=2)
		{
				if(rx3buff[0] == 0x5a && rx3buff[1] == 0x5a) //接收帧头
				{	 
															//最后一位校验	
//							if(check(rx3buff) == rx3buff[j-1])
//							{
//		            Usart_SendString(USART1,rx3buff); 
//                printf("ok ");								
//   						  j = 0;		
//  						  memset(rx3buff,0,buffer2_size);//将数据清除;
//							}	
              if(j>7)
							{
								if(check(rx3buff))
							  {
								//printf("done \r\n");
								dist = rx3buff[4]<<8|rx3buff[5];
								j = 0;		
  					    memset(rx3buff,0,buffer2_size);//将数据清除;		
							  }	
						  }			 
				}
				else
				{
					j = 0;
					memset(rx3buff,0,buffer2_size);             //重新缓存帧
				}
		}
		if(j>=buffer2_size)       
		{
			j = 0;
			memset(rx3buff,0,buffer2_size);
		}                        
	}
}
                        
int check(u8 *buffer)//累加校验
{
	int i;
	u8 sum=0;
	for(i=0;i<=6;i++)
	{
		sum += buffer[i];
	}
	if(sum == buffer[7])
	{
		return 1;
	}
	return 0;
}


