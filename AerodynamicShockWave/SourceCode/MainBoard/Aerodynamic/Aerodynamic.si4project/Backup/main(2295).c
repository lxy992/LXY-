
/********************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  *****************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "config.h"

//����ֱ�
TaskHandle_t StartTask_Handler;
//������
TaskHandle_t KeyTask_Handler;
//������
TaskHandle_t ComTask_Handler;
//������
TaskHandle_t MainTask_Handler;
//�����
TaskHandle_t DetectTask_Handler;
//Debug���
TaskHandle_t DebugTask_Handler;

//������
void start_task(void *pvParameters);
//������
void Detect_task(void *pvParameters);
//������
void key_task(void *pvParameters);
//������
void com_task(void *pvParameters);
//������
void main_task(void *pvParameters);
//����������M
void debug_task(void *pvParameters);


/******************************************************************************
* Function Name  : void main(void)
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

int main(void)
{
      
    __set_PRIMASK(1);
	
    SystemHardInit(); 
	
    SystemDeviceInit();  
	
    SystemSoftInit();  
    
   //������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )128,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )1,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������                
    vTaskStartScheduler();          //��ʼ�������   
}

/*******************************************************************************
* Function Name  : start_task
* Description    : create project all task function 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();        //�����ٽ���

	//������������
    xTaskCreate((TaskFunction_t )key_task,     	
                (const char*    )"key_task",   	
                (uint16_t       )128, 
                (void*          )NULL,				
                (UBaseType_t    )4,	
                (TaskHandle_t*  )&KeyTask_Handler);  

	//�����������
    xTaskCreate((TaskFunction_t )Detect_task,     	
                (const char*    )"Detect_task",   	
                (uint16_t       )128, 
                (void*          )NULL,				
                (UBaseType_t    )5,	
                (TaskHandle_t*  )&DetectTask_Handler);   
    //����ͨ������
    xTaskCreate((TaskFunction_t )com_task,     
                (const char*    )"com_task",   
                (uint16_t       )128, 
                (void*          )NULL,
                (UBaseType_t    )6,
                (TaskHandle_t*  )&ComTask_Handler);
	//����������
    xTaskCreate((TaskFunction_t )main_task,     
                (const char*    )"main_task",   
                (uint16_t       )128, 
                (void*          )NULL,
                (UBaseType_t    )3,
                (TaskHandle_t*  )&MainTask_Handler);
	//������������
    xTaskCreate((TaskFunction_t )debug_task,     
                (const char*    )"debug_task",   
                (uint16_t       )128, 
                (void*          )NULL,
                (UBaseType_t    )2,
                (TaskHandle_t*  )&DebugTask_Handler);
	
	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
	
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

u16 time_out[5];

//key����
void key_task(void *pvParameters)
{
    while(1)
    {
	   KEY_Handle();
       vTaskDelay(20);
       time_out[0]++;
    }
}

//�������
void Detect_task(void *pvParameters)
{
    while(1)
    {
       ADC_SamplingAirPressure();
	   vTaskDelay(12);
       time_out[1]++;
    }
}

//ͨ������
void com_task(void *pvParameters)
{
    // u8 i;
	// u8 flash_id[12];
	// u16 id_write_flag = 0x5050;
	// u16 write_id[3];
	 //u16 read_id[3];
	 while(1)
     {
		DEVICE_MessageDecode(BLUETOOTH,&Bluetooth_COM,&Bluetooth_Code,&Bluetooth_StatusParams.ucCmdStatu);   
	    vTaskDelay(1);
        time_out[2]++;
     }
}

//������
void main_task(void *pvParameters)
{
    while(1)
    {
	   SystemWorkState(SystemParams.ucPowerStateFlag); 
	   vTaskDelay(25);
       time_out[3]++;
    } 
}

//����
void encryption_calculate(void)
{
    
	uint8_t key_id[2]={0x00,0x05};
	uint8_t rand_id[2];
	uint8_t new_key[4];
	
    uint8_t rand_id_temp[2];
	uint8_t key_id_temp[2];
	uint8_t send_code[5];
		
    rand_id[0] = rand()%256;
	rand_id[1] = rand()%256;
    
	new_key[0]=((rand_id[0]^0xab)&0xf0)^(key_id[0]);
	new_key[1]=((rand_id[1]^0xab)&0x0f)^(key_id[1]);
	new_key[2]=(rand_id[0]^0xcd);
	new_key[3]=(rand_id[1]^0xcd);

	rand_id_temp[0] = new_key[2]^0xcd;
	rand_id_temp[1] = new_key[3]^0xcd;
    
	key_id_temp[0]  = ((rand_id_temp[0]^0xab)&0xf0)^new_key[0];
	key_id_temp[1] = ((rand_id_temp[1]^0xab)&0x0f)^new_key[1];

    send_code[0] = 0xFE;
	send_code[1] = new_key[0];
	send_code[2] = new_key[1];
	send_code[3] = new_key[2];
	send_code[4] = new_key[3];

	SendDatPacketToDevice(USART2,5,0x44,0x02,send_code);
	
	#if 0
	printf("rand_id[0]:%d rand_id[1]:%d\r\n",rand_id[0],rand_id[1]);
	printf("\r\n");
	printf("\r\n");
	printf("new_key[0]:%d  new_key[1]:%d new_key[2]:%d  new_key[3]:%d\r\n",new_key[0],new_key[1],new_key[2],new_key[3]);
	printf("\r\n");
	printf("\r\n");
	printf("rand_id_temp[0]:%d  rand_id_temp[1]:%d key_id_temp[0]:%d  key_id_temp[1]:%d\r\n",rand_id_temp[0],rand_id_temp[1],key_id_temp[0],key_id_temp[1]);
	printf("\r\n");
	printf("\r\n");
	#endif
	
}


//��������
void debug_task(void *pvParameters)
{
    float master_pressure_value = 0;
	float slave_pressure_value = 0.0f;
	float set_slave_pressure_value = 0.0f;
	u8  step = 0;
	u8  time_out = 0;
	u32 time_count = 0;
	
	while(1)
    {

	   #if MAIN_DEBUG_WINDOW
	   
	   printf("+-----------------------------------+\r\n");

	   if(SystemParams.ucPowerStateFlag)
	   {
          printf("��Դ״̬:����\r\n");
	   }
	   else
	   {
          printf("��Դ״̬:�ػ�\r\n");
	   }

	   printf("������ѹ��ѹֵ:%dmv\r\n",SystemParams.uiMasterAirCylinder);
	   printf("������ѹ��ѹֵ:%dmv\r\n",SystemParams.uiSlaveAirCylinder);
	   printf("������ѹ��ѹֵ:%dmv\r\n",SystemParams.uiFlowAirCylinder);

	   //y=6.48x-103  x��ѹֵ    y��ѹֵ
       if(SystemParams.uiMasterAirCylinder != 0)
	   {
           master_pressure_value = (float)((float)(SystemParams.uiMasterAirCylinder+103)/6.48); 
	   }
       else
       {
           master_pressure_value = 0; 
	   }

	   SystemParams.uiMasterPressure = (u16)master_pressure_value;
	   
	   printf("������ѹֵ:%.1fKpa\r\n",master_pressure_value);
	   
       #if PRESSURE_SENSOR_4250
	   //y=19.3x-115  x��ѹֵ    y��ѹֵ 
	   if(SystemParams.uiSlaveAirCylinder != 0)
	   {
           slave_pressure_value = (float)((float)(SystemParams.uiSlaveAirCylinder+115)/19.3); 
	   }
       else
       {
           slave_pressure_value = 0; 
	   }
       #else
       //y=6.48x-103  x��ѹֵ    y��ѹֵ
       if(SystemParams.uiSlaveAirCylinder != 0)
	   {
           slave_pressure_value = (float)((float)(SystemParams.uiSlaveAirCylinder+103)/6.48); 
	   }
       else
       {
           slave_pressure_value = 0; 
	   }


	   #endif

	   SystemParams.uiSlavePressure = (u16)slave_pressure_value;
	   
	   printf("������ѹֵ:%.1fKpa\r\n",slave_pressure_value);
	
	   
	   printf("����ģʽ:%d\r\n",SystemParams.ucCurrentFlowMode);
	   printf("���ñȽ�������ѹֵ:%d\r\n",SystemParams.uiSetFlowPressureValue);
	   printf("����ģʽ:%d\r\n",SystemParams.ucCurrentTaktMode);
	   printf("���ñȽ���ѹADCֵ:%d\r\n",SystemParams.uiSetPressureValue);
	   
	   //y=19.3x-115  x��ѹֵ    y��ѹֵ 
	   if(SystemParams.uiSetPressureValue != 0)
	   {
           set_slave_pressure_value = (float)((float)(SystemParams.uiSetPressureValue+115)/19.3); 
	   }
       else
       {
           set_slave_pressure_value = 0; 
	   }
	   
	   printf("���ñȽ���ѹֵ:%.1fKpa\r\n",set_slave_pressure_value);
	   printf("������ѹ����:%d\r\n",SystemParams.ucLimitPressureSwitch);
	   printf("������ѹ�Ƚ�ѹ��ֵ:%d\r\n",SystemParams.uiMasterCompareCylinder);

	   if(SystemParams.ucChannelAB)
	   {
          printf("����Ͱģʽ:AB����Ͱ�л�ģʽ\r\n");
	   }
	   else 
	   {
           if(SystemParams.ucChannelA)
           {
              printf("����Ͱģʽ:A����Ͱģʽ\r\n");
		   }

		   if(SystemParams.ucChannelB)
		   {
              printf("����Ͱģʽ:B����Ͱģʽ\r\n");
		   }
	   }
	   
	   if(SystemParams.ucTemperatureStateFlag)
	   {
           printf("��ȡ�¶���������\r\n"); 
		   printf("�¶�ֵ:%.1f\r\n",SystemParams.fTemperatureValue);
		   
	   }
	   else
	   {
           printf("��ȡ�¶������쳣\r\n"); 
	   }     

	   printf("�����ߵ�ƽʱ��:%d\r\n",SystemParams.uiFlowPulseWidth);

	   if(SystemParams.ucOverTemperatureProtectBuffer)
	   {
           printf("���±���\r\n"); 
	   }
	   
       printf("+-----------------------------------+\r\n");
	   
       #endif

	   BLUETOOTH_MacAddressSet();
	   
	   #if BT_NAME 

	   if(SystemParams.ucPowerStateFlag)
	   {
          BLUETOOTH_NameChange();
	   } 
	   
	   #endif

	   #if BT_HANDSHAKE

	   if((SystemParams.ucPowerStateFlag == 1)&&(SystemParams.ucHandShakeStatusFlag == 0))
	   {
          encryption_calculate();
	   }
	    
	   #endif
	   
       NTC_TempHandle(&SystemParams.ucTemperature1Value,&SystemParams.ucTemperature2Value);
	   

	   if(SystemParams.ucAgingTestMode == 1)
	   {
           switch(step)
           {
              case 0:
			  SystemParams.ucAppStartupFlag = 1;
			  step = 1;
			  time_out = 10;
			  break;			  
			  case 1:
			  if(time_out !=0)break;
			  SystemParams.uiSetPressureValue = 2315;
			  SystemParams.ucCurrentFlowModeBuffer = 4;
			  SystemParams.ucCurrentTaktModeBuffer = 1;
			  step = 2;
			  time_out = 100;
			  break;
			  case 2:
			  if(time_out !=0)break;//1	5	5	5	5	2	3	0	1	5
			  SystemParams.ucCurrentFlowModeBuffer = 4;
			  SystemParams.ucCurrentTaktModeBuffer = 5; 
			  step = 3;
			  time_out = 100;
			  break;
			  case 3:
			  if(time_out !=0)break;//1	5	5	5	5	2	3	0	1	5
			  SystemParams.ucCurrentFlowModeBuffer = 3;
			  SystemParams.ucCurrentTaktModeBuffer = 5; 
			  step = 4;
			  time_out = 100;	
			  break;
			  case 4:
			  if(time_out !=0)break;//1	5	5	5	5	2	3	0	1	5
			  SystemParams.ucCurrentFlowModeBuffer = 4;
			  SystemParams.ucCurrentTaktModeBuffer = 5; 
			  step = 5;
			  time_out = 100;	
			  break;
			  case 5:
			  if(time_out !=0)break;//1	5	5	5	5	2	3	0	1	5
			  SystemParams.ucCurrentFlowModeBuffer = 3;
			  SystemParams.ucCurrentTaktModeBuffer = 5; 
			  step = 6;
			  time_out = 100;	
			  break;
			  case 6:
			  if(time_out !=0)break;//1	5	5	5	5	2	3	0	1	5
			  SystemParams.ucCurrentFlowModeBuffer = 4;
			  SystemParams.ucCurrentTaktModeBuffer = 2; 
			  step = 7;
			  time_out = 100;	
			  break;
			  case 7:
			  if(time_out !=0)break;//1	5	5	5	5	2	3	0	1	5
			  SystemParams.ucCurrentFlowModeBuffer = 4;
			  SystemParams.ucCurrentTaktModeBuffer = 3; 
			  step = 8;
			  time_out = 100;	
			  break;
			  case 8:
			  if(time_out !=0)break;//1	5	5	5	5	2	3	0	1	5
			  SystemParams.ucCurrentFlowModeBuffer = 5;
			  SystemParams.ucCurrentTaktModeBuffer = 0; 
			  step = 9;
			  time_out = 10;	
			  break;
			  case 9:
			  if(time_out !=0)break;//1	5	5	5	5	2	3	0	1	5
			  SystemParams.ucCurrentFlowModeBuffer = 4;
			  SystemParams.ucCurrentTaktModeBuffer = 1; 
			  step = 10;
			  time_out = 100;	
			  break;
			  case 10:
			  if(time_out !=0)break;//1	5	5	5	5	2	3	0	1	5
			  SystemParams.ucCurrentFlowModeBuffer = 4;
			  SystemParams.ucCurrentTaktModeBuffer = 5; 
			  step = 1;
			  time_out = 100;	
			  break;
			  case 11:
			  if(time_out != 0)break;
			  step = 0;
			  break;
		   }

		   time_count++;
		   if(time_count%1700 == 0)
		   {
              step = 11;
			  SystemParams.ucAppRunPauseFlag = 1;
			  SystemParams.ucRunStateFlag = 0;
			  time_out = 44;
		   }

		   if(time_count>=61500)
		   {
              SystemParams.ucAgingTestMode = 2;
			  SystemParams.ucAppRunPauseFlag = 1;
			  SystemParams.ucRunStateFlag = 0;
		   }
	   }
       else
       {
          step = 0; time_count = 0;
		  if(SystemParams.ucAgingTestMode == 2)
		  {
             LED_ORANGE_ON();
		     LED_BLUE_ON();
			 SystemParams.uiSetPressureValue = 0; 
			 SystemParams.ucCurrentTaktModeBuffer = 0; 
			 SystemParams.ucAgingTestMode = 0;
		  }
	   }
	   
	   if(time_out>0)
	   {
          time_out--;  
	   }
	   
	   vTaskDelay(700);
	  
    } 
} 
