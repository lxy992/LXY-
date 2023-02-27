
#include "config.h"

SystemParams_t SystemParams = SystemParams_Defaults;

/**** ��֤��Կ�����һЩ���� */
#define KEY_LOCATION   0x08002FE0  //��Կ�ڵ�ǰоƬ�еĴ洢��ַ
#define UID_LOCATION   0x1FFFF7E8  //��ǰоƬUID���ڵ�ַ
u8 checkResult = 0; //��֤���
u8 myID[12] = {
                0x02, 0x01, 0x03, 0x04,
                0x05, 0x06, 0x07, 0x08, 
                0x09, 0x0A, 0x0D, 0x0C};//�û��Զ���ID
		
/*******************************************************************************
* FunctionName   : Fan_Switch
* Description    : ���ȵĿ��غ���
* Notice         : NO.
* Param[in]      : key---Ϊö�����ͣ�ENABLE�������ȣ�DISABLE�رշ���
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Fan_Switch(FunctionalState NewState)//���ȿ��� 0---�رգ�1---����
{
    if(NewState)
    {
       FAN_ON();
    }
    else
    {
       FAN_OFF();  
    }
}

/*******************************************************************************
* FunctionName   : Fan_Init
* Description    : ���ȵĳ�ʼ����Ĭ�ϳ�ʼ��Ϊ�ر�״̬
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Fan_Init(void)//��ʼ������״̬
{
    Fan_Switch(ENABLE);//���ȹر�
}

/*******************************************************************************
* FunctionName   : Compressor_Switch
* Description    : ѹ�����Ŀ��غ���
* Notice         : NO.
* Param[in]      : NewState---Ϊö�����ͣ�ENABLE����ѹ������DISABLE�ر�ѹ����
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Compressor_Switch(FunctionalState NewState)//ѹ�������� 0---��ѹ������1---��ѹ����
{
    if(NewState)
    {
        COMPRESSOR_ON();
    }
    else
    {
        COMPRESSOR_OFF();
    }
}

/*******************************************************************************
* FunctionName   : Compressor_Init
* Description    : ѹ�����ĳ�ʼ����Ĭ�ϳ�ʼ��Ϊ�ر�״̬
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Compressor_Init(void)//ѹ������ʼ��
{
    Compressor_Switch(DISABLE);//ѹ�����ر�
}

/*******************************************************************************
* FunctionName   : FilterVat_ChannelASwitch
* Description    : ����Ͱͨ��A����
* Notice         : NO.
* Param[in]      : NewState---Ϊö�����ͣ�ENABLE�����̵�����DISABLE�رռ̵���
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void FilterVat_ChannelASwitch(FunctionalState NewState)//P11��Ӧ�ļ̵������� 0---�ضϣ�1---����
{
    if(NewState)
    {
       EN_PI1_ON();
    }
    else
    {
        EN_PI1_OFF();
    }
}

/*******************************************************************************
* FunctionName   : FilterVat_ChannelASwitch
* Description    : ����ͰB�л�����
* Notice         : NO.
* Param[in]      : key---Ϊö�����ͣ�ENABLE�����̵�����DISABLE�رռ̵���
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void FilterVat_ChannelBSwitch(FunctionalState NewState)//����P12��Ӧ�ļ̵���
{
    if(NewState)
    {
         EN_PI2_ON();
    }
    else
    {
         EN_PI2_OFF();
    }
}

/*******************************************************************************
* FunctionName   : FilterVat_ChannelSwtichOutput
* Description    : ����Ͱͨ���л����
* Notice         : 100ms ����һ��.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void  FilterVat_ChannelSwtichOutput(void)
{
    static u8 num = 0;
    static u8 time_count = 0;
	static u8 channel_select = 0;

	
    if(SystemParams.ucFilterVatChannelABFlag)
    {
        if(++time_count>=10)  //1S
		{
	       time_count = 0;
		}
		else
		{
	        return;
		}

		if(num == 1)
		{
           if(channel_select == 0)
	        {
	            
				FilterVat_ChannelBSwitch(DISABLE); 
		        FilterVat_ChannelASwitch(ENABLE); 
			}
			else
			{
	            FilterVat_ChannelASwitch(DISABLE); 
		        FilterVat_ChannelBSwitch(ENABLE); 
			}
		}
		
		if(++num>=FILTER_VAT_ALL_TIME)
	    {
	        if(SystemParams.ucCurrentTaktModeBuffer == 0)
	        {
	            num = 0;
				FilterVat_ChannelASwitch(ENABLE); 
		        FilterVat_ChannelBSwitch(ENABLE); 
		        channel_select = (channel_select==0)?1:0; 
	        }
			else
			{
               num = FILTER_VAT_ALL_TIME;  
			}
	    }

        if(num>1)
        {
            if(SystemParams.uiMasterAirCylinder>=2500) //400Kpa
	        {
	            num = 0;
		        channel_select = (channel_select==0)?1:0; 
	        }
		}
	
    }
	else
	{
       time_count = 0;  
	   num = 0;
	   channel_select = 0;
	}
	
}

/*******************************************************************************
* FunctionName   : Ctrl_Relay_Init
* Description    : ��ʼ��5���̵�����Ĭ�ϳ�ʼ��Ϊȫ���ر�״̬
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Relay_Init(void)//��ʼ���̵���
{
    FilterVat_ChannelASwitch(DISABLE);  
	FilterVat_ChannelBSwitch(DISABLE);
	Venting_Switch(DISABLE);        //�ر�����
    Intake_Switch(DISABLE);        //�رս���
    MasterAirCylinder_Switch(DISABLE); //��������ѹ
}

void  Delay_ms(u16 ms)
{
    u16 i,j;
	
	for(i=0;i<ms;i++)
	{
	   for(j=0;j<1000;j++)
	   {
	      Delay_us(1);
	   }
	}
}

/*******����LEDָʾ*********/
void LED_OpenLock(void)
{
   static u8  led_count = 0;
   
   if((SystemParams.ucOpenLockFlag == 1)||(SystemParams.ucAgingTestMode == 1))
   {
      if(++led_count%2)
      {
          LED_ORANGE_OFF();
	      LED_BLUE_OFF();
	  }
	  else
	  {
          LED_ORANGE_ON();
	      LED_BLUE_ON();
	  }
   }
   else
   {
      led_count = 0;
   }
}

