#include "usart.h"	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
//���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	
//��������֡ͷ ��!+����(��λ)+#  ��360 #
//��ʼ��IO ����1 
//bound:������
int angle=135;
void uart_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��

	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);            //��ʼ��PA9

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//���ù���
	GPIO_Init(GPIOA,&GPIO_InitStructure); //PA10
	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
#endif
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
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
//����1�жϷ������
//=======================================
#if MY_WAY
//�Լ����õ����ݽ��շ�ʽ
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
		if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//�����ж�
	{
		data = USART1->SR;
		data = USART1->DR;
		cnt = 0;
		if(RX1Buff[0] == '!'&&RX1Buff[4] == '#')
		{
			//�Ѿ�ʶ��֡ͷ
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
u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
u16 USART_RX_STA;         		//����״̬���	

void USART1_IRQHandler(void)
{
	u8 res=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{
		res = USART_ReceiveData(USART1);
		if(!(USART_RX_STA&0x8000))//���ݽ���δ���
		{
			USART_RX_BUF[USART_RX_STA++]=res;
			if(res==0x0d)
			{
				USART_RX_STA|=0x4000; //ʶ�𵽽�β�ĵ�һ���ַ�
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
//uint8_t Recv1[128]={0};//���ڽ��ջ���
//u8 rx_cnt=0;//�������ݸ�����������

//int sizecopy=128;

//void USART1_IRQHandler(void)                	
//{
//	uint8_t data;//���������ݴ����
//	uint8_t bufcopy[128];//���ֻȡǰ64������

//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
//	{
//		data = USART_ReceiveData(USART1);   			
//		Recv1[rx_cnt++]=data;//���յ����ݴ���������� 
//		//protocol_data_recv(&data, 1);
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//	} 
//	
//	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//�����ж�
//	{
//		data = USART1->SR;//���ڿ����жϵ��жϱ�־ֻ��ͨ���ȶ�SR�Ĵ������ٶ�DR�Ĵ��������
//		data = USART1->DR;
//		
//		//��ձ��ؽ�������
//		memset(bufcopy,0,sizecopy);
//		
//		memcpy(bufcopy,Recv1,rx_cnt);//�м������Ƽ���
//		
//	//	protocol_data_recv(bufcopy, rx_cnt);

//		memset(Recv1,0,sizecopy);
//		rx_cnt=0;
//	}
//} 
#endif



 



