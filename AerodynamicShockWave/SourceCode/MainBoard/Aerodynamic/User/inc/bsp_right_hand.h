
#ifndef __RIGHT_HAND_H__
#define __RIGHT_HAND_H__

typedef struct{
         u8   ucStatusFlag;              //????״̬??־λ
         u8   ucCmdStatu; 
}RIGHT_HAND_StatusParams_t;

#define   RIGHT_HAND_StatusParamsDefaults {0,\
	                                    0};

extern   RIGHT_HAND_StatusParams_t  RIGHT_HAND_StatusParams;

void  RIGHT_HAND_MessageHandle(USART_TypeDef* USARTx,DeviceReceiveData_t *device_decode,u8 data_length, u8 *actual_data);


#endif


