
#include "config.h"

/***********************************
函数功能：设备缓存变量初始化
输入参数：设备名称
输出参数：无
备注：设备缓存变量初始化
************************************/
void DEVICE_VarInit(DeviceReceiveData_t *device_id)
{
    device_id->uiFrameHeader = 0; 
    device_id->ucDataLength = 0;
    device_id->ucDeviceID = 0;
    device_id->ucCmdType = 0;
    device_id->ucCmdStatu = 0;
    for(u8 i = 0; i < 20; i++)
    {
        device_id->ucDataField[i] = 0x00;
    }
}

/***********************************
函数功能：设备读取数据
输入参数：串口名称，变量地址
输出参数：无
备注：发送数据包到设备
************************************/
void DeviceReadData(USART_TypeDef* USARTx, u16 temp_addr)
{
    u8  send_data[10];
    u16 temp_crc16 = 0;

    send_data[0] = 0x5A;
    send_data[1] = 0xA5;
    send_data[2] = 0x04;
	send_data[3] = 0x83;
    send_data[4] = (temp_addr>>8);
	send_data[5] = (u8)(temp_addr&0x00ff);
    send_data[6] = 0x01;
    UART_SendData(USARTx,&send_data[0],7);
}

/***********************************
鍑芥暟鍔熻兘锛氭牳鏌ユ暟鎹?
杈撳叆鍙傛暟锛氳緭鍏ユ暟鎹紝鏈?灏忓?硷紝鏈?澶у??
杈撳嚭鍙傛暟锛氭帴鏀剁姸鎬?
澶?   娉細鏍告煡鏁版嵁
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
鍑芥暟鍔熻兘锛氳澶囪В鐮?
杈撳叆鍙傛暟锛氳澶囧悕绉?  瑙ｇ爜鏁版嵁鐘舵??
杈撳嚭鍙傛暟锛氭棤
澶?   娉細璁惧瑙ｇ爜
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
            case eDataLength: //鏁版嵁闀垮害
                device_decode->ucDataLength = UART_GetByte(device_id);
                *cmd_status = eDeviceID;
                device_decode->ucDataField[0] = device_decode->ucDataLength;
            break;
            case eDeviceID:  //璁惧ID鍙?
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
            case eCmdType: //鍛戒护绫诲瀷
                device_decode->ucCmdType = UART_GetByte(device_id);
                *cmd_status = eCmdStatu;
                device_decode->ucDataField[3] = device_decode->ucCmdType;
            break;
            case eCmdStatu: //鍛戒护鐘舵??
                device_decode->ucCmdStatu = UART_GetByte(device_id);
                *cmd_status = eDataField;
                device_decode->ucDataField[4] = device_decode->ucCmdStatu;
            break;
            case eDataField: //鏁版嵁鍩?
                rx_count++;
                device_decode->ucDataField[rx_count + 4] = UART_GetByte(device_id);
                if(rx_count == (device_decode->ucDataLength - 4))
                {
                    rx_count = 0;
                    *cmd_status = eCrc16;
                }
            break;
            case eCrc16:  //CRC鏍￠獙
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




