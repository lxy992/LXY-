
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
    device_id->ucCmdType = 0;
    for(u8 i = 0; i < 20; i++)
    {
        device_id->ucDataField[i] = 0x00;
    }
}

/***********************************
函数功能：设备读取数据
输入参数：串口名称，变量地址
输出参数：无
备注：设备读取数据
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
函数功能：设备写数据
输入参数：串口名称，变量地址,写入数据
输出参数：无
备注：设备读取数据
************************************/
void DeviceWriteData(USART_TypeDef* USARTx, u16 temp_addr,u8 *data)
{
    u8  send_data[10];
    u16 temp_crc16 = 0;

    send_data[0] = 0x5A;
    send_data[1] = 0xA5;
    send_data[2] = 0x05;
	send_data[3] = 0x82;
    send_data[4] = (temp_addr>>8);
	send_data[5] = (u8)(temp_addr&0x00ff);
    send_data[6] = data[0];
	send_data[7] = data[1];
    UART_SendData(USARTx,&send_data[0],8);
}

/***********************************
函数功能：检查数据正确性
输入参数：输入数据、最小数据、最大数据
输出参数：核查结果
备注：核查数据
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
函数功能：设备解码
输入参数：设备类型，状态
输出参数：无
备注：设备解码
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
                    if(device_decode->uiFrameHeader == 0X5A00)
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
                    if(device_decode->uiFrameHeader  == 0x5AA5)
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
            case eDataLength: 
                device_decode->ucDataLength = UART_GetByte(device_id);
                *cmd_status = eCmdType;
            break;
            case eCmdType: 
                device_decode->ucCmdType = UART_GetByte(device_id);
                *cmd_status = eDataField;
            break;
            case eDataField: /*******数据域******/
                device_decode->ucDataField[rx_count++] = UART_GetByte(device_id);
                if(rx_count >= device_decode->ucDataLength)
                {
                    rx_count = 0;
                    switch(device)
                    {
                       case  DWIN:
					     DWIN_MessageHandle(USART1,device_decode,device_decode->ucDataLength,&(device_decode->ucDataField[0]));	
					   break;
					   default:break;
				    }
					DEVICE_VarInit(device_decode);
                    *cmd_status = eFrameHeader;
                }
            break;
        }
    }
}




