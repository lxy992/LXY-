
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
                case  SYSTEM_ON_OFF_CMD: //电源开关机
				 data_check_flag = CheckData(tmparr[1],0,1);
				 if(data_check_flag)
				 {
					if(tmparr[1] == 0x01)
                    {
			            SystemParams.ucAppStartupFlag = 1;
					}
					else
					{
						SystemParams.ucPowerStateFlag = SYSTEM_POWER_OFF; 
					}
				 }
                SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,data_check_flag,tmparr);  	
				break;
				case  SYSTEM_RUN_PAUSE_CMD: //系统运行和暂停
                    data_check_flag = CheckData(tmparr[1],0,1);
					if(data_check_flag)
					{
	                   SystemParams.ucRunStateFlag = tmparr[1];
					   SystemParams.ucAppRunPauseFlag = 1;
					}
                    SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,data_check_flag,tmparr);  
                break; 	
				case SET_FLOW_CMD:  //设置流量模式
                    SystemParams.ucCurrentFlowMode = tmparr[1];
				    Flow_Configuration(SystemParams.ucCurrentFlowMode); //配置流量
					SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr); 
				break;
				case SET_COMPARE_FLOW_CMD:  //设置比较流量值
				   SystemParams.uiSetFlowPressureValue = tmparr[1];   
				   SystemParams.uiSetFlowPressureValue <<= 8;
				   SystemParams.uiSetFlowPressureValue |= tmparr[2];
				   if(SystemParams.uiSetFlowPressureValue == 0)
				   {
                       Flow_Init(); 
				   }
                   else
                   {
                        if(SystemParams.uiSetFlowPressureValue<=1130)
					    {
					        SystemParams.uiFlowPulseWidth = 140;
					    }
					    else if((SystemParams.uiSetFlowPressureValue>1130)&&(SystemParams.uiSetFlowPressureValue<=1227))
					    {
					        SystemParams.uiFlowPulseWidth = 143;
					    }
					    else if((SystemParams.uiSetFlowPressureValue>1227)&&(SystemParams.uiSetFlowPressureValue<=1596))
					    {
					        SystemParams.uiFlowPulseWidth = 155;
					    }
					    else if(SystemParams.uiSetFlowPressureValue>1596)
					    {
					        SystemParams.uiFlowPulseWidth = 165;
					    }
					    Flow_Custom(SystemParams.uiFlowPulseWidth);
				   }
				   
				   SendDatPacketToDevice(USARTx,3,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);   
				break;
				case SET_TAKT_CMD:        //设置节拍
                   data_check_flag = CheckData(tmparr[1],0,11);
				   if(data_check_flag)
				   {
                       SystemParams.ucCurrentTaktMode = tmparr[1];
					   Takt_Configuration(SystemParams.ucCurrentTaktMode);
					   //SystemParams.ucCurrentTaktModeBuffer = tmparr[1];
				   }
				   SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,data_check_flag,tmparr);   
				break;
				case SET_COMPARE_PRESSURE_CMD:   //设置比较气压值
				    SystemParams.uiSetPressureValue = tmparr[1];       //读出需要配置的气压值，赋值给全局流量值
                    SystemParams.uiSetPressureValue <<= 8;
                    SystemParams.uiSetPressureValue |= tmparr[2];
				    SendDatPacketToDevice(USARTx,3,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);   
				break;
				case OVER_TEMPERATURE_PROPECT: //过温保护
                   SystemParams.ucOverTemperatureProtectFlag = 1;
				   SendDatPacketToDevice(USARTx,1,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);   
				break;
				case MASTER_CYLINDER_PRESSURE:  //主缸限压比较压力值
                    SystemParams.uiMasterCompareCylinder = tmparr[1];
					SystemParams.uiMasterCompareCylinder <<= 8;
                    SystemParams.uiMasterCompareCylinder |= tmparr[2];
				   SendDatPacketToDevice(USARTx,3,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);   
				break;
				case AUTO_PRESSURE_SWITCH:   //自动限压开关
                    switch(tmparr[1]) //读出需要配置的开关值，赋值给全局流量值
				    {
				        case 0x00: SystemParams.ucLimitPressureSwitch=0; MasterAirCylinder_Switch(DISABLE); break; //关  关自动限压，同时关闭电磁阀
				        case 0x01: SystemParams.ucLimitPressureSwitch=1; MasterAirCylinder_Switch(DISABLE); break; //开 开启自动限压，同时关闭电磁阀
				        case 0x02: SystemParams.ucLimitPressureSwitch=0; MasterAirCylinder_Switch(ENABLE); break;  //排气 关自动限压，同时开启电磁阀，排气
				        default: SystemParams.ucLimitPressureSwitch=1; MasterAirCylinder_Switch(DISABLE); break;   //开 开启自动限压，同时关闭电磁阀
				    }			
				   SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);   
				break;
				case CHANNEL_SWITCH:      //过滤桶的通道开关选择
				    SystemParams.ucChannelAB = tmparr[1]; //读出需要配置的通道开关值，赋值给全局值
				    SystemParams.ucChannelA = tmparr[2]; 
				    SystemParams.ucChannelB = tmparr[3];
				    SendDatPacketToDevice(USARTx,4,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr); 
				    if(SystemParams.ucChannelAB == 1)
					{
					   SystemParams.ucFilterVatChannelABFlag = 1;
					   break;
					}
				    else
					{
					   SystemParams.ucFilterVatChannelABFlag = 0;
					  // Delay_ms(2);
				    }
					
				    if(SystemParams.ucChannelA)
				    {
				        FilterVat_ChannelASwitch(ENABLE);
				        FilterVat_ChannelBSwitch(DISABLE);
				    }
				    else
				    {
				        FilterVat_ChannelASwitch(DISABLE);
				        FilterVat_ChannelBSwitch(ENABLE);
				    }
					
				    if(SystemParams.ucChannelB)
				    {
				        FilterVat_ChannelBSwitch(ENABLE);
				        FilterVat_ChannelASwitch(DISABLE);
				    }
				    else
				    {
				        FilterVat_ChannelBSwitch(DISABLE);
				        FilterVat_ChannelASwitch(ENABLE);
				    }  
				break;
				case FLOW_HIGH_TIME:    //流量高电平时间
                    SystemParams.uiFlowPulseWidth = tmparr[1];
				    Flow_PulseWidth(SystemParams.uiFlowPulseWidth); //配置流量
				    SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);   
				break;
                default:break;   
            }
        break;
        case SEND_DAT_STATU://发送数据或状态
		   switch(tmparr[0])
           {
              case ENCRYPTION_PROTOCOL:
                    key_id_temp[0]  = (((tmparr[3]^0xcd)^0xab)&0xf0)^tmparr[1];
					key_id_temp[1] = (((tmparr[4]^0xcd)^0xab)&0x0f)^tmparr[2];
					if((key_id_temp[0] == 0x00)&&(key_id_temp[1] == 0x05))
					{
		               SystemParams.ucHandShakeStatusFlag = 1;
					}

					#if 0
					printf("\r\n");
					printf("\r\n");
					printf("tmparr[0]:%d  tmparr[1]:%d tmparr[2]:%d  tmparr[3]:%d\r\n",tmparr[0],tmparr[1],tmparr[2],tmparr[3]);
					printf("\r\n");
					printf("\r\n");
					printf("key_id_temp[0]:%d  key_id_temp[1]:%d\r\n",key_id_temp[0],key_id_temp[1]);
					printf("\r\n");
					printf("\r\n");
					#endif
			  break;	
		   }
		break;
        case READ_DAT_STATU: //读取数据或状态 
            switch(tmparr[0])
            {
               case READ_RUN_PAUSE:             //读运行暂停
               tmparr[1] = SystemParams.ucRunStateFlag; 
               SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_SET_FLOW:              //读设置的流量模式
               tmparr[1] = SystemParams.ucCurrentFlowMode; 
               SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_SET_COMPARE_FLOW:      //读设置的比较流量值
               tmparr[1] = SystemParams.uiSetFlowPressureValue>>8;
			   tmparr[2] = SystemParams.uiSetFlowPressureValue;
               SendDatPacketToDevice(USARTx,3,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_SET_TAKT:               //读设置的节拍模式
               tmparr[1] = SystemParams.ucCurrentTaktMode; 
               SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_SET_COMPARE_PRESSURE:   //读设置比较压力值
               tmparr[1] = SystemParams.uiSetPressureValue>>8;
			   tmparr[2] = SystemParams.uiSetPressureValue;
               SendDatPacketToDevice(USARTx,3,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_CURRENT_PRESSURE:       //读实时气压值
               tmparr[1] = SystemParams.uiMasterAirCylinder>>8; 
			   tmparr[2] = SystemParams.uiMasterAirCylinder;
			   tmparr[3] = SystemParams.uiSlaveAirCylinder>>8; 
			   tmparr[4] = SystemParams.uiSlaveAirCylinder; 
			   tmparr[5] = SystemParams.uiFlowAirCylinder>>8; 
			   tmparr[6] = SystemParams.uiFlowAirCylinder; 
               SendDatPacketToDevice(USARTx,7,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_TEMPERATURE:           //读温度值
			     /* tmparr[1] = (SystemParams.ucTemperatureStateFlag == 1)?0:1;
			   tmparr[2] = (SystemParams.fTemperatureValue>0)?0x2b:0x2d;
			   tmparr[3] = ((u16)(SystemParams.fTemperatureValue*10))%10;
			   tmparr[4] = ((u16)(SystemParams.fTemperatureValue*10))/10; */
			   tmparr[1] = (u8)SystemParams.fTemperatureValue;
			   tmparr[2] = SystemParams.ucTemperature1Value;
			   tmparr[3] = SystemParams.ucTemperature2Value;
               SendDatPacketToDevice(USARTx,4,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_MASTER_CYLINDER_LIMIT_PRESSURE: //读主气缸限压值
               tmparr[1] = SystemParams.uiMasterCompareCylinder>>8;
			   tmparr[2] = SystemParams.uiMasterCompareCylinder;
               SendDatPacketToDevice(USARTx,3,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_FLOW_HIGH_TIME:        //读流量高电平时间值
               tmparr[1] = SystemParams.uiFlowPulseWidth; 
               SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_VERSION_NUM:
               tmparr[1] = VERSION_NUM; 
               SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_DEVICE_ID:
               tmparr[1] = SystemParams.ucID[0]; 
			   tmparr[2] = SystemParams.ucID[1]; 
			   tmparr[3] = SystemParams.ucID[2]; 
			   tmparr[4] = SystemParams.ucID[3]; 
			   tmparr[5] = SystemParams.ucID[4]; 
			   tmparr[6] = SystemParams.ucID[5]; 
               SendDatPacketToDevice(USARTx,7,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case  READ_SLAVE_CYLINDER_DATA:
			   tmparr[1] = (u8)(SystemParams.uiSlavePressure>>8);
			   tmparr[2] = (u8)SystemParams.uiSlavePressure;
			   SendDatPacketToDevice(USARTx,3,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);	
			   break;
            }
        break;
        default:break;
    }
}



