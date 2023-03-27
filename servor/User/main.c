#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
//#include "serial.h"
#include "pwm.h"
#include "motor.h"
#include "stdio.h"
#include  "ENCODER.H"
#include "PID.H"
#include "led.h"
#include "usart.h"	
int16_t speed,time=1,tar=80;
int count=0;
char buff[512]={0};
int main(void)
{
	
	//OLED_Init();
    LED_Init();
  //Serial_Init();
	  uart_init(115200);
    PWM_Init();
  //Encoder1_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  //OLED_ShowString(1, 1, "speed:");
	//printf("init\r\n");
	  Delay_ms(1000);
	  printf("AT+RST\r\n");
	  Delay_ms(1000);
//	  printf("AT+CWMODE=1\r\n");
//	  Delay_ms(1000);
	  printf("AT+CIPSNTPCFG=1,8,\"ntpl.aliyun.com\"\r\n");
	  Delay_ms(1000);Delay_ms(1000);Delay_ms(1000);
//	  printf("AT+CWJAP=\"lll\",\"12345678\"\r\n");
//	  Delay_ms(1000);Delay_ms(1000);Delay_ms(1000);Delay_ms(1000);
	  printf("AT+MQTTUSERCFG=0,1,\"NULL\",\"wechat&ibbkznChKyz\",\"c0aa726eb854171a86579e9b5b1ba02d1d781bb4a1e59e69a6201b5358ebd91c\",0,0\,\"\"\r\n");
	  Delay_ms(1000);Delay_ms(1000);
		printf("AT+MQTTCLIENTID=0,\"ibbkznChKyz.wechat|securemode=2\\,signmethod=hmacsha256\\,timestamp=1679300529935|\"\r\n");
		Delay_ms(1000);Delay_ms(1000);Delay_ms(1000);
		printf("AT+MQTTCONN=0,\"iot-06z00iqmc1xtlkp.mqtt.iothub.aliyuncs.com\",1883,1\r\n");
		Delay_ms(1000);Delay_ms(1000);Delay_ms(1000);
		printf("AT+MQTTSUB=0,\"/sys/ibbkznChKyz/wechat/thing/service/property/set\",1\r\n");
		Delay_ms(1000);Delay_ms(1000);Delay_ms(1000);
		
		sprintf(buff,"AT+MQTTPUB=0,\"/sys/ibbkznChKyz/wechat/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.service.property.set\\\"\\,\\\"id\\\":\\\"1358479105\\\"\\,\\\"params\\\":{\\\"temperature\\\":%d}\\,\\\"version\\\":\\\"1.0.0\\\"}\",1,0",count);
		printf("%s\r\n",buff);
		Delay_ms(1000);Delay_ms(1000);
	while (1)
	{
		 count++;
		 Delay_ms(1000);Delay_ms(1000);
		 if(count>45) count =1;
//	 sprintf(buff,"AT+MQTTPUB=0,\"/sys/ibbkznChKyz/wechat/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.service.property.set\\\"\\,\\\"id\\\":\\\"1358479105\\\"\\,\\\"params\\\":{\\\"temperature\\\":%d}\\,\\\"version\\\":\\\"1.0.0\\\"}\",1,0",count);
//	 printf("%s\r\n",buff);
		//sprintf(buff,"AT+MQTTPUB=0,\"/sys/ibbkznChKyz/wechat/thing/event/property/post\",\"{\"method\":\"thing.service.property.set\",\"id\":\"1358479105\",\"params\":{\"temperature\":%d},\"version\":\"1.0.0\"}\",1,0",count);
		//AT+MQTTPUB=0,"/sys/ibbkznChKyz/wechat/thing/event/property/post","{\"method\":\"thing.service.property.set\"\,\"id\":\"1358479105\"\,\"params\":{\"temperature\":35}\,\"version\":\"1.0.0\"}",1,0
//   right_f(pid1(speed,tar));
//   printf("%d\r\n",speed);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
        time++;
			  if(time>2000-(int)(angle*33/45+50))
					{
						GPIO_SetBits(GPIOB,GPIO_Pin_0);
					}
				else
				  {
						GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			  	}
				
        if(time == 2000)
        {
					  time = 1;
					//	TIM_Cmd(TIM2,DISABLE);
//					  count++;
//					  if(count == 4)
//						{
//							TIM_Cmd(TIM2,DISABLE);
//							count = 0;
//						}
					  
            //speed=Encoder1_get();
        }
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}





