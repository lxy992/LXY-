
#include "config.h"

UsartReceiveData_t  DWIN_COM;
DeviceReceiveData_t DWIN_Code;

DWIN_StatusParams_t  DWIN_StatusParams = DWIN_StatusParamsDefaults;

/***********************************
�������ܣ�������Ϣ����
����������������ƣ��豸�����룬���ݳ��ȣ�����ֵ
�����������
��    ע�� DWIN��Ϣ����
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
        case SEND_CMD://����ָ��
            switch(tmparr[0])
            {
                case  SYSTEM_ON_OFF_CMD: //��Դ���ػ�
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
				case  SYSTEM_RUN_PAUSE_CMD: //ϵͳ���к���ͣ
                    data_check_flag = CheckData(tmparr[1],0,1);
					if(data_check_flag)
					{
	                   SystemParams.ucRunStateFlag = tmparr[1];
					   SystemParams.ucAppRunPauseFlag = 1;
					}
                    SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,data_check_flag,tmparr);  
                break; 	
				case SET_FLOW_CMD:  //��������ģʽ
                    SystemParams.ucCurrentFlowMode = tmparr[1];
				    Flow_Configuration(SystemParams.ucCurrentFlowMode); //��������
					SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr); 
				break;
				case SET_COMPARE_FLOW_CMD:  //���ñȽ�����ֵ
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
				case SET_TAKT_CMD:        //���ý���
                   data_check_flag = CheckData(tmparr[1],0,11);
				   if(data_check_flag)
				   {
                       SystemParams.ucCurrentTaktMode = tmparr[1];
					   Takt_Configuration(SystemParams.ucCurrentTaktMode);
					   //SystemParams.ucCurrentTaktModeBuffer = tmparr[1];
				   }
				   SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,data_check_flag,tmparr);   
				break;
				case SET_COMPARE_PRESSURE_CMD:   //���ñȽ���ѹֵ
				    SystemParams.uiSetPressureValue = tmparr[1];       //������Ҫ���õ���ѹֵ����ֵ��ȫ������ֵ
                    SystemParams.uiSetPressureValue <<= 8;
                    SystemParams.uiSetPressureValue |= tmparr[2];
				    SendDatPacketToDevice(USARTx,3,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);   
				break;
				case OVER_TEMPERATURE_PROPECT: //���±���
                   SystemParams.ucOverTemperatureProtectFlag = 1;
				   SendDatPacketToDevice(USARTx,1,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);   
				break;
				case MASTER_CYLINDER_PRESSURE:  //������ѹ�Ƚ�ѹ��ֵ
                    SystemParams.uiMasterCompareCylinder = tmparr[1];
					SystemParams.uiMasterCompareCylinder <<= 8;
                    SystemParams.uiMasterCompareCylinder |= tmparr[2];
				   SendDatPacketToDevice(USARTx,3,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);   
				break;
				case AUTO_PRESSURE_SWITCH:   //�Զ���ѹ����
                    switch(tmparr[1]) //������Ҫ���õĿ���ֵ����ֵ��ȫ������ֵ
				    {
				        case 0x00: SystemParams.ucLimitPressureSwitch=0; MasterAirCylinder_Switch(DISABLE); break; //��  ���Զ���ѹ��ͬʱ�رյ�ŷ�
				        case 0x01: SystemParams.ucLimitPressureSwitch=1; MasterAirCylinder_Switch(DISABLE); break; //�� �����Զ���ѹ��ͬʱ�رյ�ŷ�
				        case 0x02: SystemParams.ucLimitPressureSwitch=0; MasterAirCylinder_Switch(ENABLE); break;  //���� ���Զ���ѹ��ͬʱ������ŷ�������
				        default: SystemParams.ucLimitPressureSwitch=1; MasterAirCylinder_Switch(DISABLE); break;   //�� �����Զ���ѹ��ͬʱ�رյ�ŷ�
				    }			
				   SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);   
				break;
				case CHANNEL_SWITCH:      //����Ͱ��ͨ������ѡ��
				    SystemParams.ucChannelAB = tmparr[1]; //������Ҫ���õ�ͨ������ֵ����ֵ��ȫ��ֵ
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
				case FLOW_HIGH_TIME:    //�����ߵ�ƽʱ��
                    SystemParams.uiFlowPulseWidth = tmparr[1];
				    Flow_PulseWidth(SystemParams.uiFlowPulseWidth); //��������
				    SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);   
				break;
                default:break;   
            }
        break;
        case SEND_DAT_STATU://�������ݻ�״̬
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
        case READ_DAT_STATU: //��ȡ���ݻ�״̬ 
            switch(tmparr[0])
            {
               case READ_RUN_PAUSE:             //��������ͣ
               tmparr[1] = SystemParams.ucRunStateFlag; 
               SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_SET_FLOW:              //�����õ�����ģʽ
               tmparr[1] = SystemParams.ucCurrentFlowMode; 
               SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_SET_COMPARE_FLOW:      //�����õıȽ�����ֵ
               tmparr[1] = SystemParams.uiSetFlowPressureValue>>8;
			   tmparr[2] = SystemParams.uiSetFlowPressureValue;
               SendDatPacketToDevice(USARTx,3,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_SET_TAKT:               //�����õĽ���ģʽ
               tmparr[1] = SystemParams.ucCurrentTaktMode; 
               SendDatPacketToDevice(USARTx,2,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_SET_COMPARE_PRESSURE:   //�����ñȽ�ѹ��ֵ
               tmparr[1] = SystemParams.uiSetPressureValue>>8;
			   tmparr[2] = SystemParams.uiSetPressureValue;
               SendDatPacketToDevice(USARTx,3,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_CURRENT_PRESSURE:       //��ʵʱ��ѹֵ
               tmparr[1] = SystemParams.uiMasterAirCylinder>>8; 
			   tmparr[2] = SystemParams.uiMasterAirCylinder;
			   tmparr[3] = SystemParams.uiSlaveAirCylinder>>8; 
			   tmparr[4] = SystemParams.uiSlaveAirCylinder; 
			   tmparr[5] = SystemParams.uiFlowAirCylinder>>8; 
			   tmparr[6] = SystemParams.uiFlowAirCylinder; 
               SendDatPacketToDevice(USARTx,7,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_TEMPERATURE:           //���¶�ֵ
			     /* tmparr[1] = (SystemParams.ucTemperatureStateFlag == 1)?0:1;
			   tmparr[2] = (SystemParams.fTemperatureValue>0)?0x2b:0x2d;
			   tmparr[3] = ((u16)(SystemParams.fTemperatureValue*10))%10;
			   tmparr[4] = ((u16)(SystemParams.fTemperatureValue*10))/10; */
			   tmparr[1] = (u8)SystemParams.fTemperatureValue;
			   tmparr[2] = SystemParams.ucTemperature1Value;
			   tmparr[3] = SystemParams.ucTemperature2Value;
               SendDatPacketToDevice(USARTx,4,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_MASTER_CYLINDER_LIMIT_PRESSURE: //����������ѹֵ
               tmparr[1] = SystemParams.uiMasterCompareCylinder>>8;
			   tmparr[2] = SystemParams.uiMasterCompareCylinder;
               SendDatPacketToDevice(USARTx,3,device_decode->ucCmdType,RECEIVE_DAT_SUCCESS,tmparr);
               break;
			   case READ_FLOW_HIGH_TIME:        //�������ߵ�ƽʱ��ֵ
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



