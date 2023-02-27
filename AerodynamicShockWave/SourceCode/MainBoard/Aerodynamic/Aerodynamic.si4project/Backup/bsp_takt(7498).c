
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

/*******************************************************************************
* Function Name  : TIM4_PWM_Init(u16 arr,u16 psc)
* Description    : Config the timer4 channel2 output pwm ctrl takt
* Input          : 1000 4800
* Output         : None
* Return         : None
* Author         : sunny    
*******************************************************************************/
void TIM4_PWM_Init(u16 arr,u16 psc)  
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
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
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);  
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM4, ENABLE);
    //TIM_GenerateEvent(TIM4, TIM_EventSource_Update);
    /* TIM4 enable counter */
    TIM_Cmd(TIM4, ENABLE);
    TIM_SetCompare2(TIM4, 0x00);
}


/*******************************************************************************
* FunctionName   : Takt_Switch
* Description    : 节拍的开关配置
* Notice         : NO.
* Param[in]      : NewState--- 若ENABLE，开启节拍;若DISABLE，关闭节拍
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Takt_Switch(FunctionalState NewState)
{
	if(NewState)
    {
       //TIM_Cmd(TIM4, ENABLE);
    }
    else
    {   
       TIM_SetCompare2(TIM4, 0x00);
	  // TIM_Cmd(TIM4, DISABLE);
    }
}

/*******************************************************************************
* FunctionName   : Takt_Custom
* Description    : 自定义节拍配置
* Notice         : NO.
* Param[in]      : Period---比较周期值，  CompareValue---比较值
* Output         : 输出一个PWM波形
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Takt_Custom(u16 Period,u16 CompareValue)//自定义节拍输出
{
	TIM_SetAutoreload(TIM4,Period);
    TIM_SetCompare2(TIM4,CompareValue);
    TIM_Cmd(TIM4, ENABLE);
}

/*******************************************************************************
* FunctionName   : Takt_Init
* Description    : 初始化节拍配置
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Takt_Init(void)
{
   Takt_Switch(DISABLE);
}

/*******************************************************************************
* FunctionName   : Takt_Configuration
* Description    : 配置节拍Takt的5种模式
* Notice         : NO.
* Param[in]      : take_mode---节拍模式，共5种，输入参数0到4.
                    0---Takt Off; 1---Takt1; 2---Takt2; 3---Takt3; 4---Takt4;
                    5---节拍5
                    Other---Takt Off.
* Output         : 其中一种模式的PWM
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Takt_Configuration(u8 takt_mode)
{
	static	u8	takt_mode_temp = 13;

	if(takt_mode != takt_mode_temp)
	{
		if((takt_mode>=1) && (takt_mode<=11))
		{
		   Takt_Switch(ENABLE);
		}
		else
		{
		   Takt_Switch(DISABLE);
		}
		
		switch(takt_mode)
		{
			case 0x00: Takt_Init(); break; //输出低电平
			case 0x01: Takt_Custom(TAKT1PERIOD,TAKT1COMPAREC); break; //10Hz
			case 0x02: Takt_Custom(TAKT2PERIOD,TAKT2COMPAREC); break; //5Hz
			case 0x03: Takt_Custom(TAKT3PERIOD,TAKT3COMPAREC); break; //2.5Hz
			case 0x04: Takt_Custom(TAKT4PERIOD,TAKT4COMPAREC); break; //2Hz
			case 0x05: Takt_Custom(TAKTOFFPERIOD,TAKTOFFCOMPAREC); break; //输出高电平
		  
			case 0x06: Takt_Custom(TAKT6PERIOD,TAKT6COMPAREC); break; //9Hz
			case 0x07: Takt_Custom(TAKT7PERIOD,TAKT7COMPAREC); break; //8Hz
			case 0x08: Takt_Custom(TAKT8PERIOD,TAKT8COMPAREC); break; //7Hz
			case 0x09: Takt_Custom(TAKT9PERIOD,TAKT9COMPAREC); break; //6Hz
			case 0x0a: Takt_Custom(TAKT10PERIOD,TAKT10COMPAREC); break; //4Hz
			case 0x0b: Takt_Custom(TAKT11PERIOD,TAKT11COMPAREC); break; //3Hz
			default: Takt_Switch(DISABLE); break; //输出低电平
		}
  
		takt_mode_temp = takt_mode;
	}
}



