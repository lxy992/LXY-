
#include "config.h"

/***********************************
函数功能：设备缓存变量初始化
输入参数：设备名�?
输出参数：无
�?   注：设备缓存变量初始�?
************************************/
void DEVICE_VarInit(DeviceReceiveData_t *device_id)
{
    device_id->uiFrameHeader = 0; 
    device_id->ucDataLength = 0;
    device_id->ucDeviceID = 0;
    device_id->ucCmdType = 0;
    device_id->ucCmdStatu = 0;
    device_id->uiCrc16 = 0;
    for(u8 i = 0; i < 20; i++)
    {
        device_id->ucDataField[i] = 0x00;
    }
}

/***********************************
函数功能：发送数据包到设�?
输入参数：串口名称，数据长度，命令类型，命令状态，数据�?
输出参数：无
�?   注：发送数据包到设�?
************************************/
void SendDatPacketToDevice(USART_TypeDef* USARTx,u8 data_field_length, u8 cmd_type, u8 cmd_status, u8 *data_field)
{
    u8  send_data[18];
    u16 temp_crc16 = 0;

    send_data[0] = 0x53;
    send_data[1] = 0x47;
    send_data[2] = 4 + data_field_length;
    send_data[3] = 0x00;
    send_data[4] = SYSTEMDEVICE_ID;
    send_data[5] = cmd_type;
    send_data[6] = cmd_status;

    for(u8 i = 0; i < data_field_length; i++)
    {
        send_data[7 + i] = data_field[i];
    }

    temp_crc16 = CRC16_Check(&send_data[2], 5 + data_field_length);

    send_data[6 + data_field_length + 1] =  (u8)(temp_crc16 >> 8);
    send_data[6 + data_field_length + 1 + 1] = (u8)(temp_crc16);

     UART_SendData(USARTx,&send_data[0],(7 + data_field_length + 2));
}

/***********************************
函数功能：核查数�?
输入参数：输入数据，最小值，最大�?
输出参数：接收状�?
�?   注：核查数据
************************************/
u8 CheckData(u8 input_data,u8 min_num,u8 max_num)
{
   if((input_data>=min_num)&&(input_data<=max_num))
   {
      return RECEIVE_DAT_SUCCESS;
   }
   else
   {
      return ILLEGAL_DAT;
   }
}

/***********************************
函数功能：设备解�?
输入参数：设备名�?  解码数据状�?
输出参数：无
�?   注：设备解码
************************************/
void DEVICE_MessageDecode(u8 device,UsartReceiveData_t *device_id,DeviceReceiveData_t *device_decode,u8 *cmd_status)
{
    u8 data_temp = 0;
    static u8 rx_count = 0;

	if((device_id->receive_full_flag == 1)&&(device_id->receive_signal_flag == 0))
	{
        device_id->receive_signal_flag = 1;     
	}
	
    if(device_id->receive_signal_flag)
    {
        switch(*cmd_status)
        {
            case eFrameHeader: 
                if(rx_count == 0)
                {
                    device_decode->uiFrameHeader = UART_GetByte(device_id);
                    device_decode->uiFrameHeader <<= 8;
                    if(device_decode->uiFrameHeader == 0X5300)
                    {
                        rx_count++;
                    }
                    else
                    {
                        device_decode->uiFrameHeader = 0;
                    }
                }
                else
                {
                    device_decode->uiFrameHeader |= UART_GetByte(device_id);
                    if(device_decode->uiFrameHeader  == 0x5347)
                    {
                        *cmd_status = eDataLength;
                    }
                    else
                    {
                        *cmd_status = eFrameHeader;
                    }
                    rx_count = 0;
                }
            break;
            case eDataLength: //数据长度
                device_decode->ucDataLength = UART_GetByte(device_id);
                *cmd_status = eDeviceID;
                device_decode->ucDataField[0] = device_decode->ucDataLength;
            break;
            case eDeviceID:  //设备ID�?
                if(rx_count == 0)
                {
                    device_decode->ucDeviceID = UART_GetByte(device_id);
                    device_decode->ucDataField[1] =  device_decode->ucDeviceID;

                    device_decode->ucDeviceID <<= 8;
                    if(device_decode->ucDeviceID == 0X0000)
                    {
                        rx_count++;
                    }
                    else
                    {
                        device_decode->ucDeviceID = 0;
                        *cmd_status = eFrameHeader;
                    }
                }
                else
                {
                    device_decode->ucDeviceID |= UART_GetByte(device_id);
                    device_decode->ucDataField[2] = device_decode->ucDeviceID;

                    if(device_decode->ucDeviceID == SYSTEMDEVICE_ID)
                    {
                        *cmd_status = eCmdType;
                    }
                    else
                    {
                        *cmd_status = eFrameHeader;
                    }
                    rx_count = 0;
                }
            break;
            case eCmdType: //命令类型
                device_decode->ucCmdType = UART_GetByte(device_id);
                *cmd_status = eCmdStatu;
                device_decode->ucDataField[3] = device_decode->ucCmdType;
            break;
            case eCmdStatu: //命令状�?
                device_decode->ucCmdStatu = UART_GetByte(device_id);
                *cmd_status = eDataField;
                device_decode->ucDataField[4] = device_decode->ucCmdStatu;
            break;
            case eDataField: //数据�?
                rx_count++;
                device_decode->ucDataField[rx_count + 4] = UART_GetByte(device_id);
                if(rx_count == (device_decode->ucDataLength - 4))
                {
                    rx_count = 0;
                    *cmd_status = eCrc16;
                }
            break;
            case eCrc16:  //CRC校验
                data_temp = UART_GetByte(device_id);
                if(rx_count == 0)
                {
                    device_decode->uiCrc16 = ((u16)data_temp) << 8;
                    rx_count++;
                }
                else
                {
                    device_decode->uiCrc16 |= ((u16)data_temp);
                   
                    if(device_decode->uiCrc16 == CRC16_Check(device_decode->ucDataField,device_decode->ucDataLength + 1))
                    {                                       
                       switch(device)
                       {
                          case  BLUETOOTH:
						     BLUETOOTH_MessageHandle(USART2,device_decode,(device_decode->ucDataLength) - 4,&(device_decode->ucDataField[5]));	
						  break;
						  default:break;
					   }
					}
                    rx_count = 0;
					DEVICE_VarInit(device_decode);
                    *cmd_status = eFrameHeader;
                }
             break;
        }
    }
}




