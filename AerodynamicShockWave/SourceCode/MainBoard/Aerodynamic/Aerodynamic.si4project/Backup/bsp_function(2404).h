
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

#define  BLUETOOTH_LED_ON()     GPIOB->BSRR=1<<6
#define  BLUETOOTH_LED_OFF()    GPIOB->BRR=1<<6 

#define  WORK_LED_ON()     GPIOB->BSRR=1<<2
#define  WORK_LED_OFF()    GPIOB->BRR=1<<2

#define  COMMUNICATION_LED_ON()     GPIOB->BSRR=1<<14 
#define  COMMUNICATION_LED_OFF()    GPIOB->BRR=1<<14

#define  LOCK_OUTPUT_DETECT_ON()    GPIOC->BSRR=1<<10 
#define  LOCK_OUTPUT_DETECT_OFF()   GPIOC->BRR=1<<10 

#define  ANDROID_POWER_ON()     GPIOA->BSRR=1<<5 
#define  ANDROID_POWER_OFF()    GPIOA->BRR=1<<5 


#define  LOCK_DETECT()    GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)

#define  BRIDGE1_ON()   GPIOB->BSRR=1<<8 
#define  BRIDGE1_OFF()  GPIOB->BRR=1<<8

#define  BRIDGE2_ON()   GPIOB->BSRR=1<<9 
#define  BRIDGE2_OFF()  GPIOB->BRR=1<<9

#define SYSTEM_POWER_ON           1       //œµÕ≥ø™ª˙
#define SYSTEM_POWER_OFF          0       //œµÕ≥πÿª˙

#define SYSTEM_RUN                1      //‘À––
#define SYSTEM_PAUSE              0      //‘›Õ£
#define SYSTEM_ERR                2      //“Ï≥£

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


#define  DEVICE_TPYE   BLUETOOTH  //ANDROID BLUETOOTH

#define  FILTER_VAT_ALL_TIME   18
#define  FILTER_VAT_SAME_TIME  FILTER_VAT_ALL_TIME-2

/************************$)AF$7t@`PMFxQ92N?<V5:j6(Re*********************************/
#define  NORMAL_KPA_1  950       //$)AU}3#F$7tFxQ9V5#:950                      56KP
#define  NORMAL_KPA_2  1150      //$)AU}3#F$7tFxQ9V5#:1150                     63KP
#define  NORMAL_KPA_3  1350      //$)AU}3#F$7tFxQ9V5#:1350                     78KP
#define  NORMAL_KPA_4  1550      //$)AU}3#F$7tFxQ9V5#:1550                     88KP

#define  DRY_KPA_1  4100        //dry $)AF$7tFxQ9V5#:4100
#define  DRY_KPA_2  4300        //dry $)AF$7tFxQ9V5#:4300
#define  DRY_KPA_3  4500        //dry $)AF$7tFxQ9V5#:4500
#define  DRY_KPA_4  4700        //dry $)AF$7tFxQ9V5#:4700

typedef struct{
        u8   ucPowerStateFlag;          
        u8   ucRunStateFlag;           
        u8   ucResetFlag;               
        u8   ucAppStartupFlag;         
        u8   ucAppRunPauseFlag;         
        u8   ucAppStateLimitFlag;       
        u8   ucBluetoothConnectFlag;    
        u8   ucBluetoothRunFlag;       
		u16  uiFlashMemoryByte[4];    
		u16  uiMasterAirCylinder;      /**÷˜∏◊∆¯—π÷µ**/
        u8   ucBeepFlag;              //$)A7dCyFw1jV>N;
        float fTemperatureValue;      //$)ANB6HV5
        u8   ucTemperatureStateFlag;   //$)ANB6HW4L,
        u8   ucOverTemperatureProtectFlag; //$)A9}NB1#;$1jV>N;
        u8   ucOverTemperatureProtectBuffer; // $)A9}NB1#;$1j<G;:4f
		u16  uiMasterCompareCylinder; //$)AVw8W5D1H=OFxQ9
		u8   ucLimitPressureSwitch;   //$)AO^VFFxQ9?*9X
		u8   ucFilterVatChannelABFlag; //$)A9}BKM0M(5@ABGP;;1jV>N;
		u8   ucCurrentFlowMode;       //$)A51G0AwA?D#J=
		u8   ucCurrentTaktMode;       //$)A51G0=ZEDD#J=
		u16  uiSetPressureValue;      //$)AIhVCFxQ9V5
		u16  uiSetFlowPressureValue;  //$)AUkM2MbCf5DQ9A&#,R2JGflow5DJd3vFZM{Q9A&
		u16  uiFlowPulseWidth;        //$)AAwA?Bv3e?m6H 
		u8   ucChannelAB;             //$)AM(5@AB5D?*9X
        u8   ucChannelA;              //$)AM(5@A5D?*9X
        u8   ucChannelB;              //$)AM(5@B5D?*9X
        u8   ucID[6];                //$)AIh18ID:E
        u8   ucUart5RcvBuf[20];      //$)A4.?Z=SJU;:4f
        u8   ucUart5RcvSta;          //$)A4.?Z=SJUW4L,  
        u8   ucFlashWriteFlag;       //Flash $)AP41jV>N;
        u16  uiSetCompressorVoltage; //$)AQ9Ku;z5gQ9V5
        u16  uiNtc1VoltageValue;    //NTC1µÁ—π÷µ
        u16  uiNtc2VoltageValue;    //NTC2µÁ—π÷µ
        u8   ucTemperature1Value;   
		u8   ucTemperature2Value;
		u8   ucCommunicationFlag;
		u16  uiMasterPressure;
		u16  uiSlavePressure;
		u8   ucLockStep;
		u8   ucUnlockFlag;
		u8   ucOpenLockFlag;
		u8   ucCurrentFlowModeBuffer;
		u8   ucCurrentTaktModeBuffer;
		u8   ucKey[4];
		u8   ucHandShakeStatusFlag;
		u8   ucOverTemperatureProtectResponseFlag;
		u8   ucUnLockResponseFlag;
		u8   ucAgingTestMode;
		u8   ucStartupFlag;
		u8   ucReceiveMacAddressFlag;
		u8   ucMacAddress[12];
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
void  FilterVat_ChannelSwtichOutput(void);
void  FilterVat_ChannelASwitch(FunctionalState NewState);
void  FilterVat_ChannelBSwitch(FunctionalState NewState);
void  FilterVat_ChannelSwtichOutput(void);
void  Fan_Switch(FunctionalState NewState);
void  Compressor_Switch(FunctionalState NewState);
void  LED_OpenLock(void);

void  Delay_ms(u16   ms);

#endif


