
#include "config.h"

UsartReceiveData_t  LEFT_HAND_COM;
DeviceReceiveData_t LEFT_HAND_Code;

LEFT_HAND_StatusParams_t  LEFT_HAND_StatusParams = LEFT_HAND_StatusParamsDefaults;

/***********************************
函数功能：左手柄信息处理
输入参数：串口名称，设备数据码，数据长度，数据值
输出参数：无
备    注： 左手柄信息处理
************************************/
void LEFT_HAND_MessageHandle(USART_TypeDef* USARTx,DeviceReceiveData_t *device_decode,u8 data_length, u8 *actual_data)
{
    u8 tmparr[10];
    u8 data_check_flag = 0;
	u8 bar_grade = 0;
	
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
					   if(SystemParams.ucRunStateFlag)
					   {
                          if((SystemParams.ucDwinStartFlag == 0)||(SystemParams.uiSetCount == 0))break;
						  SystemParams.ucLeftHandStartWorkFlag = 1;
						  SystemParams.ucRightHandStartWorkFlag = 0;
					   }
					   else
					   {
                           SystemParams.ucLeftHandStartWorkFlag = 0;
						   SystemParams.ucRightHandStartWorkFlag = 0;
					   }
					}
                break; 	
				case SET_FRQ_CMD:  //设置频率
                    SystemParams.ucFrqMode= tmparr[1];
				break;
				case SET_PRESSURE_CMD:  //设置气压
				   bar_grade = tmparr[1];    /*******1bar=100KPa*******/
				   SystemParams.uiSetPressureValue = bar_grade*10; 
				break;
                default:break;   
            }
        break;
        default:break;
    }
}





