#include "stm32f10x.h"
#include "usart.h"            //����1ͷ�ļ������崮�ڵĳ�ʼ������
#include "usart2.h"           //����2ͷ�ļ������崮�ڵĳ�ʼ���������жϷ��������������ջ�����
#include "usart3.h"           //����3ͷ�ļ������崮�ڵĳ�ʼ���������жϷ��������������ջ�����
#include "usart4.h"
#include "delay.h"            //��ʱ��������
#include "sys.h"              //IO�ڵ��ض����ļ�
#include "kalman.h"           //һ�׿������˲���
/*
�����壺STM32F103ZET6
��ϸ����֡�������ֲ�
CO2 һ֡����֡��  (֡ͷ)20 5A 20 30 30 31 35 30 20 7A 20 30 30 31 35 38 0D 0A(֡β) 
���Դ���������֡��(֡ͷ)5A 5A 15 03 00 13 02 E1 (У��) 
�¶ȴ���������֡��(֡ͷ)01 03 02 00 DA 39 DF (crcУ��)
             TX     RX
		USART1   PA9    PA10      �������ڽ�������ͨ������1����
		USART2   PA2    PA3       ����CO2
		USART3   PB10   PB11      ���Ӳ�ഫ����
		USART4   PC10   PC11			�����¶ȴ�����
*/
int main(void)
{
	delay_init();               //��ʱ��ʼ��
	NVIC_Configuration();       //�ж����ȼ���������
	USART1_Init(9600);          //����1��ʼ��
//USART2_Init(9600);          //����2��ʼ�����жϷ������д�������
//USART3_Init(9600);          //����3��ʼ��
//USART4_Init(9600);          //����4��ʼ��
	printf("initalize \r\n");   //���Դ��ڷ���
	while(1)
	{
	  delay_ms(1000); 
//	printf("Z %d z %d \r\n",CO2_filter,CO2);//��ѯ����
//		printf("dist %d \r\n",dist);
	}
}


