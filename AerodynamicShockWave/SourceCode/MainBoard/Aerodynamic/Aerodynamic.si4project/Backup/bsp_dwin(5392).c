
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
	u8 bar_grade = 0;
	
    for(u8 i = 0; i < data_length; i++)
    {
        tmparr[i] = actual_data[i];
    }

    switch(device_decode->ucCmdType)
    {
        case SEND_CMD://����ָ��
            switch(tmparr[0])
            {
				case  SYSTEM_RUN_PAUSE_CMD: //ϵͳ���к���ͣ
                    data_check_flag = CheckData(tmparr[1],0,1);
					if(data_check_flag)
					{
	                   SystemParams.ucDwinStartFlag = tmparr[1];
					}
                break; 	
			    case SET_PRESCRIPTION_CMD: /**����**/
					SystemParams.ucPrescription = tmparr[1];
				break;
				case SET_FRQ_CMD:  //����Ƶ��
				    data_check_flag = CheckData(tmparr[1],1,17);
					if(data_check_flag)
					{
                       SystemParams.ucFrqMode = tmparr[1];
					}
				break;
				case SET_PRESSURE_CMD:  //������ѹ
				    bar_grade = tmparr[1];    /*******1bar=100KPa*******/
					SystemParams.uiSetPressureValue = bar_grade*10;
				break;
				case SET_CNT_CMD:        //���ô���
				    set_count_buf = tmparr[1];       
                    set_count_buf <<= 8;
                    set_count_buf |= tmparr[2];
					SystemParams.uiSetCount = set_count_buf;
				break;
                default:break;   
            }
        break;
        default:break;
    }
}



