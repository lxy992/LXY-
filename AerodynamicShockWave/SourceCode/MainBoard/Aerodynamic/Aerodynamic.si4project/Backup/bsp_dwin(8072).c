
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
    u16 set_count_buf = 0;
	
    for(u8 i = 0; i < data_length; i++)
    {
        tmparr[i] = actual_data[i];
    }

    switch(device_decode->ucCmdType)
    {
        case SEND_CMD://����ָ��
            switch(tmparr[1])
            {
				case  SYSTEM_RUN_PAUSE_CMD: //ϵͳ���к���ͣ
                    data_check_flag = CheckData(tmparr[4],0,1);
					if(data_check_flag)
					{
	                   SystemParams.ucDwinStartFlag = tmparr[4];
					}
                break; 	
			    case SET_PRESCRIPTION_CMD: /**����**/
					SystemParams.ucPrescription = tmparr[4];
				break;
				case SET_FRQ_CMD:  //����Ƶ��
				    data_check_flag = CheckData(tmparr[4],1,17);
					if(data_check_flag)
					{
                       SystemParams.ucFrqMode = tmparr[4];
					   tmparr[2] = 0x00;tmparr[3] = SystemParams.ucFrqMode;
					   DeviceWriteData(USART2,0x0400,&tmparr[2]);
					   DeviceWriteData(USART3,0x0400,&tmparr[2]);
					}
				break;
				case SET_PRESSURE_CMD:  //������ѹ
				    SystemParams.ucSetBarValue = tmparr[4];    /*******1bar=100KPa*******/
					SystemParams.uiSetPressureValue = SystemParams.ucSetBarValue*10;
				    tmparr[2] = 0x00;tmparr[3] = SystemParams.ucSetBarValue;
				    DeviceWriteData(USART2,0x0500,&tmparr[2]);
					DeviceWriteData(USART3,0x0500,&tmparr[2]);
				break;
				case SET_CNT_CMD:        //���ô���
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



