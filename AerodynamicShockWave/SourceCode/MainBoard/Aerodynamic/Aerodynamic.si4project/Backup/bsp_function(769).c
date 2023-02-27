
#include "config.h"

SystemParams_t SystemParams = SystemParams_Defaults;

/**** 验证密钥所需的一些参数 */
#define KEY_LOCATION   0x08002FE0  //密钥在当前芯片中的存储地址
#define UID_LOCATION   0x1FFFF7E8  //当前芯片UID所在地址
u8 checkResult = 0; //验证结果
u8 myID[12] = {
                0x02, 0x01, 0x03, 0x04,
                0x05, 0x06, 0x07, 0x08, 
                0x09, 0x0A, 0x0D, 0x0C};//用户自定义ID


				
/**
 * @brief       手柄状态检测GPIO初始化
 * @param       无
 * @retval      无
 */
void HandStatusDetect_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,  ENABLE);  
	 
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	 /****** PB15、PB14********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 
 	 GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/***********************************
函数功能：手柄装入拔出状态监测
输入参数：无
输出参数：无
备    注：间隔3S询问手柄是否在
************************************/
void  HandLoadPullStatusDetectionSend(void)
{
   
   static u8  left_hand_status_change = 0;
   static u8  right_hand_status_change = 0;
   static u8  hand_load_flag = 0;
   static u8  left_pull_num = 0;
   static u8  right_pull_num = 0;
   static u8  left_hand_detect_count[2] = {0,0};
   static u8  right_hand_detect_count[2] = {0,0};
   
   u8  send_code[3];
   
 
   if(SystemParams.ucPowerStateFlag == SYSTEM_POWER_OFF)
   {
	  return;
   }
   
   if(LEFT_HAND_DETECT() == 0)
   {
      if(++left_hand_detect_count[0]>=30)
      {
        left_hand_detect_count[0] = 0;   
		Left_Hand_Status.ucLoadPull = 1;
	  }
	  left_hand_detect_count[1] = 0;
   }
   else
   {
      if(++left_hand_detect_count[1]>=10)
      {
        left_hand_detect_count[1] = 0;   
		Left_Hand_Status.ucLoadPull = 0;
	  }
	  left_hand_detect_count[0] = 0;
   }
   
   if(RIGHT_HAND_DETECT() == 0)
   {
      if(++right_hand_detect_count[0]>=30)
      {
        right_hand_detect_count[0] = 0;   
		Right_Hand_Status.ucLoadPull = 1;
	  }
	  right_hand_detect_count[1] = 0;
   }
   else
   {
      if(++right_hand_detect_count[1]>=10)
      {
        right_hand_detect_count[1] = 0;   
		Right_Hand_Status.ucLoadPull = 0;
	  }
	  right_hand_detect_count[0] = 0;
   }
   
   #endif
   
   if(Left_Hand_Status.ucLoadPull != left_hand_status_change)
   {
      send_code[0] = 0x04;
	  send_code[1] = SystemParams.ucLeftHandType;
	  send_code[2] = Left_Hand_Status.ucLoadPull;
	  SendDatPacketToDevice(&huart8,3,0x44,0x02,send_code);
	  left_hand_status_change = Left_Hand_Status.ucLoadPull;
   }

   if(Right_Hand_Status.ucLoadPull != right_hand_status_change)
   {
      send_code[0] = 0x04;
	  send_code[1] = SystemParams.ucRightHandType;
	  send_code[2] = Right_Hand_Status.ucLoadPull;
	  SendDatPacketToDevice(&huart8,3,0x44,0x02,send_code);
	  right_hand_status_change = Right_Hand_Status.ucLoadPull; 
   }
   
}

