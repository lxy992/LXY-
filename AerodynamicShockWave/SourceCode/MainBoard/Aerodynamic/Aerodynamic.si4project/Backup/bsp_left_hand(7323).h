
#ifndef __DWIN_H__
#define __DWIN_H__

#define  SEND_CMD               0x83   
#define  ILLEGAL_DAT            0x00
#define  RECEIVE_DAT_SUCCESS    0x01

typedef struct{
         u8   ucStatusFlag;              //À¶ÑÀ×´Ì¬±êÖ¾Î»
         u8   ucCmdStatu; 
}DWIN_StatusParams_t;

#define   DWIN_StatusParamsDefaults {0,\
	                                    0};

extern   DWIN_StatusParams_t  DWIN_StatusParams;

void  DWIN_MessageHandle(USART_TypeDef* USARTx,DeviceReceiveData_t *device_decode,u8 data_length, u8 *actual_data);


#endif


