
#ifndef __LEFT_HAND_H__
#define __LEFT_HAND_H__
 
#define  ILLEGAL_DAT            0x00
#define  RECEIVE_DAT_SUCCESS    0x01

typedef struct{
         u8   ucStatusFlag;              //À¶ÑÀ×´Ì¬±êÖ¾Î»
         u8   ucCmdStatu; 
}LEFT_HAND_StatusParams_t;

#define   LEFT_HAND_StatusParamsDefaults {0,\
	                                    0};

extern   LEFT_HAND_StatusParams_t  LEFT_HAND_StatusParams;

void  LEFT_HAND_MessageHandle(USART_TypeDef* USARTx,DeviceReceiveData_t *device_decode,u8 data_length, u8 *actual_data);


#endif


