#include "adc.h"

 int32_t adcx=0;
 u32 tick_ms=0;
 u8 flag=0;
// __IO uint16_t ADC_Value;  //��ͨ��ģʽ��ADC���ݼĴ���ʹ�õ�16λ
// __IO uint16_t ADC_Value[2]; //��ͨ��ģʽ����sram�ж�����������
//__IO uint32_t ADC_ConValue;
//��ͨ�������ȡ
/*
void ADC1_Init(void)
{
	GPIO_InitTypeDef ADC1_GPIO;
	ADC_InitTypeDef  ADC1_Config;
	DMA_InitTypeDef  ADC1_DMA;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |RCC_APB2Periph_GPIOA ,ENABLE );
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	ADC1_GPIO.GPIO_Mode = GPIO_Mode_AIN;
    ADC1_GPIO.GPIO_Pin = GPIO_Pin_1 ;
    GPIO_Init(GPIOA,&ADC1_GPIO);
		
	ADC1_DMA.DMA_BufferSize = 1;  
	ADC1_DMA.DMA_DIR = DMA_DIR_PeripheralSRC ; //����Ϊ����Դ
	ADC1_DMA.DMA_M2M = DMA_M2M_Disable;  
    ADC1_DMA.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value;
	ADC1_DMA.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord ;//16λ
	ADC1_DMA.DMA_MemoryInc  = DMA_MemoryInc_Disable ;    //�ڴ��ַ�̶�����ַֻ��һ����
	ADC1_DMA.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR)); 
	ADC1_DMA.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    ADC1_DMA.DMA_PeripheralInc = DMA_PeripheralInc_Disable ;  //�����ַ�̶�����ַֻ��һ����
    ADC1_DMA.DMA_Priority = DMA_Priority_High ; 
    ADC1_DMA.DMA_Mode = DMA_Mode_Circular ;//ѭ������	
	DMA_Init(DMA1_Channel1 ,&ADC1_DMA);
    DMA_Cmd(DMA1_Channel1,ENABLE);
	
	ADC1_Config.ADC_Mode = ADC_Mode_Independent ;
	ADC1_Config.ADC_ScanConvMode =DISABLE ;
	ADC1_Config.ADC_ContinuousConvMode  =ENABLE ;
	ADC1_Config.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ;
	ADC1_Config.ADC_DataAlign = ADC_DataAlign_Right ;
	ADC1_Config.ADC_NbrOfChannel =1;
	ADC_Init(ADC1 ,&ADC1_Config);
  
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    ADC_RegularChannelConfig(ADC1,ADC_Channel_1 ,1,ADC_SampleTime_55Cycles5  );//55.5����������
    ADC_DMACmd(ADC1 ,ENABLE );    //ADC�ύDMA����
    ADC_Cmd(ADC1 ,ENABLE );    	
     	 
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1,ENABLE); 	
}
*/
/*
void ADC1_Multi_Init(void)  //����PA1��PA3   
{ 
	GPIO_InitTypeDef ADC1_GPIO;
	ADC_InitTypeDef  ADC1_Config;
	DMA_InitTypeDef  ADC1_DMA;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |RCC_APB2Periph_GPIOA ,ENABLE );
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	ADC1_GPIO.GPIO_Mode = GPIO_Mode_AIN;
    ADC1_GPIO.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_4|GPIO_Pin_0  ;
    GPIO_Init(GPIOA,&ADC1_GPIO);
	
	ADC1_DMA.DMA_BufferSize = 3;  
	ADC1_DMA.DMA_DIR = DMA_DIR_PeripheralSRC ; //����Ϊ����Դ
	ADC1_DMA.DMA_M2M = DMA_M2M_Disable;  
    ADC1_DMA.DMA_MemoryBaseAddr =(uint32_t)ADC_Value ;//(uint32_t)&ADC_Value;
	ADC1_DMA.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord ;//16λ
	ADC1_DMA.DMA_MemoryInc  = DMA_MemoryInc_Enable ;    //�ڴ��ַ����
	ADC1_DMA.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR)); 
	ADC1_DMA.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    ADC1_DMA.DMA_PeripheralInc = DMA_PeripheralInc_Disable ;  //�����ַ�̶�����ַֻ��һ����
    ADC1_DMA.DMA_Priority = DMA_Priority_High ; 
    ADC1_DMA.DMA_Mode = DMA_Mode_Circular ;//ѭ������	
	DMA_Init(DMA1_Channel1 ,&ADC1_DMA);
    DMA_Cmd(DMA1_Channel1,ENABLE);
	
	ADC1_Config.ADC_Mode = ADC_Mode_Independent ;
	ADC1_Config.ADC_ScanConvMode =ENABLE ;//DISABLE ;��ͨ��ʹ��
	ADC1_Config.ADC_ContinuousConvMode  =ENABLE ;
	ADC1_Config.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ;
	ADC1_Config.ADC_DataAlign = ADC_DataAlign_Right ;
	ADC1_Config.ADC_NbrOfChannel =3;
	ADC_Init(ADC1,&ADC1_Config);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//ÿһ��ͨ����Ҫ���ò���˳���ʱ��
    ADC_RegularChannelConfig(ADC1,ADC_Channel_0 ,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1 ,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4 ,3,ADC_SampleTime_55Cycles5);
    ADC_DMACmd(ADC1 ,ENABLE );    //ADC�ύDMA����
    ADC_Cmd(ADC1 ,ENABLE );  
     
  	ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));	 
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}
*/

  //��ͨ�������ȡ
  /*
void ADC1_Multi_Init(void)  //����PA1��PA3   
{ 
	GPIO_InitTypeDef ADC1_GPIO;
	ADC_InitTypeDef  ADC1_Config;
	DMA_InitTypeDef  ADC1_DMA;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |RCC_APB2Periph_GPIOA ,ENABLE );
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	ADC1_GPIO.GPIO_Mode = GPIO_Mode_AIN;
    ADC1_GPIO.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_3 ;
    GPIO_Init(GPIOA,&ADC1_GPIO);
	
	ADC1_DMA.DMA_BufferSize = 2;  
	ADC1_DMA.DMA_DIR = DMA_DIR_PeripheralSRC ; //����Ϊ����Դ
	ADC1_DMA.DMA_M2M = DMA_M2M_Disable;  
    ADC1_DMA.DMA_MemoryBaseAddr =(uint32_t)ADC_Value ;//(uint32_t)&ADC_Value;
	ADC1_DMA.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord ;//16λ
	ADC1_DMA.DMA_MemoryInc  = DMA_MemoryInc_Enable ;    //�ڴ��ַ����
	ADC1_DMA.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR)); 
	ADC1_DMA.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    ADC1_DMA.DMA_PeripheralInc = DMA_PeripheralInc_Disable ;  //�����ַ�̶�����ַֻ��һ����
    ADC1_DMA.DMA_Priority = DMA_Priority_High ; 
    ADC1_DMA.DMA_Mode = DMA_Mode_Circular ;//ѭ������	
	DMA_Init(DMA1_Channel1 ,&ADC1_DMA);
    DMA_Cmd(DMA1_Channel1,ENABLE);
	
	ADC1_Config.ADC_Mode = ADC_Mode_Independent ;
	ADC1_Config.ADC_ScanConvMode =ENABLE ;//DISABLE ;��ͨ��ʹ��
	ADC1_Config.ADC_ContinuousConvMode  =ENABLE ;
	ADC1_Config.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ;
	ADC1_Config.ADC_DataAlign = ADC_DataAlign_Right ;
	ADC1_Config.ADC_NbrOfChannel =2;
	ADC_Init(ADC1,&ADC1_Config);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	//ÿһ��ͨ����Ҫ���ò���˳���ʱ��
    ADC_RegularChannelConfig(ADC1,ADC_Channel_1 ,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3 ,2,ADC_SampleTime_55Cycles5);
    ADC_DMACmd(ADC1 ,ENABLE );    //ADC�ύDMA����
    ADC_Cmd(ADC1 ,ENABLE );  
     
  	ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));	 
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}
*/