/*******************************************************************************
* FunctionName   : SystemStartupOrShutdown
* Description    : �������߹ػ��Ĺ��̺���
* Notice         : ���ػ���������˹���
* Param[in]      : NewState---RESET/SET
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void SystemStartupOrShutdown(FunctionalState NewState)
{
	SystemParams.ucFilterVatChannelABFlag = 0;
	SystemParams.uiSetPressureValue = 0;
	SystemParams.ucCurrentTaktModeBuffer = 0;
	SystemParams.ucCurrentFlowModeBuffer = 0;
    Venting_Switch(ENABLE);        //һֱ���� Ŀ����ж������ˮ��ѹ��
    SystemParams.ucCurrentFlowMode = 4;
    Flow_Configuration(SystemParams.ucCurrentFlowMode);         //�������

	SystemParams.ucCurrentTaktMode = 0;
	Takt_Configuration(SystemParams.ucCurrentTaktMode);        //���ĵ͵�ƽ

	
    if(NewState)
    {
		if(SystemParams.uiMasterAirCylinder >= 1300)
        {
            SystemParams.ucFilterVatChannelABFlag = 0;
	        Venting_Switch(ENABLE);           //һֱ���� Ŀ����ж������ˮ��ѹ��
	        Intake_Switch(DISABLE);
	        FilterVat_ChannelASwitch(ENABLE);
	        FilterVat_ChannelBSwitch(ENABLE);  
			SystemParams.ucLimitPressureSwitch = 0;
			MasterAirCylinder_Switch(ENABLE);  
			MASTER_CYLINDER_LIMIT1_ON(); 
			Delay_ms(3000);
		}
        MASTER_CYLINDER_LIMIT1_OFF(); 
		SystemParams.ucLimitPressureSwitch = 1;
		FilterVat_ChannelBSwitch(DISABLE);
		FilterVat_ChannelASwitch(ENABLE);
        Delay_ms(100);
	    SystemParams.ucFilterVatChannelABFlag = 1;
		Fan_Switch(ENABLE);
	    #if (DAC_EN == 0)
        Compressor_Switch(ENABLE);
		#else
		SystemParams.uiSetCompressorVoltage = 2400;
		while(1)
		{
             SystemParams.uiSetCompressorVoltage+=200; 			 
			 DAC1_SetVoltage(SystemParams.uiSetCompressorVoltage); 
			 Delay_ms(500);
			 if(SystemParams.uiSetCompressorVoltage>=3000)
			 {
                break;   
			 }
		}
		#endif
    }       
    else
    {
        SystemParams.ucFilterVatChannelABFlag = 0;
        Venting_Switch(ENABLE);           //һֱ���� Ŀ����ж������ˮ��ѹ��
        Intake_Switch(DISABLE);
        FilterVat_ChannelASwitch(ENABLE);
        FilterVat_ChannelBSwitch(ENABLE);  
		SystemParams.ucLimitPressureSwitch = 0;
		MasterAirCylinder_Switch(ENABLE);  
		if(SystemParams.uiMasterAirCylinder>=1300)
		{
           Delay_ms(3000);
		}
		//Fan_Switch(DISABLE);
		#if (DAC_EN == 0)
		Compressor_Switch(DISABLE);//ѹ�����ر�
        #else 
		while(1)
		{
             if(SystemParams.uiSetCompressorVoltage>=200)
             {
                SystemParams.uiSetCompressorVoltage-=200;
			 }
			 else
			 {
                 SystemParams.uiSetCompressorVoltage = 0;
			 }
			 DAC1_SetVoltage(SystemParams.uiSetCompressorVoltage); 
			 Delay_ms(300);
			 if(SystemParams.uiSetCompressorVoltage<=1200)
			 {
                SystemParams.uiSetCompressorVoltage = 0;
				DAC1_SetVoltage(SystemParams.uiSetCompressorVoltage); 
				break;   
			 }
		}
		#endif
		
		SystemParams.uiSetPressureValue = 0;
		
        if(SystemParams.uiMasterAirCylinder>0)
        {
	        FilterVat_ChannelASwitch(ENABLE);     
	        FilterVat_ChannelBSwitch(ENABLE);
	        Delay_ms(1000);
	        Intake_Switch(DISABLE);
	        FilterVat_ChannelASwitch(DISABLE);
	        FilterVat_ChannelBSwitch(DISABLE);
	        Delay_ms(1000); 
			Venting_Switch(ENABLE); //һֱ���� Ŀ����ж������ˮ��ѹ��
	        Intake_Switch(DISABLE);
        }
    }

}

/*******************************************************************************
* FunctionName   : SystemRunOrPause(FunctionalState NewState)
* Description    : ϵͳ���л���ͣ
* Notice         : ״̬�л�������˹���
* Param[in]      : NewState---ENABLE/DISABLE
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void SystemRunOrPause(FunctionalState NewState)
{
	if(NewState)
    {
      //  WORK_LED_ON();
		SystemParams.ucFilterVatChannelABFlag = 0;
		SystemParams.ucAppStateLimitFlag = 0;
        FilterVat_ChannelASwitch(DISABLE);
        FilterVat_ChannelBSwitch(DISABLE);
		MASTER_CYLINDER_LIMIT1_OFF(); 
		SystemParams.ucLimitPressureSwitch = 1;
        Delay_ms(1000);
        FilterVat_ChannelASwitch(ENABLE);
        SystemParams.ucFilterVatChannelABFlag = 1;
		Fan_Switch(ENABLE);
		#if (DAC_EN == 0)
        Compressor_Switch(ENABLE);
	    #else
		SystemParams.uiSetCompressorVoltage = 2400;
		while(1)
		{
             SystemParams.uiSetCompressorVoltage+=200; 			 
			 DAC1_SetVoltage(SystemParams.uiSetCompressorVoltage); 
			 Delay_ms(500);
			 if(SystemParams.uiSetCompressorVoltage>=3200)
			 {
                break;   
			 }
		}
		#endif
    }
    else
    {
        WORK_LED_OFF();
		SystemParams.ucFilterVatChannelABFlag = 0;
		SystemParams.uiSetPressureValue = 0;
		SystemParams.ucCurrentTaktModeBuffer = 0;
	    SystemParams.ucCurrentFlowModeBuffer = 0;
		SystemParams.ucCurrentFlowMode = 4;
		Flow_Configuration(SystemParams.ucCurrentFlowMode);     //�������
		SystemParams.ucCurrentTaktMode = 0;
		Takt_Configuration(SystemParams.ucCurrentTaktMode);    //���ĵ͵�ƽ
        Venting_Switch(ENABLE);                                //һֱ���� Ŀ����ж������ˮ��ѹ��
        Intake_Switch(DISABLE);
        FilterVat_ChannelASwitch(ENABLE);
        FilterVat_ChannelBSwitch(ENABLE);  
		SystemParams.ucLimitPressureSwitch = 0;
		MasterAirCylinder_Switch(ENABLE);  
		MASTER_CYLINDER_LIMIT1_ON(); 
        Delay_ms(3000);
		//Fan_Switch(DISABLE);
		#if (DAC_EN == 0)
		   Compressor_Switch(DISABLE);
		#else
		  while(1)
		  {
	         if(SystemParams.uiSetCompressorVoltage>=200)
             {
                SystemParams.uiSetCompressorVoltage-=200;
			 }
			 else
			 {
                 SystemParams.uiSetCompressorVoltage = 0;
			 }		 
			 DAC1_SetVoltage(SystemParams.uiSetCompressorVoltage); 
			 Delay_ms(300);
			 if(SystemParams.uiSetCompressorVoltage<=1200)
			 {
                 SystemParams.uiSetCompressorVoltage = 0;
				 DAC1_SetVoltage(SystemParams.uiSetCompressorVoltage); 
				 break;   
			 }
		   }
        #endif
		if(SystemParams.ucRunStateFlag)
		{
           SystemParams.ucAppStateLimitFlag = 1;
		}
    }
}

/*******************************************************************************
* FunctionName   : SystemProtect
* Description    : ����
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void SystemProtect(void)
{

	if((SystemParams.ucOverTemperatureProtectFlag == 0)&&(SystemParams.ucUnlockFlag == 0))
    {
       return;
	}

	if(SystemParams.ucStartupFlag == 0)
	{
       return;
	}
	
    WORK_LED_OFF();
	
	if(SystemParams.ucOverTemperatureProtectFlag)
	{
      SystemParams.ucOverTemperatureProtectBuffer = SystemParams.ucOverTemperatureProtectFlag;
	}
	SystemParams.uiSetPressureValue = 0;
    SystemParams.ucFilterVatChannelABFlag = 0;
	SystemParams.ucCurrentTaktModeBuffer = 0;
    Venting_Switch(ENABLE);  //һֱ���� Ŀ����ж������ˮ��ѹ��
    SystemParams.ucCurrentFlowMode = 4;
    Flow_Configuration(SystemParams.ucCurrentFlowMode);   //�������
    SystemParams.ucCurrentTaktMode = 5;
    Takt_Configuration(SystemParams.ucCurrentTaktMode);    //���ĸߵ�ƽ  ����ʹ�ܣ��������Ŀ���ǰ���Ͳ�����ѹж��
	SystemParams.ucFilterVatChannelABFlag = 0;
    Venting_Switch(ENABLE);           //һֱ���� Ŀ����ж������ˮ��ѹ��
    Intake_Switch(DISABLE);
    FilterVat_ChannelASwitch(ENABLE);
    FilterVat_ChannelBSwitch(ENABLE);  
	SystemParams.ucLimitPressureSwitch = 0;
	MASTER_CYLINDER_LIMIT1_ON(); 
	MasterAirCylinder_Switch(ENABLE);  
	Delay_ms(3000);
	SystemParams.ucCurrentTaktMode = 0;
	Takt_Configuration(SystemParams.ucCurrentTaktMode); //���ĵ͵�ƽ
	Fan_Switch(ENABLE);
	#if (DAC_EN == 0)
	Compressor_Switch(DISABLE);//ѹ�����ر�
    #else
	while(1)
	{
         if(SystemParams.uiSetCompressorVoltage>=200)
         {
            SystemParams.uiSetCompressorVoltage-=200;
		 }
		 else
		 {
             SystemParams.uiSetCompressorVoltage = 0;
		 }			 
		 DAC1_SetVoltage(SystemParams.uiSetCompressorVoltage); 
		 Delay_ms(300);
		 if(SystemParams.uiSetCompressorVoltage<=1200)
		 {
            SystemParams.uiSetCompressorVoltage = 0;
			DAC1_SetVoltage(SystemParams.uiSetCompressorVoltage); 
			break;   
		 }
	}
	#endif
	
	SystemParams.uiSetPressureValue = 0;

	//Intake_Switch(ENABLE);                //����   
    FilterVat_ChannelASwitch(ENABLE);     
    FilterVat_ChannelBSwitch(ENABLE);
    Delay_ms(1000);
    Intake_Switch(DISABLE);
    FilterVat_ChannelASwitch(DISABLE);
    FilterVat_ChannelBSwitch(DISABLE);
    Delay_ms(1000); 

	Venting_Switch(ENABLE); //һֱ���� Ŀ����ж������ˮ��ѹ��
    Intake_Switch(DISABLE);
    FilterVat_ChannelASwitch(DISABLE);
    FilterVat_ChannelBSwitch(DISABLE);
    SystemParams.ucStartupFlag = 0;
	SystemParams.ucOverTemperatureProtectFlag = 0;
	SystemParams.ucUnlockFlag = 0;
	
}

/*******************************************************************************
* Function Name  : System_Init(void)
* Description    : system  init
* Input          : None
* Output         : None
* Return         : None
* Author         : sunny
*******************************************************************************/
void  System_Init(void)
{
    u16  upg_flag = 0;
	
	#if PASSWORD
	
    checkResult = UID_Encryption_Key_Check(  (void*)KEY_LOCATION,//���뵱ǰоƬFLASH�е���Կ���˴�����Ĳ�������¼������Ӧ����ȫһ��
                                             (void*)UID_LOCATION,//���뵱ǰоƬUID
                                              myID,              //�����û��Զ���ID, �˴�����Ĳ�������¼������Ӧ����ȫһ��
                                              LENGTH_12,         //������Կ���ȣ� �˴�����Ĳ�������¼������Ӧ����ȫһ��
                                              LITTLE_ENDIA,      //�������ѡ�� �˴�����Ĳ�������¼������Ӧ����ȫһ��
                                              ALGORITHM_2);      //�����㷨ѡ�� �˴�����Ĳ�������¼������Ӧ����ȫһ��

    if(checkResult != 0)//�����֤���Ϊ����ֵ������֤��ͨ��
    {   
        printf("\r\n��Կ�Ƿ�!\r\n"); 
        while(1)
        {     
           LED_ORANGE_ON();
	       LED_BLUE_ON();
		   Delay_ms(600);
		   LED_ORANGE_OFF();
	       LED_BLUE_OFF();
		   Delay_ms(600); 
        }
    }
	
    printf("\r\n��Կ�Ϸ�!\r\n");

	#endif
	
	BLUETOOTH_POWEROFF();
	BLUETOOTH_PowerControl(POWER_OFF);
	LED_ORANGE_OFF();
	LED_BLUE_OFF();
	BRIDGE1_OFF();
	BRIDGE2_OFF();
	#if 1
	EN_POWER_ON();
	#endif
	WORK_LED_OFF();
	Relay_Init();       //��ʼ���̵���
	Takt_Init();       //���Ŀ��Ƴ�ʼ��
	Pressure_Init();   //��ѹ���Ƴ�ʼ��
	Fan_Init();        //��ʼ������״̬
	Flow_Init();       //�������Ƴ�ʼ��
	Compressor_Init(); //ѹ������ʼ��
	LimitPressure_Init(); //��ѹ��ʼ��
	MCP9801_Init();
    Pressure_PIDInit(&Pressure_Ctrl);
    Flow_PIDInit(&Flow_Ctrl);
	STMFLASH_Read(FLASH_SAVE_ADDR+4,&upg_flag,1);
	if(upg_flag == 0x5050)
	{
        SystemParams.ucPowerStateFlag = SYSTEM_POWER_ON;
		SystemParams.ucRunStateFlag = SYSTEM_RUN; 
        LED_ORANGE_ON();
		LED_BLUE_ON();
		BLUETOOTH_POWERON();
		ANDROID_POWER_ON(); 
		upg_flag = 0;
		STMFLASH_Write(FLASH_SAVE_ADDR+4,&upg_flag,1);	
	}
	else
	{
       SystemParams.ucPowerStateFlag = SYSTEM_POWER_OFF;
	}
    
   
}

