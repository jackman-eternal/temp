#include "usart.h"	  

void uart2_init(void)
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

	USART_InitStructure.USART_BaudRate = 9600;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2,&USART_InitStructure); //��ʼ������
//    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
    USART_Cmd(USART2,ENABLE);                    //ʹ�ܴ���
}
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}	
   
    return ch;
}  
void uart_init(u32 bound)
{
  //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
    //USART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
  
	NVIC_InitStructure.NVIC_IRQChannel  = USART1_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_Init(&NVIC_InitStructure);
	
    //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    
	USART_ITConfig(USART1 ,USART_IT_RXNE,ENABLE ); 
    USART_Init(USART1, &USART_InitStructure);     //��ʼ������1
//	USART_DMACmd(USART1 ,USART_DMAReq_Rx ,DISABLE ); 
	
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

void LED_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOC ,ENABLE );
   
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_8 |GPIO_Pin_11|GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
    GPIO_Init(GPIOA ,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOA , GPIO_Pin_8 |GPIO_Pin_11) ;
	GPIO_SetBits(GPIOA ,GPIO_Pin_13 ); 
	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
    GPIO_Init(GPIOC ,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOC , GPIO_Pin_8 |GPIO_Pin_7) ;
	
	
     	
}


void motion(int a)
{

	if(a == 1)  //zhi
	{
	 PAout(8) = 1; PAout(11) =1;  	
     TIM3->CCR1 = 3600;
	 TIM3->CCR2 = 3600;	
	}
	if(a == 2)  //��
	{
     PAout(8) = 1; PAout(11) =0;
	 TIM3->CCR1 = 2800;
	 TIM3->CCR2 = 0;
		
	}
	if(a==3)  //ͣ
	{	
	 PAout(8) = 0; PAout(11) =0;	
     TIM3->CCR1 = 0;
	 TIM3->CCR2 = 0;
//	 TIM3->CCR3 = 0; 
	}
	if(a==4)    //��
	{
	 PAout(8) = 0; PAout(11) =1;
     TIM3->CCR1 = 0;
	 TIM3->CCR2 = 2800;
	}
	if(a==5) //��
	{
	 PAout(8) = 0; PAout(11) =0;	
     TIM3->CCR1 = 4200;
	 TIM3->CCR2 = 4200;	
	}
	if(a == 6)
	{
		PCout(7) = 0; PCout(8) = 1; 
		TIM3->CCR3 = 3000;	
		delay_ms(1000);
        delay_ms(1000);
        delay_ms(1000);
        delay_ms(1000);
        TIM3->CCR3 = 0;			
	}
	if(a == 7)
	{
		PCout(7) = 1; PCout(8) = 0; 
		TIM3->CCR3 = 3000;	
		delay_ms(1000);
        delay_ms(1000);
        delay_ms(1000);
        delay_ms(1000);
		TIM3->CCR3 = 0;	
	}
	
	
	
 }
void USART1_IRQHandler(void )
{
	uint8_t temp;
	if(USART_GetITStatus(USART1 ,USART_IT_RXNE )==SET  )
	{
		temp = USART_ReceiveData(USART1 );
		switch(temp )
		{
			case '1' :
			{
			//	motion(1);  //	ǰ
				TIM_SetCompare2(TIM3,7200); //pa 6
        TIM_SetCompare1(TIM3,0);    //pa 7				
				break ;
			}
			case  '2' :
			{
				//motion(3);   //ͣ
				TIM_SetCompare2(TIM3,0); //pa 6
        TIM_SetCompare1(TIM3,0);    //pa 7	
				break ; 
			}
			case '4' :       //��
			{
			//	motion(4);
				TIM_SetCompare3(TIM3,7200); //���ش�
				break ;
			}
			case '5' :       //��
			{
			//	motion(2);
				TIM_SetCompare3(TIM3,0);  //���عر� 
				break ;
			}
			case '3' :      //��
			{
					
				TIM_SetCompare1(TIM3,7200); //pa 6
        TIM_SetCompare2(TIM3,0);    //pa 7	
			//	motion(5);
				break ;
			}
			case '7' :
			{
				motion(6);  //��
				break ;
			}
			case '8' :      //��
			{
				motion(7);
				break ;
			}
			default :
			{
				break ;
			}
		}
        USART_ClearITPendingBit(USART1 ,USART_IT_RXNE ); 		
	}
}


/*
void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); //����DMAʱ��
	
	DMA_InitStructure.DMA_BufferSize = 8;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC   ;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte ;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable ;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal   ;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR); 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte ;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable ;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High ;
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  =2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority  =2;
	NVIC_Init(&NVIC_InitStructure) ;	
	
	DMA_Init(DMA1_Channel5 ,&DMA_InitStructure); 
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,DISABLE ); 
	DMA_Cmd(DMA1_Channel5,ENABLE);
}
*/


//int getc(FILE *f)
//{
//	/* �ȴ������������� */
//	while(USART_GetFlagStatus(USART2,USART_FLAG_RXNE)==RESET);
//	/* ����ֵ����ǿ������ת�� */
//	return (int)USART_ReceiveData(USART1);
//}

// uint8_t RxBuffer[8] = {0};  DMA����
// for(i=0;i<8;i++)
//	   {		
//		 printf("RxBuffer[%d] = %d \r\n",i,RxBuffer[i]);		 			 
//	   } 

//void DMA1_Channel5_IRQHandler(void)
//{
//	u8 i=0;
//	if(DMA_GetFlagStatus(DMA1_FLAG_TC4 )==SET )
//	{
//		for(i=0;i<8;i++)
//	   {
//		 if(RxBuffer[i]!=0)  
//		 {
//		 printf("RxBuffer[%d] = %d \r\n",i,RxBuffer[i]);
//		 }			 
//	   }
//	   DMA_ClearFlag(DMA1_FLAG_TC4); 	
//	}		
//}



