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
  BOOL LED_Flash; //LED ��ˮ����˸��־
  BOOL key_scan;  //��ⰴ����־ >0ɨ���Ƿ��а�������
	//BOOL MotorStop;//��ͣ
	BOOL Start;//
	BOOL AppStart; //APP������������Ϣ
	BOOL MainBoardPower;
 
	
  BOOL ADC_Scan;//ADCɨ��
  BOOL OneSecond;//һ��
  BOOL OLED_Flash;//��ʾˢ��
  BOOL I2C_Recive;//I2C���յ�����
  BOOL I2CMastWrite;//I2C��������
  BOOL I2CMastPoll;//����ѯ
  BOOL ReadTempOneSec;
  BOOL Heartbeat;
  u8 HandConnect; //�ֱ�����������
  
}Flag_StructType;


typedef struct
{
	
	unsigned long SysTime;//ϵͳ��ʱ��
  //unsigned long I2CBusTime;
  uint32_t ShockCnt;//�������
  unsigned int TemperatureValue;//�¶�ֵ
  unsigned int preTemperatureValue;
  unsigned char PressureValue;//����ֵ
  unsigned char FrequencyValue;//Ƶ��ֵ
  unsigned char UartToI2CData;
  uint16_t TempValue;//�¶�����
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

