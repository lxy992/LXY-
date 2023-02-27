
/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : flow.h
*
*********************************************************************************************************
*/

#ifndef __FLOW_H
#define __FLOW_H


/************************�������ƺ궨��*********************************/
#define  FLOW1COMPAREA    120    //130
#define  FLOW2COMPAREA    125    //132
#define  FLOW3COMPAREA    135    //140
#define  FLOW4COMPAREA    184    //183
#define  FLOWOFFCOMPAREA  80     //106


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
}PID_FlowTypeDef;

extern  PID_FlowTypeDef  Flow_Ctrl;

void  Flow_PIDInit(PID_FlowTypeDef *sptr);
void  TIM1_PWM_Init(u16 arr,u16 psc);
void  Flow_Init(void);
void  Flow_PulseWidth(u8 PulseWidth);
void  Flow_Configuration(u8 flow_mode);
void  Flow_Custom(u16 compare_value);
void  Flow_Handle(void);


#endif

