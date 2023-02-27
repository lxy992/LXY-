
#include "config.h"

UsartReceiveData_t  DWIN_COM;
DeviceReceiveData_t DWIN_Code;

DWIN_StatusParams_t  DWIN_StatusParams = DWIN_StatusParamsDefaults;

/***********************************
函数功能：迪文信息处理
输入参数：串口名称，设备数据码，数据长度，数据值
输出参数：无
备    注： DWIN信息处理
************************************/
void DWIN_MessageHandle(USART_TypeDef* USARTx,DeviceReceiveData_t *device_decode,u8 data_length, u8 *actual_data)
{
    u8 tmparr[10];
    u8 data_check_flag = 0;
    u16 set_count_buf = 0;
	
    for(u8 i = 0; i < data_length; i++)
    {
        tmparr[i] = actual_data[i];
    }

    switch(device_decode->ucCmdType)
    {
        case SEND_CMD://发送指令
            switch(tmparr[1])
            {
				case  SYSTEM_RUN_PAUSE_CMD: //系统运行和暂停
                    data_check_flag = CheckData(tmparr[4],0,1);
					if(data_check_flag)
					{
	                   SystemParams.ucDwinStartFlag = tmparr[4];
					}
                break; 	
			    case SET_PRESCRIPTION_CMD: /**处方**/
					SystemParams.ucPrescription = tmparr[4];
					switch(SystemParams.ucPrescription)
					{
                       case 1:
					   SystemParams.ucFrqMode = 1; SystemParams.ucSetBarValue = 20;
					   SystemParams.uiSetCount = 0;
					   break;
					   case 2:
					   SystemParams.ucFrqMode = 9; SystemParams.ucSetBarValue = 30;
					   SystemParams.uiSetCount = 10000;	
					   break;
					   case 3:
					   SystemParams.ucFrqMode = 6; SystemParams.ucSetBarValue = 20;
					   SystemParams.uiSetCount = 10000;	
					   break;
					   case 4:
					   SystemParams.ucFrqMode = 4; SystemParams.ucSetBarValue = 30;
					   SystemParams.uiSetCount = 10000;	
					   break;
					   case 5:
					   SystemParams.ucFrqMode = 8; SystemParams.ucSetBarValue = 40;
					   SystemParams.uiSetCount = 10000;		
					   break;
					   case 6:
					   SystemParams.ucFrqMode = 3; SystemParams.ucSetBarValue = 20;
					   SystemParams.uiSetCount = 10000;		
					   break;
					   case 7:
					   SystemParams.ucFrqMode = 7; SystemParams.ucSetBarValue = 25;
					   SystemParams.uiSetCount = 10000;		
					   break;
					   case 8:
					   SystemParams.ucFrqMode = 10; SystemParams.ucSetBarValue = 30;
					   SystemParams.uiSetCount = 10000;		
					   break;
					   case 9:
					   SystemParams.ucFrqMode = 1; SystemParams.ucSetBarValue = 15;
					   SystemParams.uiSetCount = 10000;		
					   break;
					}
					tmparr[2] = 0x00;tmparr[3] = SystemParams.ucFrqMode;
					tmparr[4] = 0x00;tmparr[5] = SystemParams.ucSetBarValue;
					tmparr[6] = (u8)(SystemParams.uiSetCount>>8);tmparr[7] = (u8)SystemParams.uiSetCount;
					DeviceWriteData(USART1,0x0400,&tmparr[2]);
					DeviceWriteData(USART1,0x0500,&tmparr[4]);
					DeviceWriteData(USART1,0x0600,&tmparr[6]);
					
					DeviceWriteData(USART2,0x0400,&tmparr[2]);
					DeviceWriteData(USART2,0x0500,&tmparr[4]);
					DeviceWriteData(USART2,0x0600,&tmparr[6]);

					DeviceWriteData(USART3,0x0400,&tmparr[2]);
					DeviceWriteData(USART3,0x0500,&tmparr[4]);
					DeviceWriteData(USART3,0x0600,&tmparr[6]);
				break;
				case SET_FRQ_CMD:  //设置频率
				    data_check_flag = CheckData(tmparr[4],1,17);
					if(data_check_flag)
					{
                       SystemParams.ucFrqMode = tmparr[4];
					   tmparr[2] = 0x00;tmparr[3] = SystemParams.ucFrqMode;
					   DeviceWriteData(USART2,0x0400,&tmparr[2]);
					   DeviceWriteData(USART3,0x0400,&tmparr[2]);
					}
				break;
				case SET_PRESSURE_CMD:  //设置气压
				    SystemParams.ucSetBarValue = tmparr[4];    /*******1bar=100KPa*******/
					SystemParams.uiSetPressureValue = SystemParams.ucSetBarValue*10;
				    tmparr[2] = 0x00;tmparr[3] = SystemParams.ucSetBarValue;
				    DeviceWriteData(USART2,0x0500,&tmparr[2]);
					DeviceWriteData(USART3,0x0500,&tmparr[2]);
				break;
				case SET_CNT_CMD:        //设置次数
				    set_count_buf = tmparr[3];       
                    set_count_buf <<= 8;
                    set_count_buf |= tmparr[4];
					SystemParams.uiSetCount = set_count_buf;
					tmparr[2] = tmparr[3];tmparr[3] = tmparr[4];
				    DeviceWriteData(USART2,0x0600,&tmparr[2]);
					DeviceWriteData(USART3,0x0600,&tmparr[2]);
				break;
                default:break;   
            }
        break;
        default:break;
    }
}



