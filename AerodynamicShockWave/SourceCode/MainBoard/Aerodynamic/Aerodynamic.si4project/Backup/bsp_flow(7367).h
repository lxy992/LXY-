
/*
*********************************************************************************************************
*
*	模块名称 : 流量控制模块
*	文件名称 : flow.h
*
*********************************************************************************************************
*/

#ifndef __FLOW_H
#define __FLOW_H


/************************流量控制宏定义*********************************/
#define  FLOW1COMPAREA    120    //130
#define  FLOW2COMPAREA    125    //132
#define  FLOW3COMPAREA    135    //140
#define  FLOW4COMPAREA    184    //183
#define  FLOWOFFCOMPAREA  80     //106


typedef struct 
{
    int      SetValue;        //设置目标值
    int      FdbValue;        //反馈值
    int      iError;          //增量误差值
    int      iError_1;       //最近误差值 
    int      iError_2;       //上一个误差值 
    float    Kp;             //比例常数
    float    Ki;             //积分常数
    float    Kd;             //微分常数
    int      LimitKi;        //积分限制值
    int      iOut;           //PID增量输出值
    int      LastOut;        //PID增量最近输出值 
    int      PresOut;        //PID当前输出值     
    int      OutBaseValue;   //输出基准值
    int      OutMax;	    // 输出1最大阀值控制值 
    int      OutMin;	    // 输出最小阀值控制值
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

