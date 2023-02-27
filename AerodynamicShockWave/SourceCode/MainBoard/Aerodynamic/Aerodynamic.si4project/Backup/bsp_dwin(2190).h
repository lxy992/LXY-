
#ifndef __ANDROID_H__
#define __ANDROID_H__


typedef struct{
         u8   ucStatusFlag;              //À¶ÑÀ×´Ì¬±êÖ¾Î»
         u8   ucCmdStatu; 
}Android_StatusParams_t;

#define   Android_StatusParamsDefaults {0,\
	                                    0};

extern   Android_StatusParams_t  Android_StatusParams;

void  ANDROID_MessageHandle(USART_TypeDef* USARTx,DeviceReceiveData_t *device_decode,u8 data_length, u8 *actual_data);


#endif


