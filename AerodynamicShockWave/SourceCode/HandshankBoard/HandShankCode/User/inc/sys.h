/**
  ******************************************************************************
  * @file    sys.h
  * @author  ADT LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __SYS_H__
#define __SYS_H__

#ifdef __cplusplus
  extern "C" {
#endif
    
#include "stm8s.h"
      
#define FREQUENCY_NUM 22
#define ENERGY_NUM 14


#define SHOCK_START 1
#define SHOCK_STOP  0


//typedef enum{FALSE=0, TRUE = !FALSE}BOOL,bool;
typedef bool BOOL;
typedef enum{OFF=0, ON = !OFF}OnOffSatus;
typedef struct
{
	BOOL power;
  //BOOL flag;
  BOOL LED_Flash; //LED 流水灯闪烁标志
  BOOL key_scan;  //检测按键标志 >0扫描是否有按键按下
	//BOOL MotorStop;//暂停
	BOOL Start;//
	BOOL AppStart; //APP发来的启动信息
	BOOL MainBoardPower;
 
	
  BOOL ADC_Scan;//ADC扫描
  BOOL OneSecond;//一秒
  BOOL OLED_Flash;//显示刷新
  BOOL I2C_Recive;//I2C接收到数据
  BOOL I2CMastWrite;//I2C主机发送
  BOOL I2CMastPoll;//主查询
  BOOL ReadTempOneSec;
  BOOL Heartbeat;
  u8 HandConnect; //手柄连接上主板
  
}Flag_StructType;


typedef struct
{
	
	unsigned long SysTime;//系统计时器
  //unsigned long I2CBusTime;
  uint32_t ShockCnt;//捶打次数
  unsigned int TemperatureValue;//温度值
  unsigned int preTemperatureValue;
  unsigned char PressureValue;//能量值
  unsigned char FrequencyValue;//频率值
  unsigned char UartToI2CData;
  uint16_t TempValue;//温度数据
  uint32_t EepromDataValue;
}GlobalVariable_StructType;

extern unsigned char HexToAsciiTab[];

extern  Flag_StructType Flag;
extern  GlobalVariable_StructType GlobalVar;
extern u32 g_Start_ShockValue ;
extern u32 g_Stop_ShockValue ;

extern void NumberToAscii(uint8_t *buf, uint32_t number, uint8_t len);
void Paremeter_Init(void);
void Write_EEPROM(u16 addr, u8* p_data, u16 len);
void Write_EEPROM_NewData(void);
u32 Read_EEPROM_ShockCnt (void);

void Record_StartAndStop_Value(void);
void  Read_EEPROM_Data(void);
void  SendEepromShockCnt(void);
void  SendEepromShockCnt(void);

#ifdef __cplusplus
  }
#endif

#endif /* __SYS_H__ */

/* ************ ****** ************ THE FILE END  ************ ****** ************ */

