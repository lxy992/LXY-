
#ifndef __DWIN_H__
#define __DWIN_H__

#define  SEND_CMD               0x83   
#define  SEND_DATA              0x82

#define  ILLEGAL_DAT            0x00
#define  RECEIVE_DAT_SUCCESS    0x01

#define  SYSTEM_RUN_PAUSE_CMD   0x02
#define  SET_PRESCRIPTION_CMD   0x03
#define  SET_FRQ_CMD            0X04
#define  SET_PRESSURE_CMD       0X05
#define  SET_CNT_CMD            0X06

#define  HAND_STARTING_UP       0x33

typedef struct{
         u8   ucStatusFlag;              //À¶ÑÀ×´Ì¬±êÖ¾Î»
         u8   ucCmdStatu; 
}DWIN_StatusParams_t;

#define   DWIN_StatusParamsDefaults {0,\
	                                    0};

extern   DWIN_StatusParams_t  DWIN_StatusParams;

void  DWIN_MessageHandle(USART_TypeDef* USARTx,DeviceReceiveData_t *device_decode,u8 data_length, u8 *actual_data);


#endif