/*******************************************************************************
* Function Name  : void SystemON(void)
* Description    : system ON
* Input          : None
* Output         : None
* Return         : None
* Author         : sunny
*******************************************************************************/
void SystemON(void)
{
    if(SystemParams.ucResetFlag == SYSTEM_RESET_STATUS)
    {
		BLUETOOTH_POWERON();
		//LED_WHILE_ON();
	    //LED_ORANGE_OFF();
		//EN_POWER_ON();
		BLUETOOTH_PowerControl(POWER_ON);
        SystemParams.ucResetFlag = SYSTEM_SET_STATUS;
    }
}

/*******************************************************************************
* Function Name  : void SystemOFF(void)
* Description    : system off
* Input          : None
* Output         : None
* Return         : None
* Author         : sunny
*******************************************************************************/
void SystemOff(void)
{
    if(SystemParams.ucResetFlag == SYSTEM_SET_STATUS)
    { 
        SystemParams.ucHandShakeStatusFlag = 0;
		SystemParams.ucStartupFlag = 0;
		SystemParams.ucAgingTestMode = 0;
		BLUETOOTH_POWEROFF();
		BLUETOOTH_PowerControl(POWER_OFF);
	    LED_ORANGE_OFF();
		LED_BLUE_OFF();
		WORK_LED_OFF();
		ANDROID_POWER_OFF();
		SystemStartupOrShutdown(DISABLE);
		Relay_Init();
		SystemParams.ucBluetoothRunFlag = SYSTEM_PAUSE;
        SystemParams.ucResetFlag = SYSTEM_RESET_STATUS; 
    }
}

