#include "usart3.h"
u8 rx3buff[buffer2_size];//���崮��3�Ľ��ջ�����
int j=0;                      //��¼����
int dist;                             
void USART3_Init(uint32_t bound)//���ڲ�����ʼ����������IO,�жϺʹ�����Ϣ
{
	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVUART_InitStructre;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART3��GPIOBʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//USART3_TX   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//USART3_RX	  
  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);     
	                            //�ж����� 
	NVUART_InitStructre.NVIC_IRQChannel = USART3_IRQn;
	NVUART_InitStructre.NVIC_IRQChannelCmd = ENABLE;
	NVUART_InitStructre.NVIC_IRQChannelPreemptionPriority = 2;
	NVUART_InitStructre.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVUART_InitStructre);
                              //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3,&USART_InitStructure);    //��ʼ������
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3,ENABLE);                   //ʹ�ܴ���
}

void USART3_IRQHandler(void)  //�����жϷ�����
{
	if(USART_GetITStatus(USART3 ,USART_IT_RXNE )==SET )//��8λ���ݵ���ʱ���ж�
	{
	  USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		rx3buff[j++] = USART_ReceiveData(USART3);        //��������ʼ��������
		if(j>=2)
		{
				if(rx3buff[0] == 0x5a && rx3buff[1] == 0x5a) //����֡ͷ
				{	 
															//���һλУ��	
//							if(check(rx3buff) == rx3buff[j-1])
//							{
//		            Usart_SendString(USART1,rx3buff); 
//                printf("ok ");								
//   						  j = 0;		
//  						  memset(rx3buff,0,buffer2_size);//���������;
//							}	
              if(j>7)
							{
								if(check(rx3buff))
							  {
								//printf("done \r\n");
								dist = rx3buff[4]<<8|rx3buff[5];
								j = 0;		
  					    memset(rx3buff,0,buffer2_size);//���������;		
							  }	
						  }			 
				}
				else
				{
					j = 0;
					memset(rx3buff,0,buffer2_size);             //���»���֡
				}
		}
		if(j>=buffer2_size)       
		{
			j = 0;
			memset(rx3buff,0,buffer2_size);
		}                        
	}
}
                        
int check(u8 *buffer)//�ۼ�У��
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


