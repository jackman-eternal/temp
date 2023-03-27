#include "usart.h"	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       

//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
//如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	
//传输内容帧头 ：!+数据(三位)+#  ！360 #
//初始化IO 串口1 
//bound:波特率
int angle=135;
void uart_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟

	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);            //初始化PA9

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//复用功能
	GPIO_Init(GPIOA,&GPIO_InitStructure); //PA10
	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#endif
	USART_Cmd(USART1, ENABLE);  //使能串口1 
}

 void USART_Send_BYTE(USART_TypeDef *pUSART,u8 data)
{
	USART_SendData(pUSART,data);
	while(USART_GetFlagStatus(pUSART,USART_FLAG_TXE)==RESET);
}
void USART_Send_String(USART_TypeDef *pUSART,u8 *str)
{
	u8 k = 0;
	do
	{
		USART_Send_BYTE(pUSART,*(str+k));
		k++;
	}while(*(str+k)!='\0');
	while(USART_GetFlagStatus(pUSART,USART_FLAG_TC)==RESET){}
}
//=======================================
//串口1中断服务程序
//=======================================
#if MY_WAY
//自己采用的数据接收方式
u8 cnt=0;
u8 RX1Buff[buff_size]={0};
void USART1_IRQHandler(void) 
{
	u8 data;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		data = USART_ReceiveData(USART1);
		RX1Buff[cnt++] = data;
	}
		if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//空闲中断
	{
		data = USART1->SR;
		data = USART1->DR;
		cnt = 0;
		if(RX1Buff[0] == '!'&&RX1Buff[4] == '#')
		{
			//已经识别到帧头
		  //USART_Send_String(USART1,RX1Buff);
			if((RX1Buff[1]<='9'&&RX1Buff[1]>='0')&&
			   (RX1Buff[2]<='9'&&RX1Buff[2]>='0')&&
			   (RX1Buff[3]<='9'&&RX1Buff[3]>='0') )
			{
				angle = (RX1Buff[1]-'0')*100+
				        (RX1Buff[2]-'0')*10+
								(RX1Buff[3]-'0');
				printf("angle = %d \r\n",angle);
				Delay_us(10);
				TIM_Cmd(TIM2,ENABLE);
		  }
		}
		memset(RX1Buff,0,buff_size);
	}
}	
#else
u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
u16 USART_RX_STA;         		//接收状态标记	

void USART1_IRQHandler(void)
{
	u8 res=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{
		res = USART_ReceiveData(USART1);
		if(!(USART_RX_STA&0x8000))//数据接收未完成
		{
			USART_RX_BUF[USART_RX_STA++]=res;
			if(res==0x0d)
			{
				USART_RX_STA|=0x4000; //识别到结尾的第一个字符
			}
			if((USART_RX_STA&0x4000) && (res==0x0a))
			{
				USART_RX_STA|=0x8000;
			}
		}
		else
		{
			//USART_Send_String(USART1,USART_RX_BUF);
			USART_RX_STA = 0;
		}
	}
}
//uint8_t Recv1[128]={0};//串口接收缓存
//u8 rx_cnt=0;//接收数据个数计数变量

//int sizecopy=128;

//void USART1_IRQHandler(void)                	
//{
//	uint8_t data;//接收数据暂存变量
//	uint8_t bufcopy[128];//最多只取前64个数据

//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
//	{
//		data = USART_ReceiveData(USART1);   			
//		Recv1[rx_cnt++]=data;//接收的数据存入接收数组 
//		//protocol_data_recv(&data, 1);
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//	} 
//	
//	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//空闲中断
//	{
//		data = USART1->SR;//串口空闲中断的中断标志只能通过先读SR寄存器，再读DR寄存器清除！
//		data = USART1->DR;
//		
//		//清空本地接收数组
//		memset(bufcopy,0,sizecopy);
//		
//		memcpy(bufcopy,Recv1,rx_cnt);//有几个复制几个
//		
//	//	protocol_data_recv(bufcopy, rx_cnt);

//		memset(Recv1,0,sizecopy);
//		rx_cnt=0;
//	}
//} 
#endif



 