//����LEDָʾ
void WorkLedIndicate(void)
{
    if((SystemParams.uiSetPressureValue == 0)&&(SystemParams.ucCurrentFlowMode == 5)&&(SystemParams.ucCurrentTaktMode == 0))
	{
	     WORK_LED_OFF();
	}
	else
	{
       if(SystemParams.uiSetPressureValue != 0)
       {
	       WORK_LED_ON();
       }
	}
}

/*******************************************************************************
* Function Name  : void Temperature_Handle(void)
* Description    : �¶ȴ���
* Input          : None
* Output         : None
* Return         : None
* Author         : sunny
*******************************************************************************/
void  Temperature_Handle(void)
{
     static u8 temper_count[2] = {0,0};
	 static u8 temper_state = 0;
	 
	 u8 temperature_code[2] = {0x19, 0x01};

	 if((SystemParams.fTemperatureValue>=70)||(SystemParams.ucTemperature1Value>=90)||(SystemParams.ucTemperature2Value>=90))
     {
        if(++temper_count[0]>=120)
        {
           if(temper_state == 0)
           { 
              SystemParams.ucOverTemperatureProtectFlag = 1;
			  temper_state = 1;  
		   }

		   if(SystemParams.ucOverTemperatureProtectResponseFlag == 0)
		   {
              SendDatPacketToDevice(USART2,2,0x44,0x02,temperature_code);
			  temper_state = 0;
		   }
		   temper_count[0] = 0;
		}
		temper_count[1] = 0;
	 }
	 else
	 {
         if((SystemParams.fTemperatureValue<=67)&&(SystemParams.ucTemperature1Value<=87)&&(SystemParams.ucTemperature2Value<=87))
         {
            if(++temper_count[1]>=120)
	        {
	           if(temper_state == 1)
	           {
	              temper_state = 0;  
			   }
			   temper_count[1] = 0;
			}  
			SystemParams.ucOverTemperatureProtectResponseFlag = 0;
		 }
		 temper_count[0] = 0;
	 }
}

