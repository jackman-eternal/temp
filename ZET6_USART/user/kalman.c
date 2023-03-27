#include "kalman.h"
#include "math.h"
/*
    R值固定，Q值越大，代表越信任测量值，Q值无穷大，代表只用测量值。
             Q值越小，代表越信任模型预测值，Q值为0，代表只用模型预测值。
*/
                              //参数一
float  KalmanFilter( float  inData )
{
    static float prevData = 0;                            //上一个数据
    static float p = 10, q = 0.001, r = 0.001, kGain = 0; // q 控制误差 r 控制响应速度
    p = p + q;
    kGain = p / ( p + r );                                //计算卡尔曼增益
    inData = prevData + ( kGain * ( inData - prevData ) );//计算本次滤波估计值
    p = ( 1 - kGain ) * p;                                //更新测量方差
    prevData = inData;
    return inData;                                        //返回估计值
}
                              //参数二
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
    一阶低通滤波器
        y(n) = q*x(n) + (1-q)*y(n-1)    其中Y（n）为输出，x（n）为输入。其中Q取0.5
*/
unsigned int lowV( unsigned int com )
{
    static unsigned int iLastData;
    unsigned int iData;
    float dPower = 0.8;
    iData = ( com * dPower ) + ( 1 - dPower ) * iLastData; //计算
    iLastData = iData;                                     //存贮本次数据
    return iData;                                         //返回数据
}
