/*******************************************************************************
* FunctionName   : Fan_Switch
* Description    : 风扇的开关函数
* Notice         : NO.
* Param[in]      : key---为枚举类型，ENABLE开启风扇，DISABLE关闭风扇
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Fan_Switch(FunctionalState NewState)//风扇控制 0---关闭；1---开启
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
* Description    : 风扇的初始化，默认初始化为关闭状态
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Fan_Init(void)//初始化风扇状态
{
    Fan_Switch(ENABLE);//风扇关闭
}

/*******************************************************************************
* FunctionName   : Compressor_Switch
* Description    : 压缩机的开关函数
* Notice         : NO.
* Param[in]      : NewState---为枚举类型，ENABLE开启压缩机，DISABLE关闭压缩机
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Compressor_Switch(FunctionalState NewState)//压缩机控制 0---关压缩机；1---开压缩机
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
* Description    : 压缩机的初始化，默认初始化为关闭状态
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Compressor_Init(void)//压缩机初始化
{
    Compressor_Switch(DISABLE);//压缩机关闭
}

/*******************************************************************************
* FunctionName   : FilterVat_ChannelASwitch
* Description    : 过滤桶通道A开关
* Notice         : NO.
* Param[in]      : NewState---为枚举类型，ENABLE开启继电器，DISABLE关闭继电器
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void FilterVat_ChannelASwitch(FunctionalState NewState)//P11对应的继电器控制 0---关断；1---开启
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
* FunctionName   : Ctrl_Relay_Init
* Description    : 初始化5个继电器，默认初始化为全部关闭状态
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Relay_Init(void)//初始化继电器
{
    FilterVat_ChannelASwitch(DISABLE);  
	FilterVat_ChannelBSwitch(DISABLE);
	Venting_Switch(DISABLE);        //关闭排气
    Intake_Switch(DISABLE);        //关闭进气
    MasterAirCylinder_Switch(DISABLE); //主缸主气压
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

/*******开锁LED指示*********/
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
* Description    : 开机或者关机的过程函数
* Notice         : 开关机必须走完此过程
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
    Venting_Switch(ENABLE);        //一直排气 目的是卸掉推针水的压力
    SystemParams.ucCurrentFlowMode = 4;
    Flow_Configuration(SystemParams.ucCurrentFlowMode);         //流量最大

	SystemParams.ucCurrentTaktMode = 0;
	Takt_Configuration(SystemParams.ucCurrentTaktMode);        //节拍低电平

	
    if(NewState)
    {
		if(SystemParams.uiMasterAirCylinder >= 1300)
        {
            SystemParams.ucFilterVatChannelABFlag = 0;
	        Venting_Switch(ENABLE);           //一直排气 目的是卸掉推针水的压力
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
        Venting_Switch(ENABLE);           //一直排气 目的是卸掉推针水的压力
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
		Compressor_Switch(DISABLE);//压缩机关闭
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
			Venting_Switch(ENABLE); //一直排气 目的是卸掉推针水的压力
	        Intake_Switch(DISABLE);
        }
    }

}