/*******************************************************************************
* Function Name  : void Lock_Handle(void)
* Description    : ������
* Input          : None
* Output         : None
* Return         : None
* Author         : sunny
*******************************************************************************/
void  Lock_Handle(u8 power_state,u8 *lock_step,u8 *lock_state_flag,u8 *protect_response)

{
    static  u8  power_state_temp = 2;
    static  u8  time_count[2] = {0,0};
	static  u8  lock_state = 0;
	
	u8 lock_code[2] = {0x19, 0x02};
	
    if(power_state != power_state_temp)
    {
        *lock_step = 0; 
		time_count[0] = 0;time_count[1] = 0;
		
		LOCK_OUTPUT_DETECT_OFF();
		
		if(power_state)
		{
			BRIDGE2_OFF();
			BRIDGE1_ON();
		}
		/*else
		{
			 
             BRIDGE1_OFF();
			 BRIDGE2_ON();
		}*/
		
		power_state_temp = power_state;
		*lock_state_flag = 0;
	}

	switch(*lock_step)
    {
        case 0:
	    if(++time_count[0]>=20)
        {
           time_count[0] = 0;
		   *lock_step = 1;
		}
		break;
	    case 1:
		BRIDGE1_OFF();
		BRIDGE2_OFF();
		if(++time_count[0]>=80)
	   	{
           time_count[0] = 0;
		   *lock_step = 2;
		}
		break;
		case 2:
	    LOCK_OUTPUT_DETECT_ON();		
		if(++time_count[0]>=20)
	   	{
           time_count[0] = 0;
		   *lock_step = 3;
		}
		break;
		case 3:
		if(LOCK_DETECT()) //����
		{
           if(++time_count[0]>=80)
           {
			  time_count[0] = 0;
			  
              if(lock_state == 0)
              {  
                 *lock_state_flag = 1;
				 lock_state = 1;
			  }
			  if(*protect_response == 0)
			  {
                 SendDatPacketToDevice(USART2,2,0x44,0x02,lock_code);
				 lock_state = 0;
			  }  
		   }
		   time_count[1] = 0;
		} 
		else  //����
		{
            if(++time_count[1]>=40)
            {
               lock_state = 0;
			   time_count[1] = 0;
               *lock_state_flag = 0;
			   *protect_response = 0;
			}
			time_count[0] = 0;
		}
		break;
	}
}

