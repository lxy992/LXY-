
#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#define  EN_POWER_ON()     GPIOA->BSRR=1<<15
#define  EN_POWER_OFF()    GPIOA->BRR=1<<15

#define  DWIN_POWER_ON()       GPIOA->BSRR=1<<5
#define  DWIN_POWER_OFF()      GPIOA->BRR=1<<5

#define  PUMP_POWER_ON()       GPIOC->BRR=1<<12
#define  PUMP_POWER_OFF()      GPIOC->BSRR=1<<12

#define  FAN_ON()          GPIOC->BSRR=1<<4
#define  FAN_OFF()         GPIOC->BRR=1<<4

#define  LEFT_HAND_POWER_ON()          GPIOC->BSRR=1<<5
#define  LEFT_HAND_POWER_OFF()         GPIOC->BRR=1<<5

#define  RIGHT_HAND_POWER_ON()          GPIOC->BSRR=1<<6
#define  RIGHT_HAND_POWER_OFF()         GPIOC->BRR=1<<6

#define  LEFT_HAND_DETECT()    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)
#define  RIGHT_HAND_DETECT()   GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)


#define SYSTEM_POWER_ON           1       //系统开机
#define SYSTEM_POWER_OFF          0       //系统关机

#define SYSTEM_RUN                1      //运行
#define SYSTEM_PAUSE              0      //暂停
#define SYSTEM_ERR                2      //异常

#define SYSTEM_RESET_STATUS       0
#define SYSTEM_SET_STATUS         1

#define SYSTEM_WORK  1 
#define SYSTEM_TEST  0

#define TRUE  0
#define FALSE 1

#define  NOT_ALLOW    0
#define  ALLOW        1

#define  PRESSURE_SENSOR_4250 0

#define  MAIN_DEBUG_WINDOW    1
#define  PID_DEBUG_WINDOW     0

#define  PASSWORD      0
#define  DAC_EN        0
#define  BT_NAME       1
#define  BT_HANDSHAKE  1
#define  UPDATE        1


#define  DEVICE_TPYE  DWIN


typedef struct{
        u8   ucPowerStateFlag;  
		u8   ucDwinStartFlag;
        u8   ucRunStateFlag;
		u8   ucLeftHandStartWorkFlag;
		u8   ucRightHandStartWorkFlag;
        u8   ucResetFlag;   
		u8   ucPrescription;             /**处方**/
		u8   ucFrqMode;                 /**频率模式**/
		u8   ucSetBarValue;              /**设置Bar值**/
        u16  uiSetPressureValue;        /**设置气压值**/
		u16  uiSetCount;                /**设置次数**/
		u16  uiMasterAirCylinderAdc;      /**主缸气压值**/
        u8   ucBeepFlag;               /**蜂鸣器标志位**/
		u16  uiMasterPressure;        /**主缸气压值**/
		u8   ucLeftHandLoadPull;      /**左手柄导入拔出**/
		u8   ucRightHandLoadPull;     /**右手柄导入拔出**/
}SystemParams_t;

#define SystemParams_Defaults   {0,\
	                             0,\
	                             0,\
	                             0,\
	                             0,\
	                             0,\
                                 0,\
                                 0,\
                                 0,\
                                 0,\
                                 0,\
                                 0,\
                                 0,\
                                 0,\
                                 0,\
                                 0}


extern SystemParams_t SystemParams;

void  System_Init(void);
void  SystemWorkState(u8         power_state);
void  SystemStartupOrShutdown(FunctionalState NewState);
void  SystemRunOrPause(FunctionalState NewState);
void  Fan_Switch(FunctionalState NewState);

void  HandStatusDetect_GPIO_Init(void);
void  EN_12V_GPIO_Init(void);
void  DWIN_POWER_GPIO_Init(void);
void  AirPump_GPIO_Init(void);
void  LED_GPIO_Init(void);
void  FAN_GPIO_Init(void);
void  HAND_POWER_GPIO_Init(void);

void  Delay_ms(u16   ms);

#endif


