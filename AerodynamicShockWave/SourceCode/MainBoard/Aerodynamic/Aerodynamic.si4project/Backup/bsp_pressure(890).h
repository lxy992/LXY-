/*
*********************************************************************************************************
*
*	ģ������ : ��ѹ����ģ��
*	�ļ����� : pressure.h
*
*********************************************************************************************************
*/

#ifndef __PRESSURE_H
#define __PRESSURE_H


#define  EN_V1_ON()        GPIOA->BSRR=1<<7
#define  EN_V1_OFF()       GPIOA->BRR=1<<7

#define  EN_V2_ON()        GPIOA->BSRR=1<<6
#define  EN_V2_OFF()       GPIOA->BRR=1<<6

#define  MAXDRUCK  2100  //�������Ƶ������ѹֵ   285KPa         1750    2500 400KPa     

typedef struct { //�ȶ�ĳ��Ŀ����ѹ�Ľṹ��
    unsigned char TimerStart;        //��ʱ������
    unsigned int  TimeMin;           //��ʱ����Сֵ
    unsigned int  TimeMax;           //��ʱ�����ֵ
    unsigned char OutAirStatus;      //������״̬Ϊ0��ʾ������Ϊ1��ʾ������
    unsigned char InAirStatus;       //������״̬Ϊ0��ʾ������Ϊ1��ʾ������
    unsigned int  Time500ms;         //��ʱ500ms
    unsigned char Timer500msKey;     //��ʱ500ms�Ŀ���
    unsigned char ReadPressureValueFlag;  //����ѹ��־λ
}PressureParams_t;

#define  PressureParams_Defaults {0,\
	                              0,\
	                              0,\
	                              0,\
	                              0,\
	                              0,\
	                              0,\
	                              0}

extern  PressureParams_t  PressureParams;

typedef struct 
{
    int      SetValue;        //����Ŀ��ֵ
    int      FdbValue;        //����ֵ
    int      iError;          //�������ֵ
    int      iError_1;       //������ֵ 
    int      iError_2;       //��һ�����ֵ 
    float    Kp;             //��������
    float    Ki;             //���ֳ���
    float    Kd;             //΢�ֳ���
    int      LimitKi;        //��������ֵ
    int      iOut;           //PID�������ֵ
    int      LastOut;        //PID����������ֵ 
    int      PresOut;        //PID��ǰ���ֵ     
    int      OutBaseValue;   //�����׼ֵ
    int      OutMax;	    // ���1���ֵ����ֵ 
    int      OutMin;	    // �����С��ֵ����ֵ
}PID_PressureTypeDef;

extern  PID_PressureTypeDef  Pressure_Ctrl;

void  PressureValve_GPIO_Init(void);
void  Pressure_PIDInit(PID_PressureTypeDef *sptr);
void  Venting_Switch(FunctionalState NewState);
void  Intake_Switch(FunctionalState NewState);
void  Pressure_Init(void);
void  Pressure_Handle(void);
void  LimitPressure_Init(void); 
void  MasterAirCylinder_Switch(FunctionalState NewState);
void  LimitPressure_Handle(void);

#endif

