#include "usart4.h"

uint8_t rx4buff[buffer3_size];//定义串口4的接收缓冲区
int k=0;                      //记录数据
float temper;
                              //串口参数初始函数，定义IO,中断和串口信息
void USART4_Init(uint32_t bound)  
{ 
	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVUART_InitStructre;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能GPIOC时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);	//使能UART4时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//USART4_TX   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//USART4_RX	  
  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);     
	                            //中断配置 
	NVUART_InitStructre.NVIC_IRQChannel = UART4_IRQn;
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
  USART_Init(UART4,&USART_InitStructure);    //初始化串口
  USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);//开启串口接受中断
  USART_Cmd(UART4,ENABLE);                   //使能串口
}
int c;
void UART4_IRQHandler(void)  //串口中断服务函数
{
	u16 CRC_Code=0;
	if(USART_GetITStatus(UART4,USART_IT_RXNE )==SET )//有8位数据到来时进中断
	{
	  USART_ClearITPendingBit(UART4,USART_IT_RXNE);
		rx4buff[k++] = USART_ReceiveData(UART4);        //缓存区开始接收数据
		if(k>=2)
		{
				if(rx4buff[0]==0x01&&rx4buff[1]==0x03) //接收帧头
				{   
                  if(k>4)
									{
										 CRC_Code = CRC16(rx4buff,5);									
								   if(((u8)(CRC_Code&0xff) == rx4buff[k-2]) &&
                      ((u8)(CRC_Code>>8)&0xff) == rx4buff[k-1])
									   {
											    Usart_SendString(USART1,rx4buff);   
											    for(c=0;c<7;c++)
														{
															Usart_SendByte(USART1,rx4buff[c]);
														}											 
//											  temper = rx4buff[3]<<8|rx4buff[4];
//										    temper = temper/10;
//											  printf("temp = %f \r\n",temper);
											  k=0;
											  memset(rx4buff,0,buffer3_size); 
									   }
									}		
				}
				else
				{
					k = 0;
					memset(rx4buff,0,buffer3_size);//重新缓存帧
				}
		}
		if(k>=buffer3_size)       //打印一帧数据
		{
			k = 0;
			memset(rx4buff,0,buffer3_size);
		}                        
	}
}
                        
u16 CRC16(u8 *bpMsgStartAddress, u8 bDataLen)
{ u8 i, n;
	u16 crc = 0xffff;
	u8 u8tmp;
//	u16 u16tmp;
	for (i=0; i<bDataLen; i++)
		{ crc ^= *bpMsgStartAddress++;
		for (n=0; n<8; n++)
			{ u8tmp = crc & 1;
				crc >>= 1;
				crc &= 0x7fff;
				if (u8tmp == 1)
					{ crc ^= 0xa001; }
			}
		} 
		return crc;
}


