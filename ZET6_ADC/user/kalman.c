#include "kalman.h"

KALMAN kal_ADC;
void Kalman_Init(void)
{
 kal_ADC.Q=2e2;//0.0025;
 kal_ADC.R=2e2;//0.3;
 kal_ADC.Pk_0 = 1;
 kal_ADC.Xk_0 = 0;
 kal_ADC.Kg = 0;
	
}
float Kalman(float Zk)
{
	float Xk_; //����Ԥ��ֵ
	float Pk_; //�������Э����
	float Xk;//����Ԥ��ֵ
	

	//Ԥ��
	Xk_ =  kal_ADC.Xk_0 ;
  Pk_ =  kal_ADC.Pk_0 +kal_ADC.Q ;
	//У��
	kal_ADC.Kg = Pk_/(Pk_+kal_ADC.R );
	Xk = Xk_ + kal_ADC.Kg *(Zk - Xk_);
	kal_ADC.Pk_0 = Pk_*(1-kal_ADC.Kg );
	kal_ADC.Xk_0 = Xk;
//	printf("XK = %.2f \r\n ",Xk);
//	printf("ִ��");
	return Xk;
}
