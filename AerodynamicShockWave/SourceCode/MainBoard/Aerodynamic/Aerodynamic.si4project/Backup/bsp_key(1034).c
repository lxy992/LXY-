
/*
*********************************************************************************************************
*
*	模块名称 : 按键模块
*	文件名称 : key.c
*	版    本   : V0.1
*	说    明   : 主机开关按键.
*********************************************************************************************************
*/

#include "config.h"

/**
 * @brief       KEY GPIO初始化
 * @param       无
 * @retval      无
 */
void KEY_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);  
	 
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	 /****** PA4********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 
 	 GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief       KEY 状态获取
 * @param       无
 * @retval      键值
 */
u8  KEY_StatusGet(void)
{
      static  u8  key_status = 0;
	  static  u16  key_count = 0;
	  static  u8  key_id = 0; 
	  static  u8  key_lock = 0;
	  	
	  u8  key_temp = 0;
	  u8  key_value = 0;
	  
	  if(SYSTEM_KEY_DET() == 0)  //按下
	  {
         key_temp |= 0x01;
	  } 
	  
	  switch(key_status)
	  {
         case eNull:
	         if(key_temp != 0)
	         {
	            key_status = ePress;
			 }
			 else
			 {
	            key_status = eNull;
			 }
			 key_id = 0;
		 break;
		 case ePress:
			 if(key_temp != 0)
	         {
		         key_status = eHold;
				 if((key_temp&0x01) == 0x01)
				 {
                    key_id = POWER_KEY;
				 }
			 }
			 else
			 {
	             key_status = eNull;
			 }
			 key_count = 0;
		 break;
		 case eHold:
			 if(key_temp == 0)
	         {
				key_status = eNull;
				if(key_count < LONG_PRESS_TIME_2S)
				{
                   key_value = SHORT_PRESS+key_id;
				}
			    key_count = 0;
				key_lock = 0;
			 }
			 else
			 {
				 key_count++; 
				 
				 if(key_count >= LONG_PRESS_TIME_2S)
				 {
                    if((key_id == POWER_KEY)&&(key_count==LONG_PRESS_TIME_2S))
                    {
					   key_value = LONG_PRESS_2S+key_id;
                    }

					if((key_count==LONG_PRESS_TIME_2S)&&(SystemParams.ucPowerStateFlag == SYSTEM_POWER_ON))
					{
                        key_lock = 1; 
					}
          
					if(key_lock)
					{
						 if(key_count == LONG_PRESS_TIME_10S)
						 {
	                        key_value = LONG_PRESS_10S+key_id;
						 }	

						 if(key_count>=LONG_PRESS_TIME_10S)
						 {
	                        key_count = LONG_PRESS_TIME_10S+50; 
						 }
					}
					else
					{
                       key_count = LONG_PRESS_TIME_2S+50;  
					}
				 }
			 }
		 break;
		 case eFree:
		 break;
 		 default :break;
	  }
	 return  key_value;
}

/**
 * @brief       KEY处理
 * @param       无
 * @retval      无
 */
void  KEY_Handle(void)
{
     u8  key_value = 0;
	 u8  buffer[]={0x53,0x47,0x06,0x00,0x02,0x43,0x02,0x01,0x00,0x14,0xB1};
	 
	 key_value = KEY_StatusGet();

	 switch(key_value)
	 {
        case eShortPress:
			if(SystemParams.ucPowerStateFlag == SYSTEM_POWER_OFF)
			{
                SystemParams.ucPowerStateFlag = SYSTEM_POWER_ON;
				SystemParams.ucRunStateFlag = SYSTEM_RUN; 
	            LED_ORANGE_ON();
				LED_BLUE_ON();
				ANDROID_POWER_ON();
			}
			else
			{
				UART_SendData(USART2,buffer,sizeof(buffer));
			}
		break;
		case eLongPress_2s:
			SystemParams.ucPowerStateFlag = (SystemParams.ucPowerStateFlag == SYSTEM_POWER_OFF)?SYSTEM_POWER_ON:SYSTEM_POWER_OFF;
			if(SystemParams.ucPowerStateFlag == SYSTEM_POWER_ON)
			{
                SystemParams.ucRunStateFlag = SYSTEM_RUN; 
	            LED_ORANGE_ON();
				LED_BLUE_ON();
				ANDROID_POWER_ON();
			}
		break;
		case eLongPress_10s:
			SystemParams.ucOpenLockFlag = 1;
		break;
		default : 
		break;
	 } 
}


