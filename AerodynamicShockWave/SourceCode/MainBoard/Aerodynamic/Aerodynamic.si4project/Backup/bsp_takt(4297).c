
/*
*********************************************************************************************************
*
*	模块名称 : 节拍驱动模块
*	文件名称 : takt.c
*	版    本 : V0.1
*	说    明 : 节拍输出.
*********************************************************************************************************
*/


#include  "config.h"

/**
 * @brief       节拍GPIO初始化
 * @param       无
 * @retval      无
 */
void Takt_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     /* Enable peripheral clocks --------------------------------------------------*/
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	  /****** PB5、PB6 ********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 	 GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief       节拍1 PWM初始化
 * @param       无
 * @retval      无
 */
void TIM4_PWM_Init(u16 arr,u16 psc)  
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); 
	
    /* Enable PWR and BKP clock */
    TIM_DeInit(TIM4);
 
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = arr ;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    /* PWM1 Mode configuration: Channel1 */      
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM4, ENABLE);
    //TIM_GenerateEvent(TIM4, TIM_EventSource_Update);
    /* TIM4 enable counter */
    TIM_Cmd(TIM4, ENABLE);
    TIM_SetCompare1(TIM4, 0x00);
}

/**
 * @brief       节拍2 PWM初始化
 * @param       周期值，预分频值
 * @retval      无
 */
void TIM3_PWM_Init(u16 arr,u16 psc)  
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 

	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射 
	
    /* Enable PWR and BKP clock */
    TIM_DeInit(TIM3);
 
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = arr ;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    /* PWM1 Mode configuration: Channel1 */      
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    //TIM_GenerateEvent(TIM4, TIM_EventSource_Update);
    /* TIM4 enable counter */
    TIM_Cmd(TIM3, ENABLE);
    TIM_SetCompare2(TIM3, 0x00);
}

/**
 * @brief       节拍状态
 * @param       ID 号 是否使能
 * @retval      无
 */
void Takt_Status(u8 id,FunctionalState NewState)
{
    if(id == LEFT_HAND)
    {
	    if(NewState == ENABLE)
	    {
	       TIM_Cmd(TIM3, ENABLE); 
		}
		else
		{
           TIM_SetCompare2(TIM3,0);
		   TIM_Cmd(TIM3, DISABLE);
		}
	}
	else if(id == RIGHT_HAND)
    {
	    if(NewState == ENABLE)
	    {
	       TIM_Cmd(TIM4, ENABLE); 
		}
		else
		{
           TIM_SetCompare1(TIM4,0);
		   TIM_Cmd(TIM4, DISABLE);
		}
	}
}

/**
 * @brief       节拍设置周期比较值
 * @param       ID 号 周期值，比较值
 * @retval      无
 */
void Takt_SetPeriodCompareValue(u8 id,u16 Period,u16 CompareValue)
{
    if(id == LEFT_HAND)
    {
       TIM_SetAutoreload(TIM3,Period);
       TIM_SetCompare2(TIM3,CompareValue);
       TIM_Cmd(TIM3, ENABLE);
	}
	else if(id == RIGHT_HAND)
    {
       TIM_SetAutoreload(TIM4,Period);
       TIM_SetCompare1(TIM4,CompareValue);
       TIM_Cmd(TIM4, ENABLE);
	}
	
}


/**
 * @brief       节拍set
 * @param       ID 号 周期值，比较值
 * @retval      无
 */
void Takt_SetFrq(u8 id,u8 takt_mode)
{
	static	u8	takt_mode_temp = 13;

	if(takt_mode != takt_mode_temp)
	{
		if((takt_mode>=1) && (takt_mode<=17))
		{
		   Takt_Status(id,FunctionalState ENABLE);
		}
		else
		{
		   Takt_Status(id,FunctionalState DISABLE);
		}
		
		switch(takt_mode)
		{
			case 0x00: Takt_Status(id,FunctionalState DISABLE);(); break; 
			case 0x01: Takt_SetPeriodCompareValue(id,u16 Period,u16 CompareValue) break; 
			case 0x02: Takt_Custom(TAKT2PERIOD,TAKT2COMPAREC); break; 
			case 0x03: Takt_Custom(TAKT3PERIOD,TAKT3COMPAREC); break; 
			case 0x04: Takt_Custom(TAKT4PERIOD,TAKT4COMPAREC); break; 
			case 0x05: Takt_Custom(TAKTOFFPERIOD,TAKTOFFCOMPAREC); break; 
		  
			case 0x06: Takt_Custom(TAKT6PERIOD,TAKT6COMPAREC); break; 
			case 0x07: Takt_Custom(TAKT7PERIOD,TAKT7COMPAREC); break; 
			case 0x08: Takt_Custom(TAKT8PERIOD,TAKT8COMPAREC); break; 
			case 0x09: Takt_Custom(TAKT9PERIOD,TAKT9COMPAREC); break; 
			case 0x0a: Takt_Custom(TAKT10PERIOD,TAKT10COMPAREC); break; 
			case 0x0b: Takt_Custom(TAKT11PERIOD,TAKT11COMPAREC); break; 
			default: Takt_Switch(DISABLE); break; 
		}
  
		takt_mode_temp = takt_mode;
	}
}



