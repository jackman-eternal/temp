#include "usart4.h"

uint8_t rx4buff[buffer3_size];//���崮��4�Ľ��ջ�����
int k=0;                      //��¼����
float temper;
                              //���ڲ�����ʼ����������IO,�жϺʹ�����Ϣ
void USART4_Init(uint32_t bound)  
{ 
	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVUART_InitStructre;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��GPIOCʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);	//ʹ��UART4ʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//USART4_TX   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//USART4_RX	  
  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);     
	                            //�ж����� 
	NVUART_InitStructre.NVIC_IRQChannel = UART4_IRQn;
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
  USART_Init(UART4,&USART_InitStructure);    //��ʼ������
  USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
  USART_Cmd(UART4,ENABLE);                   //ʹ�ܴ���
}
int c;
void UART4_IRQHandler(void)  //�����жϷ�����
{
	u16 CRC_Code=0;
	if(USART_GetITStatus(UART4,USART_IT_RXNE )==SET )//��8λ���ݵ���ʱ���ж�
	{
	  USART_ClearITPendingBit(UART4,USART_IT_RXNE);
		rx4buff[k++] = USART_ReceiveData(UART4);        //��������ʼ��������
		if(k>=2)
		{
				if(rx4buff[0]==0x01&&rx4buff[1]==0x03) //����֡ͷ
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
					memset(rx4buff,0,buffer3_size);//���»���֡
				}
		}
		if(k>=buffer3_size)       //��ӡһ֡����
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


