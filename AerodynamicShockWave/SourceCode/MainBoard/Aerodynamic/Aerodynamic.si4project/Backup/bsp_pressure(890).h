/*
*********************************************************************************************************
*
*	模块名称 : 气压控制模块
*	文件名称 : pressure.h
*
*********************************************************************************************************
*/

#ifndef __PRESSURE_H
#define __PRESSURE_H


#define  EN_V1_ON()        GPIOA->BSRR=1<<7
#define  EN_V1_OFF()       GPIOA->BRR=1<<7

#define  EN_V2_ON()        GPIOA->BSRR=1<<6
#define  EN_V2_OFF()       GPIOA->BRR=1<<6

#define  MAXDRUCK  2100  //主缸限制的最大气压值   285KPa         1750    2500 400KPa     

typedef struct { //稳定某个目标气压的结构体
    unsigned char TimerStart;        //定时器开关
    unsigned int  TimeMin;           //定时器最小值
    unsigned int  TimeMax;           //定时器最大值
    unsigned char OutAirStatus;      //排气的状态为0表示结束，为1表示排气中
    unsigned char InAirStatus;       //进气的状态为0表示结束，为1表示进气中
    unsigned int  Time500ms;         //定时500ms
    unsigned char Timer500msKey;     //定时500ms的开关
    unsigned char ReadPressureValueFlag;  //读气压标志位
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

