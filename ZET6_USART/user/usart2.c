#include "usart2.h"
uint8_t rx2buff[buffer_size]; //���崮��2�Ľ��ջ�����
int i=0;                        //��¼����
                              //���ڲ�����ʼ����������IO,�жϺʹ�����Ϣ
void USART2_Init(uint32_t bound)  
{
	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVUART_InitStructre;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART2��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//ʹ��USART2��GPIOAʱ��
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIOA.9 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//USART2_RX	  GPIOA.3
  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIOA.10  
	                            //�ж����� 
	NVUART_InitStructre.NVIC_IRQChannel = USART2_IRQn;
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
  USART_Init(USART2,&USART_InitStructure);    //��ʼ������
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
  USART_Cmd(USART2,ENABLE);                   //ʹ�ܴ���
}

int CO2,CO2_filter;           //�������ݣ���ֵ����ȫ�ֱ����� 
void USART2_IRQHandler(void)  //�����жϷ�����
{
	if(USART_GetITStatus(USART2 ,USART_IT_RXNE )==SET )//��8λ���ݵ���ʱ���ж�
	{
	  USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		//printf("i = %d \r\n",i);
		rx2buff[i++] = USART_ReceiveData(USART2);        //��������ʼ��������
		if(i>=2)
		{
				if(rx2buff[0] == 0x20 && rx2buff[1] == 0x5a) //����֡ͷ
				{
					if(rx2buff[i-1] == 0x0a&&rx2buff[i-2]==0x0d) //ʶ��֡β����
					{
						                  //�������ݣ���ȡ���ֹ��˺��֮ǰ�Ľ��
              Usart_SendString(USART1,rx2buff); 						
						  i = 0;		
						  memset(rx2buff,0,buffer_size);         //���������);
					}					
				}
				else
				{
					i = 0;
					memset(rx2buff,0,buffer_size);             //���»���֡
				}
		}
		if(i>=buffer_size)      //��ӡһ֡����
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
                        
void Usart_SendByte(USART_TypeDef *pUSARTx,uint8_t ch)  //����һ���ַ� 
{
	USART_SendData(pUSARTx,ch); //����һ���ֽ����ݵ� USART
                              //�ȴ��������ݼĴ���Ϊ��
 while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}
                              
void Usart_SendString(USART_TypeDef *pUSARTx,uint8_t *str) //�����ַ��� 
{
  unsigned int k=0;
  do
		{
			Usart_SendByte( pUSARTx, *(str + k) );
			k++;
    }while(*(str + k)!='\0');
 
                              //�ȴ��������
 while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET) {}
}




/*
void USART2_IRQHandler(void)       //�����жϷ�����
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        rxbuff [i++] = USART_ReceiveData(USART1);
    }
    if(i >=  18 )
    {
        i = 0;             //���ջ�������������¿�ʼ����
       memset(rxbuff,0,18);
    }
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)        //��ǰΪ���յ�һ֡���������ݰ�
    {
        USART2->SR;  //�ȶ�SR
        USART2->DR;  //�ٶ�DR  ���֡�����жϱ�־λ
        //�˴�Ϊ���ݰ������߼�
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
		res=USART_ReceiveData(USART2);//��������
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
	  USART_ClearFlag(USART2, USART_IT_RXNE);//����жϱ�־λ
	 }
} 
*/

