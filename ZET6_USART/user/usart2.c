#include "usart2.h"
uint8_t rx2buff[buffer_size]; //定义串口2的接收缓冲区
int i=0;                        //记录数据
                              //串口参数初始函数，定义IO,中断和串口信息
void USART2_Init(uint32_t bound)  
{
	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVUART_InitStructre;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//使能USART2，GPIOA时钟
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA.9 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//USART2_RX	  GPIOA.3
  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA.10  
	                            //中断配置 
	NVUART_InitStructre.NVIC_IRQChannel = USART2_IRQn;
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
  USART_Init(USART2,&USART_InitStructure);    //初始化串口
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启串口接受中断
  USART_Cmd(USART2,ENABLE);                   //使能串口
}

int CO2,CO2_filter;           //解析数据，将值存在全局变量中 
void USART2_IRQHandler(void)  //串口中断服务函数
{
	if(USART_GetITStatus(USART2 ,USART_IT_RXNE )==SET )//有8位数据到来时进中断
	{
	  USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		//printf("i = %d \r\n",i);
		rx2buff[i++] = USART_ReceiveData(USART2);        //缓存区开始接收数据
		if(i>=2)
		{
				if(rx2buff[0] == 0x20 && rx2buff[1] == 0x5a) //接收帧头
				{
					if(rx2buff[i-1] == 0x0a&&rx2buff[i-2]==0x0d) //识别到帧尾结束
					{
						                  //解析数据，获取数字过滤后和之前的结果
              Usart_SendString(USART1,rx2buff); 						
						  i = 0;		
						  memset(rx2buff,0,buffer_size);         //将数据清除);
					}					
				}
				else
				{
					i = 0;
					memset(rx2buff,0,buffer_size);             //重新缓存帧
				}
		}
		if(i>=buffer_size)      //打印一帧数据
		{
      // Usart_SendString(USART1,rx2buff);
//	   CO2_filter = (rx2buff[3]-'0')*10000+(rx2buff[4]-'0')*1000
//						     +(rx2buff[5]-'0')*100+(rx2buff[6]-'0')*10+
//						      (rx2buff[7]-'0');			
// 		 CO2 =        (rx2buff[11]-'0')*10000+(rx2buff[12]-'0')*1000
//						     +(rx2buff[13]-'0')*100+(rx2buff[14]-'0')*10+
//						      (rx2buff[15]-'0');
			i = 0;
			memset(rx2buff,0,buffer_size);
		}                        
	}
}
                        
void Usart_SendByte(USART_TypeDef *pUSARTx,uint8_t ch)  //发送一个字符 
{
	USART_SendData(pUSARTx,ch); //发送一个字节数据到 USART
                              //等待发送数据寄存器为空
 while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}
                              
void Usart_SendString(USART_TypeDef *pUSARTx,uint8_t *str) //发送字符串 
{
  unsigned int k=0;
  do
		{
			Usart_SendByte( pUSARTx, *(str + k) );
			k++;
    }while(*(str + k)!='\0');
 
                              //等待发送完成
 while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET) {}
}




/*
void USART2_IRQHandler(void)       //串口中断服务函数
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        rxbuff [i++] = USART_ReceiveData(USART1);
    }
    if(i >=  18 )
    {
        i = 0;             //接收缓冲区溢出，重新开始接收
       memset(rxbuff,0,18);
    }
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)        //当前为接收到一帧完整的数据包
    {
        USART2->SR;  //先读SR
        USART2->DR;  //再度DR  清除帧空闲中断标志位
        //此处为数据包处理逻辑
			  for(i=0;i<18;i++)
			{
				printf("i = %d ,d = %x \r\n",i ,rxbuff[i]);
			}
        i = 0;
       memset(rxbuff,0,18);

    }
}		
*/

/*
int CO21,co21,CO2,co2;
void USART2_IRQHandler(void)               
{
	static u8 res,cnt;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
		{
		res=USART_ReceiveData(USART2);//接收数据
    if(res=='Z'){CO2=CO21;cnt=0;co2=co21;}
		if(res==' '&&cnt==0)cnt=1;
	  if(cnt>0&&cnt<0xf0&&res>='0'&&res<='9')
		{
			res=res-'0';
			if(cnt==1)CO21=res;
			else CO21=CO21*10+res;
			cnt++;
		}
    if(res=='z')cnt=0XF0;
		if(res==' '&&cnt==0XF0)cnt=0XF1;
	  if(cnt>0xf0&&res>='0'&&res<='9')
		{
			res=res-'0';
			if(cnt==0xF1)co21=res;
			else co21=co21*10+res;
			cnt++;
		}	
	  USART_ClearFlag(USART2, USART_IT_RXNE);//清除中断标志位
	 }
} 
*/

