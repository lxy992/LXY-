
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

#define  FRQUENCYVALUE_GRADE 17

const u16 FrequencyValue[FRQUENCYVALUE_GRADE] = {1000, 500, 333, 250, 200, \
                                                 167, 143, 125, 111, 100, \
                                                 91, 83, 77, 74, 71, 68, \
                                                 65};			
/**
 * @brief       �ֱ�״̬���GPIO��ʼ��
 * @param       ��
 * @retval      ��
 */
void HandStatusDetect_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,  ENABLE);  
	 
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	 /****** PB15��PB14********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 
 	 GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief       12V��Դ����GPIO
 * @param       ��
 * @retval      ��
 */
void EN_12V_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);  
	 
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	 /****** PA15********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 	 GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief       DWIN ����
 * @param       ��
 * @retval      ��
 */
void DWIN_POWER_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);  
	 
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	 /****** PA5********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 	 GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief       ���ÿ��� GPIO��ʼ��
 * @param       ��
 * @retval      ��
 */
void AirPump_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE);  
	 
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	 /****** PC12********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 	 GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
 * @brief       LED GPIO��ʼ��
 * @param       ��
 * @retval      ��
 */
void LED_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE);  
	 
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	 /****** PC0��PC1********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 	 GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
 * @brief       ���� GPIO��ʼ��
 * @param       ��
 * @retval      ��
 */
void FAN_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE);  
	 
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	 /****** PC4********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 	 GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
 * @brief       �����ֱ����� GPIO��ʼ��
 * @param       ��
 * @retval      ��
 */
void HAND_POWER_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE);  
	 
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	 /****** PC5��PC6********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 	 GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/***********************************
�������ܣ��ֱ�װ��γ�״̬��ⷢ��
�����������
�����������
��    ע�����3Sѯ���ֱ��Ƿ���
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
		SystemParams.ucLeftHandLoadPull = 1;
	  }
	  left_hand_detect_count[1] = 0;
   }
   else
   {
      if(++left_hand_detect_count[1]>=10)
      {
        left_hand_detect_count[1] = 0;   
		SystemParams.ucLeftHandLoadPull = 0;
	  }
	  left_hand_detect_count[0] = 0;
   }
   
   if(RIGHT_HAND_DETECT() == 0)
   {
      if(++right_hand_detect_count[0]>=30)
      {
        right_hand_detect_count[0] = 0;   
		SystemParams.ucRightHandLoadPull = 1;
	  }
	  right_hand_detect_count[1] = 0;
   }
   else
   {
      if(++right_hand_detect_count[1]>=10)
      {
        right_hand_detect_count[1] = 0;   
		SystemParams.ucRightHandLoadPull = 0;
	  }
	  right_hand_detect_count[0] = 0;
   }
      
   if(SystemParams.ucLeftHandLoadPull != left_hand_status_change)
   {
	  left_hand_status_change = SystemParams.ucLeftHandLoadPull;
   }

   if(SystemParams.ucRightHandLoadPull != right_hand_status_change)
   {
	  right_hand_status_change = SystemParams.ucRightHandLoadPull; 
   }
   
}



/*******************************************************************************
* FunctionName   : SystemStartupOrShutdown
* Description    : �������߹ػ��Ĺ��̺���
* Notice         : ���ػ���������˹���
* Param[in]      : NewState---RESET/SET
* Output         : NO.
* Return         : NO.
*******************************************************************************/
void SystemStartupOrShutdown(FunctionalState NewState)
{

}

/*******************************************************************************
* FunctionName   : SystemRunOrPause(FunctionalState NewState)
* Description    : ϵͳ���л���ͣ
* Notice         : ״̬�л�������˹���
* Param[in]      : NewState---ENABLE/DISABLE
* Output         : NO.
* Return         : NO.
*******************************************************************************/
void SystemRunOrPause(FunctionalState NewState)
{
	
}

/*******************************************************************************
* FunctionName   : SystemProtect
* Description    : ����
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
*******************************************************************************/
void SystemProtect(void)
{

	
	
}

/*******************************************************************************
* Function Name  : System_Init(void)
* Description    : system  init
* Input          : None
* Output         : None
* Return         : None
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
           ORANG_LED_ON();
		   Delay_ms(600);
		   ORANG_LED_OFF();
		   Delay_ms(600); 
        }
    }
	
    printf("\r\n��Կ�Ϸ�!\r\n");

	#endif
    BEEP_OFF();
	EN_POWER_OFF();
	DWIN_POWER_OFF();
	PUMP_POWER_OFF();
	FAN_OFF();
	LEFT_HAND_POWER_OFF();
	RIGHT_HAND_POWER_OFF();
	Pressure_Init();
}

/*******************************************************************************
* Function Name  : void SystemON(void)
* Description    : system ON
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SystemON(void)
{
    if(SystemParams.ucResetFlag == SYSTEM_RESET_STATUS)
    {
		EN_POWER_ON();
        WHITE_LED_ON();
		ORANG_LED_OFF(); 
		DWIN_POWER_ON();
		EN_V1_OFF();
        SystemParams.ucResetFlag = SYSTEM_SET_STATUS;
    }
}

/*******************************************************************************
* Function Name  : void SystemOFF(void)
* Description    : system off
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SystemOff(void)
{
    if(SystemParams.ucResetFlag == SYSTEM_SET_STATUS)
    { 
		SystemStartupOrShutdown(DISABLE);
		LEFT_HAND_POWER_OFF();
		RIGHT_HAND_POWER_OFF();
		WHITE_LED_OFF();
		ORANG_LED_ON(); 
		EN_POWER_OFF();
		DWIN_POWER_OFF();
		EN_V1_ON();
		EN_V2_ON(); 
        SystemParams.ucResetFlag = SYSTEM_RESET_STATUS; 
    }
}

/***********************************
�������ܣ���ʱ���ֱ�����ȡ�汾��
�����������
�����������
��    ע����ʱ���ֱ���Դ����ȡ�ֱ��汾��
************************************/
void  TimeSharingOperation(void)
{
    
	u8  send_data[2];

	static  u8  step_num = 0;
	static  u8  time_count = 0;
	
	if(SystemParams.ucPowerStateFlag)
    {
        switch(step_num)
        {
           case 0:
		   time_count = 12;
		   step_num = 1;
		   break;
		   case 1:
		   if(time_count != 0)break;
		   time_count = 12;
		   LEFT_HAND_POWER_ON();
		   step_num = 2;
		   break;
		   case 2:
		   if(time_count != 0)break;	
		   time_count = 12;
		   RIGHT_HAND_POWER_ON();
		   step_num = 3;
		   break;
		   case 3:
		   if(time_count != 0)break;
		   time_count = 12;
		   step_num = 4;
		   break;
		   case 4:
		   if(time_count != 0)break;
		   step_num = 5;
		   time_count = 12;
		   break;
		   case 5:
		   break;
		}
	}
	else
	{
        step_num = 0;
	}

	if(time_count)
	{
       time_count--;
	}
}

/*******************************************************************************
* Function Name  : SystemWorkState(u8 power_state) 
* Description    : System power state 
* Input          : Power state  
* Output         : None
* Return         : None
*******************************************************************************/
void  SystemWorkState(u8      power_state)
{
     
	 switch(power_state)
     {
        case SYSTEM_POWER_ON:  //���� 
        SystemON();
		Pressure_Handle();                 //��ѹ����
	    BEEP_Handle(&SystemParams);
        break;
        case SYSTEM_POWER_OFF:  //�ػ�
        SystemOff();                      
		break;
     }

	 TimeSharingOperation();
	 SystemProtect();   //ϵͳ����
}