/** ������� **/
void  OpenLockOutput(void)
{
    if(SystemParams.ucOpenLockFlag == 1)
    {
	    SystemParams.ucCurrentTaktMode = 5;
        Takt_Configuration(SystemParams.ucCurrentTaktMode);        //���ĵ͵�ƽ
        Delay_ms(1000);
		//SystemParams.ucCurrentTaktMode = 0;
       // Takt_Configuration(SystemParams.ucCurrentTaktMode);        //���ĵ͵�ƽ
        BRIDGE1_OFF();
		BRIDGE2_ON();
		Delay_ms(500);
		BRIDGE1_OFF();
		BRIDGE2_OFF();
		LED_ORANGE_OFF();
	    LED_BLUE_OFF();
		SystemParams.ucOpenLockFlag = 0;
	}
}

/*******************************************************************************
* Function Name  : SystemWorkState(u8 power_state) 
* Description    : System power state 
* Input          : Power state  
* Output         : None
* Return         : None
* Author         : sunny
*******************************************************************************/
void  SystemWorkState(u8         power_state)
{
     
	 switch(power_state)
     {
        case SYSTEM_POWER_ON:  //���� 
        
        SystemON();              
        
	    if(SystemParams.ucAppStartupFlag == 1)  
	    {
            SystemParams.ucAppStartupFlag = 0;
			SystemStartupOrShutdown(ENABLE);
			if((SystemParams.ucUnLockResponseFlag == 0)&&(SystemParams.ucOverTemperatureProtectResponseFlag == 0))
			{
               SystemParams.ucStartupFlag = 1;
			}
	    }

        if(SystemParams.ucAppRunPauseFlag == 1)
        {
            SystemParams.ucAppRunPauseFlag = 0;
			if(SystemParams.ucRunStateFlag == 0)
			{
				 SystemRunOrPause(DISABLE);
			}
			else 
			{
				SystemRunOrPause(ENABLE);
				if((SystemParams.ucUnLockResponseFlag == 0)&&(SystemParams.ucOverTemperatureProtectResponseFlag == 0))
				{
	               SystemParams.ucStartupFlag = 1;
				}
			}
		}
		
		Pressure_Handle();                 //��ѹ����
	    LimitPressure_Handle();            //��ѹ����
	    Flow_Handle();                     //��������
	    Takt_Configuration(SystemParams.ucCurrentTaktMode);
		Flow_Configuration(SystemParams.ucCurrentFlowMode);
	    MCP9801_TemperatureDataGet();     //�¶����ݻ�ȡ
	    BEEP_Handle(&SystemParams);
		WorkLedIndicate();
		
        break;
        case SYSTEM_POWER_OFF:  //�ػ�
        SystemOff();                      
		OpenLockOutput();
		break;
     }
	 
	 Temperature_Handle(); 
	 Lock_Handle(power_state,&SystemParams.ucLockStep,&SystemParams.ucUnlockFlag,&SystemParams.ucUnLockResponseFlag);
	 SystemProtect();   //ϵͳ����
	 
}



