
#ifndef _DECODE_H__
#define _DECODE_H__

#define   UART_DAT_LENGTH   30

#define   DWIN        1
#define   LEFT_HAND   2
#define   RIGHT_HAND  3

typedef struct
{
    u16 uiFrameHeader;
    u8  ucDataLength;
    u8  ucCmdType;
    u8  ucDataField[20];
}DeviceReceiveData_t;

typedef enum
{
    eFrameHeader,
    eDataLength,
    eCmdType,
    eDataField,
}eDeviceCmdStatus;


extern DeviceReceiveData_t Dwin_Code;


void DEVICE_VarInit(DeviceReceiveData_t *device_id);
void DeviceReadData(USART_TypeDef* USARTx, u16 temp_addr);
void DeviceWriteData(USART_TypeDef* USARTx, u16 temp_addr,u8 *data);
u8 CheckData(u8 input_data,u8 min_num,u8 max_num);
void DEVICE_MessageDecode(u8 device,UsartReceiveData_t *device_id,DeviceReceiveData_t *device_decode,u8 *cmd_status);

#endif



