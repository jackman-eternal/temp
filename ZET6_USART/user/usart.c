#include "usart.h"	  

void USART1_Init(u32 bound)
{                       
	GPIO_InitTypeDef  GPIO_InitStructure; //配置串口1GPIO引脚
	USART_InitTypeDef USART_InitStructure;//配置串口参数
	NVIC_InitTypeDef  NVIC_InitStructure; //配置串口中断
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);//使能USART1，GPIOA时钟
                              
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);          //初始化参数
                                           
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);            //初始化参数 

	NVIC_InitStructure.NVIC_IRQChannel  = USART1_IRQn;//配置中断源
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;  //使能中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0; //响应优先级
	NVIC_Init(&NVIC_InitStructure);                   //初始化参数
	
	USART_InitStructure.USART_BaudRate = bound;       //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    
	USART_ITConfig(USART1 ,USART_IT_RXNE,DISABLE );   //配置中断的事件 
  USART_Init(USART1, &USART_InitStructure);         //初始化串口
  USART_Cmd(USART1, ENABLE);                        //使能串口
}

//重定义可以用printf发送指定格式的字符串
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}	
   
    return ch;
} 
int getc(FILE *f)
{
	/* 等待串口输入数据 */
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
	/* 返回值进行强制类型转换 */
	return (int)USART_ReceiveData(USART1);
}

/*
未使用串口的中断，如果要使用串口1的中断，并取消注释此代码
修改 USART_ITConfig(USART1 ,USART_IT_RXNE,ENABLE);
void USART1_IRQHandler(void )
{
	uint8_t temp;
	if(USART_GetITStatus(USART1 ,USART_IT_RXNE )==SET  )
	{
		temp = USART_ReceiveData(USART1 );
	}
    USART_ClearITPendingBit(USART1 ,USART_IT_RXNE ); 		
}
*/


