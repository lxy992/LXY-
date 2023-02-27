
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
	
	
	u8 key_id_temp[2];
	
    for(u8 i = 0; i < data_length; i++)
    {
        tmparr[i] = actual_data[i];
    }

    switch(device_decode->ucCmdType)
    {
        case SEND_CMD://发送指令
            switch(tmparr[0])
            {
				case  SYSTEM_RUN_PAUSE_CMD: //系统运行和暂停
                    data_check_flag = CheckData(tmparr[1],0,1);
					if(data_check_flag)
					{
	                   SystemParams.ucRunStateFlag = tmparr[1];
					   SystemParams.ucAppRunPauseFlag = 1;
					}
                break; 	
				case SET_FRQ_CMD:  //设置频率
                    SystemParams.ucCurrentFlowMode = tmparr[1];
				break;
				case SET_PRESSURE_CMD:  //设置气压
				    SystemParams.uiSetPressureValue = tmparr[1];       //读出需要配置的气压值，赋值给全局流量值
                    SystemParams.uiSetPressureValue <<= 8;
                    SystemParams.uiSetPressureValue |= tmparr[2];
				break;
				case SET_CNT_CMD:        //设置次数
				break;
                default:break;   
            }
        break;
        default:break;
    }
}



