/*
*********************************************************************************************************
*
*	模块名称 : 节拍控制模块
*	文件名称 : takt.h
*
*********************************************************************************************************
*/

#ifndef __TAKT_H
#define __TAKT_H


/************************节拍控制宏定义*********************************/
#define  TAKT1PERIOD      1000
#define  TAKT1COMPAREC    500   //10Hz   T=100ms G=50ms D=50ms

#define  TAKT2PERIOD      2000
#define  TAKT2COMPAREC    1000  //5Hz    T=200ms G=100ms D=100ms

#define  TAKT3PERIOD      4000
#define  TAKT3COMPAREC    2000  //2.5Hz  T=400ms G=200ms D=200ms

#define  TAKT4PERIOD      5000
#define  TAKT4COMPAREC    3000  //2Hz    T=500ms G=300ms D=200ms

#define  TAKTOFFPERIOD    10
#define  TAKTOFFCOMPAREC  11    //0Hz

#define  TAKT6PERIOD      1111
#define  TAKT6COMPAREC    556   //9Hz    T=111ms G=55.5ms D=55.5ms

#define  TAKT7PERIOD      1250
#define  TAKT7COMPAREC    625   //8Hz    T=125ms G=62.5ms D=62.5ms

#define  TAKT8PERIOD      1430
#define  TAKT8COMPAREC    715   //7Hz    T=143ms G=71.5ms D=71.5ms

#define  TAKT9PERIOD      1667
#define  TAKT9COMPAREC    834   //6Hz    T=167ms G=83.5ms D=83.5ms

#define  TAKT10PERIOD     2500
#define  TAKT10COMPAREC   1250  //4Hz    T=250ms G=125ms D=125ms

#define  TAKT11PERIOD     3333
#define  TAKT11COMPAREC   1667  //3Hz    T=333ms G=166.5ms D=166.5ms


void  TIM4_PWM_Init(u16 arr,u16 psc);
void  Takt_Init(void);
void  Takt_Configuration(u8 take_mode);


#endif