/*******************************************************************************
* FunctionName   : SystemRunOrPause(FunctionalState NewState)
* Description    : 系统运行或暂停
* Notice         : 状态切换需走完此过程
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
		Flow_Configuration(SystemParams.ucCurrentFlowMode);     //流量最大
		SystemParams.ucCurrentTaktMode = 0;
		Takt_Configuration(SystemParams.ucCurrentTaktMode);    //节拍低电平
        Venting_Switch(ENABLE);                                //一直排气 目的是卸掉推针水的压力
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
* Description    : 保护
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
    Venting_Switch(ENABLE);  //一直排气 目的是卸掉推针水的压力
    SystemParams.ucCurrentFlowMode = 4;
    Flow_Configuration(SystemParams.ucCurrentFlowMode);   //流量最大
    SystemParams.ucCurrentTaktMode = 5;
    Takt_Configuration(SystemParams.ucCurrentTaktMode);    //节拍高电平  节拍使能，流量最大，目的是把针筒外的气压卸掉
	SystemParams.ucFilterVatChannelABFlag = 0;
    Venting_Switch(ENABLE);           //一直排气 目的是卸掉推针水的压力
    Intake_Switch(DISABLE);
    FilterVat_ChannelASwitch(ENABLE);
    FilterVat_ChannelBSwitch(ENABLE);  
	SystemParams.ucLimitPressureSwitch = 0;
	MASTER_CYLINDER_LIMIT1_ON(); 
	MasterAirCylinder_Switch(ENABLE);  
	Delay_ms(3000);
	SystemParams.ucCurrentTaktMode = 0;
	Takt_Configuration(SystemParams.ucCurrentTaktMode); //节拍低电平
	Fan_Switch(ENABLE);
	#if (DAC_EN == 0)
	Compressor_Switch(DISABLE);//压缩机关闭
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

	//Intake_Switch(ENABLE);                //进气   
    FilterVat_ChannelASwitch(ENABLE);     
    FilterVat_ChannelBSwitch(ENABLE);
    Delay_ms(1000);
    Intake_Switch(DISABLE);
    FilterVat_ChannelASwitch(DISABLE);
    FilterVat_ChannelBSwitch(DISABLE);
    Delay_ms(1000); 

	Venting_Switch(ENABLE); //一直排气 目的是卸掉推针水的压力
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
	
    checkResult = UID_Encryption_Key_Check(  (void*)KEY_LOCATION,//传入当前芯片FLASH中的密钥，此处输入的参数与烧录器配置应当完全一致
                                             (void*)UID_LOCATION,//传入当前芯片UID
                                              myID,              //传入用户自定义ID, 此处输入的参数与烧录器配置应当完全一致
                                              LENGTH_12,         //传入密钥长度， 此处输入的参数与烧录器配置应当完全一致
                                              LITTLE_ENDIA,      //传入端序选择， 此处输入的参数与烧录器配置应当完全一致
                                              ALGORITHM_2);      //传入算法选择， 此处输入的参数与烧录器配置应当完全一致

    if(checkResult != 0)//如果验证结果为非零值，则验证不通过
    {   
        printf("\r\n密钥非法!\r\n"); 
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
	
    printf("\r\n密钥合法!\r\n");

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
	Relay_Init();       //初始化继电器
	Takt_Init();       //节拍控制初始化
	Pressure_Init();   //气压控制初始化
	Fan_Init();        //初始化风扇状态
	Flow_Init();       //流量控制初始化
	Compressor_Init(); //压缩机初始化
	LimitPressure_Init(); //限压初始化
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

//工作LED指示
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
* Description    : 温度处理
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
* Description    : 锁处理
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
		if(LOCK_DETECT()) //无锁
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
		else  //有锁
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

/** 开锁输出 **/
void  OpenLockOutput(void)
{
    if(SystemParams.ucOpenLockFlag == 1)
    {
	    SystemParams.ucCurrentTaktMode = 5;
        Takt_Configuration(SystemParams.ucCurrentTaktMode);        //节拍低电平
        Delay_ms(1000);
		//SystemParams.ucCurrentTaktMode = 0;
       // Takt_Configuration(SystemParams.ucCurrentTaktMode);        //节拍低电平
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
        case SYSTEM_POWER_ON:  //开机 
        
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
		
		Pressure_Handle();                 //气压处理
	    LimitPressure_Handle();            //限压处理
	    Flow_Handle();                     //流量处理
	    Takt_Configuration(SystemParams.ucCurrentTaktMode);
		Flow_Configuration(SystemParams.ucCurrentFlowMode);
	    MCP9801_TemperatureDataGet();     //温度数据获取
	    BEEP_Handle(&SystemParams);
		WorkLedIndicate();
		
        break;
        case SYSTEM_POWER_OFF:  //关机
        SystemOff();                      
		OpenLockOutput();
		break;
     }
	 
	 Temperature_Handle(); 
	 Lock_Handle(power_state,&SystemParams.ucLockStep,&SystemParams.ucUnlockFlag,&SystemParams.ucUnLockResponseFlag);
	 SystemProtect();   //系统保护
	 
}



