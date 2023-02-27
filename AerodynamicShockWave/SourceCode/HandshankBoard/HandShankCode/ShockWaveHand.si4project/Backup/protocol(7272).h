/**
  ******************************************************************************
  * @file    protocol.h
  * @author  LL
  * @version V0.0.0
  * @date    2017-02-27
  * @brief   Í·ÎÄ¼þ
  ******************************************************************************
  ******************************************************************************
***/

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

typedef struct
{
    uint8_t step;
    uint8_t len;
    uint8_t cnt;
    uint8_t crc16_h;
	uint8_t crc16_l;
	uint8_t DataCodeBuff[256];
} Protocol_StructType;

//ÃüÁî×´Ì¬
#define COMMANDSTATE_INVALID 0x00
#define COMMANDSTATE_RECIVEOK 0x01
#define COMMANDSTATE_SEND 0x02

#define COMMANDCODE_FAULT 0x19


extern Protocol_StructType *pProtocol_DataPort1;

extern void Protocol_PrintfCommand(uint8_t sta, uint8_t cmd, uint32_t para);
extern void Protocol_PrintfData(uint8_t sta, uint8_t cmd, uint8_t *para, uint16_t len);
extern void Protocol_Init(void);
extern void Protocol_DataDecode(uint8_t data, Protocol_StructType *p);
extern void Protocol_Test(uint8_t data);
void Protocol_PrintfData(uint8_t sta, uint8_t cmd, uint8_t *para, uint16_t len);

#endif /* __PROTOCOL_H__ */


