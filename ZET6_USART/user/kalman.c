#include "kalman.h"
#include "math.h"
/*
    Rֵ�̶���QֵԽ�󣬴���Խ���β���ֵ��Qֵ����󣬴���ֻ�ò���ֵ��
             QֵԽС������Խ����ģ��Ԥ��ֵ��QֵΪ0������ֻ��ģ��Ԥ��ֵ��
*/
                              //����һ
float  KalmanFilter( float  inData )
{
    static float prevData = 0;                            //��һ������
    static float p = 10, q = 0.001, r = 0.001, kGain = 0; // q ������� r ������Ӧ�ٶ�
    p = p + q;
    kGain = p / ( p + r );                                //���㿨��������
    inData = prevData + ( kGain * ( inData - prevData ) );//���㱾���˲�����ֵ
    p = ( 1 - kGain ) * p;                                //���²�������
    prevData = inData;
    return inData;                                        //���ع���ֵ
}
                              //������
unsigned long kalman_filter( unsigned long ADC_Value )
{
    float LastData;
    float NowData;
    float kalman_adc;
    static float kalman_adc_old = 0;
    static float P1;
    static float Q = 0.0003;
    static float R = 5;
    static float Kg = 0;
    static float P = 1;
    NowData = ADC_Value;
    LastData = kalman_adc_old;
    P = P1 + Q;
    Kg = P / ( P + R );
    kalman_adc = LastData + Kg * ( NowData - kalman_adc_old );
    P1 = ( 1 - Kg ) * P;
    P = P1;
    kalman_adc_old = kalman_adc;
    return ( unsigned long )( kalman_adc );
}

/*
    һ�׵�ͨ�˲���
        y(n) = q*x(n) + (1-q)*y(n-1)    ����Y��n��Ϊ�����x��n��Ϊ���롣����Qȡ0.5
*/
unsigned int lowV( unsigned int com )
{
    static unsigned int iLastData;
    unsigned int iData;
    float dPower = 0.8;
    iData = ( com * dPower ) + ( 1 - dPower ) * iLastData; //����
    iLastData = iData;                                     //������������
    return iData;                                         //��������
}
















