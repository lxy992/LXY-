
/*
*********************************************************************************************************
*
*	模块名称 : 蜂鸣器驱动模块
*	文件名称 : beep.c
*	版    本 : V1.0
*	说    明 : 驱动蜂鸣器.
*********************************************************************************************************
*/

#include  "config.h"

/***********************************
函数功能：设备驱动初始化
输入参数：无
输出参数：无
�?   注：初始化设备驱�?
************************************/
void TIM3_Init(u16 psc,u16 arr)  
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_DeInit(TIM3);

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = arr ;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
    /***** PWM1 Mode configuration: Channel4 PB1 *******/      
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);  
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);
    //TIM_GenerateEvent(TIM3, TIM_EventSource_Update);
    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);
	TIM_SetCompare4(TIM3,0);
}

/*******************************************************************************
* Function Name  : BEEP_Handle(void)
* Description    : 蜂鸣器处理
* Input          : None
* Output         : None
* Return         : None
* Author         : sunny
*******************************************************************************/
void BEEP_Handle(SystemParams_t *p)
{
	static u16  beep_count = 0;
    static u8   beep_num = 0;
	static u8   num_value = 0;
	static u16  cycle_value = 0; 
	static u8   beep_temp = 0;
	
    if(p->ucBeepFlag == 0)
    {
       beep_count = 0;
	   beep_num = 0;
	   return;
	}

	if(p->ucBeepFlag != beep_temp)
	{
       beep_count = 0;
	   beep_num = 0;
	   beep_temp = p->ucBeepFlag;
	}
	
	switch(p->ucBeepFlag)
	{
       case  1:                /*****工作1挡，温度低于40C 3S响2声*****/
	   	num_value = 2;
		cycle_value = 259;     
	   break;
	   case  2:               /*****工作1挡，温度大于40C 1S响2声*****/
	   	num_value = 2; 
		cycle_value = 59;     
	   break;
	   case  3:              /*****工作2挡，温度小于42C 3S响2声*****/ 
	   	num_value = 2;
		cycle_value = 259;
	   break;
	   case  4:              /*****工作2挡，温度大于42C 1S响3声*****/
	   	num_value = 3;
		cycle_value = 39;
	   break;
	   case  5:              /*****工作3挡，温度低于43C 3S响2声*****/
	   	num_value = 2;
		cycle_value = 259;
	   break;
	   case  6:             /*****工作3挡，温度大于43C 1S响4声*****/
	   	num_value = 4;
		cycle_value = 19;
	   break;
	   case  7:             /***** 3S响1声*****/
	   	num_value = 1;
		cycle_value = 279;
	   break;
	   default:break;
	}
	
	if(p->ucBeepFlag)   
	{
	    if(++beep_count<=9)
        {
           TIM_SetCompare4(TIM3,500);
		}
		else
		{
            TIM_SetCompare4(TIM3,0);
		 	if(beep_count==18)
		 	{
			   if(++beep_num >= num_value)
			   {
                  beep_num = 0;
			   }
			   else
			   {
                  beep_count = 0;
			   }
			}
			
			if(beep_count>=cycle_value)
			{
	           beep_count = 0;  
			   p->ucBeepFlag = 0;
			}
		 }
	}
	
}


