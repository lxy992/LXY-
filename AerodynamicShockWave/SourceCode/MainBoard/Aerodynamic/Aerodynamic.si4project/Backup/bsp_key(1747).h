
#ifndef __KEY_H__
#define __KEY_H__

#define  SYSTEM_KEY_DET()   GPIO_ReadInputDataBit(GPIOB,  GPIO_Pin_12)

#define  LONG_PRESS_TIME_2S  100    //2S
#define  LONG_PRESS_TIME_10S  500

#define  SHORT_PRESS      1
#define  LONG_PRESS_2S    2
#define  LONG_PRESS_10S    3

#define   POWER_KEY    0x80

typedef enum{
   eNull = 0,
   ePress,
   eHold,
   eFree
}eKeyStatus;

typedef enum{
   eShortPress = 0x81,
   eLongPress_2s  = 0x82,
   eLongPress_10s = 0x83
}eKeyFunctionType;

void  KEY_Handle(void);

#endif

