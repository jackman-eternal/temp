#include "stm32f10x.h"
#include "usart.h"            //串口1头文件，定义串口的初始化函数
#include "usart2.h"           //串口2头文件，定义串口的初始化函数，中断服务函数，声明接收缓存区
#include "usart3.h"           //串口3头文件，定义串口的初始化函数，中断服务函数，声明接收缓存区
#include "usart4.h"
#include "delay.h"            //延时函数定义
#include "sys.h"              //IO口的重定义文件
#include "kalman.h"           //一阶卡尔曼滤波器
/*
开发板：STM32F103ZET6
详细数据帧见数据手册
CO2 一帧数据帧：  (帧头)20 5A 20 30 30 31 35 30 20 7A 20 30 30 31 35 38 0D 0A(帧尾) 
测试传感器数据帧：(帧头)5A 5A 15 03 00 13 02 E1 (校验) 
温度传感器数据帧：(帧头)01 03 02 00 DA 39 DF (crc校验)
             TX     RX
		USART1   PA9    PA10      其他串口接收数据通过串口1发送
		USART2   PA2    PA3       连接CO2
		USART3   PB10   PB11      连接测距传感器
		USART4   PC10   PC11			连接温度传感器
*/
int main(void)
{
	delay_init();               //延时初始化
	NVIC_Configuration();       //中断优先级分组配置
	USART1_Init(9600);          //串口1初始化
//USART2_Init(9600);          //串口2初始化，中断服务函数中处理数据
//USART3_Init(9600);          //串口3初始化
//USART4_Init(9600);          //串口4初始化
	printf("initalize \r\n");   //测试串口发送
	while(1)
	{
	  delay_ms(1000); 
//	printf("Z %d z %d \r\n",CO2_filter,CO2);//查询数据
//		printf("dist %d \r\n",dist);
	}
}