/*
void ADC12_Reg_Init(void)  //adc1-pa1-channel1  adc2-pa3-channel3
{
	GPIO_InitTypeDef ADC1_GPIO;
	DMA_InitTypeDef  ADC1_DMA;
	ADC_InitTypeDef  ADC1_Config;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2|RCC_APB2Periph_ADC1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	ADC1_GPIO.GPIO_Mode = GPIO_Mode_AIN;
    ADC1_GPIO.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 ;
    GPIO_Init(GPIOA,&ADC1_GPIO); 
	
	ADC1_DMA.DMA_BufferSize = 1;               //ÿ��ADC�õ���һ��ͨ��
	ADC1_DMA.DMA_DIR = DMA_DIR_PeripheralSRC ; //����Ϊ����Դ
	ADC1_DMA.DMA_M2M = DMA_M2M_Disable;  
    ADC1_DMA.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConValue;
	ADC1_DMA.DMA_MemoryDataSize = DMA_MemoryDataSize_Word  ;//32λ
	ADC1_DMA.DMA_MemoryInc  = DMA_MemoryInc_Disable ;    //�ڴ��ַ�̶�����ַֻ��һ����
	ADC1_DMA.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR)); 
	ADC1_DMA.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word ;
    ADC1_DMA.DMA_PeripheralInc = DMA_PeripheralInc_Disable ;  //�����ַ�̶�����ַֻ��һ����
    ADC1_DMA.DMA_Priority = DMA_Priority_High ; 
    ADC1_DMA.DMA_Mode = DMA_Mode_Circular ;//ѭ������	
	DMA_Init(DMA1_Channel1 ,&ADC1_DMA);
    DMA_Cmd(DMA1_Channel1,ENABLE);
	//��ʼ��2��ADC
	ADC1_Config.ADC_Mode = ADC_Mode_RegSimult;
	ADC1_Config.ADC_ScanConvMode =DISABLE ;//ADC��ͨ��ʹ��
	ADC1_Config.ADC_ContinuousConvMode =ENABLE ;
	ADC1_Config.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ;
	ADC1_Config.ADC_DataAlign = ADC_DataAlign_Right ;
	ADC1_Config.ADC_NbrOfChannel =1;
	ADC_Init(ADC1,&ADC1_Config);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	ADC_RegularChannelConfig(ADC1 ,ADC_Channel_1,1,ADC_SampleTime_55Cycles5); 
	ADC_DMACmd(ADC1,ENABLE);
     
    //ADC2
    ADC1_Config.ADC_Mode = ADC_Mode_RegSimult; //ͬ������
	ADC1_Config.ADC_ScanConvMode =DISABLE ;//�ر�ɨ��ģʽ
	ADC1_Config.ADC_ContinuousConvMode =ENABLE ;//����ת��ģʽ
	ADC1_Config.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ;
	ADC1_Config.ADC_DataAlign = ADC_DataAlign_Right ;
	ADC1_Config.ADC_NbrOfChannel =1;
	ADC_Init(ADC2,&ADC1_Config);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);	

    ADC_RegularChannelConfig(ADC2,ADC_Channel_4,1,ADC_SampleTime_55Cycles5);
    
    //adc_calibration
    ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));	
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    ADC_Cmd(ADC2,ENABLE);
	ADC_ResetCalibration(ADC2);
    while(ADC_GetResetCalibrationStatus(ADC2));	
    ADC_StartCalibration(ADC2);
    while(ADC_GetCalibrationStatus(ADC2));
    
 	ADC_SoftwareStartConvCmd(ADC1,ENABLE); //ADC1����(���������)��ADC2�Ǵӣ�adc�Ķ�·���ô���������     	
	ADC_ExternalTrigConvCmd(ADC2,ENABLE ); 
}
//	   Value1 = (ADC_ConValue&0xffff0000)>>16;//ȡ����16λ
//     Value2 = (ADC_ConValue&0x0000ffff);	  //ȡ����16λ
//	   ADC_Value_Temp1 = (float)Value1*3.3/4096;
//	   ADC_Value_Temp2 = (float)Value2*3.3/4096;
*/
//u16 Get_Adc_Average(u8 ch,u8 times)
//{
//	u32 temp_val=0;
//	u8 t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=Get_Adc(ch);
//		//delay_us(150);
//	}
//	return temp_val/times;
//} 	 
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}	

 void Control_task(void)
 {
//	tick_ms++;
	// adcx = Get_Adc(ADC_Channel_1);
	 adcx = Get_Adc(ADC_Channel_1);
	 /*
		if(tick_ms%2==0)   //����Ƶ��Ϊ500HZ
		{
			adcx = Get_Adc(ADC_Channel_1); 
		}
	 */
 }
 

