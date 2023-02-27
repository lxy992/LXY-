
#ifndef _DECODE_H__
#define _DECODE_H__

#define   UART_DAT_LENGTH   80

#define   BLUETOOTH  1
#define   ANDROID    2

typedef struct
{
    u16 uiFrameHeader;
    u8  ucDataLength;
    u8  ucDeviceID;
    u8  ucCmdType;
    u8  ucCmdStatu;
    u8  ucDataField[20];
    u16 uiCrc16;
}DeviceReceiveData_t;

typedef enum
{
    eFrameHeader,
    eDataLength,
    eDeviceID,
    eCmdType,
    eCmdStatu,
    eDataField,
    eCrc16
}eDeviceCmdStatus;

extern DeviceReceiveData_t Bluetooth_Code;
extern DeviceReceiveData_t Android_Code;


void DEVICE_VarInit(DeviceReceiveData_t *device_id);
void SendDatPacketToDevice(USART_TypeDef* USARTx,u8 data_field_length, u8 cmd_type, u8 cmd_status, u8 *data_field);
u8 CheckData(u8 input_data,u8 min_num,u8 max_num);
void DEVICE_MessageDecode(u8 device,UsartReceiveData_t *device_id,DeviceReceiveData_t *device_decode,u8 *cmd_status);

#endif



