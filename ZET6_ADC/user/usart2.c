#include "usart2.h"
//#include "stm32f10x.h"

void USART2_Init(void) //115200
{
	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//使能USART2，GPIOA时钟
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9 
  //USART2_RX	  GPIOA.3初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2,&USART_InitStructure); //初始化串口
//    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启串口接受中断
  USART_Cmd(USART2,ENABLE);                    //使能串口
}


/***************** 发送一个字符 **********************/
 void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
 {
 /* 发送一个字节数据到 USART */
 USART_SendData(pUSARTx,ch);
 
 /* 等待发送数据寄存器为空 */
 while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
 }
 
 /***************** 发送字符串 **********************/
 void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
 {
 unsigned int k=0;
 do {
 Usart_SendByte( pUSARTx, *(str + k) );
 k++;
 } while (*(str + k)!='\0');
 
 /* 等待发送完成 */
 while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET) {
 }
 }
 //指令格式：#000P1500T1000！  调用 Motor_Control("#000P1500T1000")
 void Motor_Control(uint8_t *s)
 {
	 Usart_SendString(USART2,s);
 }
