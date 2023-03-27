#include "usart.h"	  

void USART1_Init(u32 bound)
{                       
	GPIO_InitTypeDef  GPIO_InitStructure; //���ô���1GPIO����
	USART_InitTypeDef USART_InitStructure;//���ô��ڲ���
	NVIC_InitTypeDef  NVIC_InitStructure; //���ô����ж�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);//ʹ��USART1��GPIOAʱ��
                              
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);          //��ʼ������
                                           
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);            //��ʼ������ 

	NVIC_InitStructure.NVIC_IRQChannel  = USART1_IRQn;//�����ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;  //ʹ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0; //��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);                   //��ʼ������
	
	USART_InitStructure.USART_BaudRate = bound;       //���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    
	USART_ITConfig(USART1 ,USART_IT_RXNE,DISABLE );   //�����жϵ��¼� 
  USART_Init(USART1, &USART_InitStructure);         //��ʼ������
  USART_Cmd(USART1, ENABLE);                        //ʹ�ܴ���
}

//�ض��������printf����ָ����ʽ���ַ���
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}	
   
    return ch;
} 
int getc(FILE *f)
{
	/* �ȴ������������� */
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
	/* ����ֵ����ǿ������ת�� */
	return (int)USART_ReceiveData(USART1);
}

/*
δʹ�ô��ڵ��жϣ����Ҫʹ�ô���1���жϣ���ȡ��ע�ʹ˴���
�޸� USART_ITConfig(USART1 ,USART_IT_RXNE,ENABLE);
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


