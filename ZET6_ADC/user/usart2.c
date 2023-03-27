#include "usart2.h"
//#include "stm32f10x.h"

void USART2_Init(void) //115200
{
	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART2��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//ʹ��USART2��GPIOAʱ��
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9 
  //USART2_RX	  GPIOA.3��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2,&USART_InitStructure); //��ʼ������
//    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
  USART_Cmd(USART2,ENABLE);                    //ʹ�ܴ���
}


/***************** ����һ���ַ� **********************/
 void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
 {
 /* ����һ���ֽ����ݵ� USART */
 USART_SendData(pUSARTx,ch);
 
 /* �ȴ��������ݼĴ���Ϊ�� */
 while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
 }
 
 /***************** �����ַ��� **********************/
 void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
 {
 unsigned int k=0;
 do {
 Usart_SendByte( pUSARTx, *(str + k) );
 k++;
 } while (*(str + k)!='\0');
 
 /* �ȴ�������� */
 while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET) {
 }
 }
 //ָ���ʽ��#000P1500T1000��  ���� Motor_Control("#000P1500T1000")
 void Motor_Control(uint8_t *s)
 {
	 Usart_SendString(USART2,s);
 }
