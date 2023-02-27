
#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#define  EN_POWER_ON()     GPIOB->BSRR=1<<5
#define  EN_POWER_OFF()    GPIOB->BRR=1<<5

#define  LED_ORANGE_ON()   GPIOC->BSRR=1<<5
#define  LED_ORANGE_OFF()  GPIOC->BRR=1<<5

#define  LED_BLUE_ON()     GPIOB->BSRR=1<<0
#define  LED_BLUE_OFF()    GPIOB->BRR=1<<0

#define  FAN_ON()          GPIOC->BSRR=1<<6
#define  FAN_OFF()         GPIOC->BRR=1<<6

#define  COMPRESSOR_ON()   GPIOC->BSRR=1<<9
#define  COMPRESSOR_OFF()  GPIOC->BRR=1<<9

#define  EN_PI1_ON()       GPIOC->BSRR=1<<8
#define  EN_PI1_OFF()      GPIOC->BRR=1<<8

#define  EN_PI2_ON()       GPIOC->BSRR=1<<7
#define  EN_PI2_OFF()      GPIOC->BRR=1<<7

#define  WORK_LED_ON()     GPIOB->BSRR=1<<2
#define  WORK_LED_OFF()    GPIOB->BRR=1<<2

#define  COMMUNICATION_LED_ON()     GPIOB->BSRR=1<<14 
#define  COMMUNICATION_LED_OFF()    GPIOB->BRR=1<<14

#define  ANDROID_POWER_ON()     GPIOA->BSRR=1<<5 
#define  ANDROID_POWER_OFF()    GPIOA->BRR=1<<5 


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
        u8   ucRunStateFlag;           
        u8   ucResetFlag;                                
		u16  uiMasterAirCylinder;      /**主缸气压值**/
        u8   ucBeepFlag;               /**蜂鸣器标志位**/
		u8   ucCurrentTaktMode;       /**节拍模式**/
		u16  uiSetPressureValue;      /**设置气压值**/
		u16  uiMasterPressure;        /**主缸气压值**/
		u8   ucLeftHandLoadPull;
		u8   ucRightHandLoadPull;
}SystemParams_t;

#define SystemParams_Defaults   {0,\
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
void  FilterVat_ChannelSwtichOutput(void);
void  FilterVat_ChannelASwitch(FunctionalState NewState);
void  FilterVat_ChannelBSwitch(FunctionalState NewState);
void  FilterVat_ChannelSwtichOutput(void);
void  Fan_Switch(FunctionalState NewState);

void  HandStatusDetect_GPIO_Init(void);

void  Delay_ms(u16   ms);

#endif


