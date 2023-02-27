
/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : takt.c
*	��    �� : V0.1
*	˵    �� : �������.
*********************************************************************************************************
*/


#include  "config.h"

/**
 * @brief       ����GPIO��ʼ��
 * @param       ��
 * @retval      ��
 */
void Takt_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     /* Enable peripheral clocks --------------------------------------------------*/
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	  /****** PB5��PB6 ********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 	 GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief       ����1 PWM��ʼ��
 * @param       ��
 * @retval      ��
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
 * @brief       ����2 PWM��ʼ��
 * @param       ����ֵ��Ԥ��Ƶֵ
 * @retval      ��
 */
void TIM3_PWM_Init(u16 arr,u16 psc)  
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 

	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ�� 
	
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
 * @brief       ����״̬
 * @param       ID �� �Ƿ�ʹ��
 * @retval      ��
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
 * @brief       ������������
 * @param       ID �� ����ֵ
 * @retval      ��
 */
void Takt_SetPeriod(u8 id,u16 Period)
{
    if(id == LEFT_HAND)
    {
       TIM_SetAutoreload(TIM3,Period);
       TIM_SetCompare2(TIM3,Period/2);
       TIM_Cmd(TIM3, ENABLE);
	}
	else if(id == RIGHT_HAND)
    {
       TIM_SetAutoreload(TIM4,Period);
       TIM_SetCompare1(TIM4,Period/2);
       TIM_Cmd(TIM4, ENABLE);
	}
}

/**
 * @brief       ����set
 * @param       ID �� ����ֵ���Ƚ�ֵ
 * @retval      ��
 */
void Takt_SetFrq(u8 id,u8 takt_mode)
{
	static	u8	takt_mode_temp = 0;

	if(takt_mode != takt_mode_temp)
	{
		if((takt_mode>=1) && (takt_mode<=17))
		{
		   Takt_Status(id,ENABLE);
		   Takt_SetPeriod(id,47999/takt_mode);
		}
		else
		{
		   Takt_Status(id,DISABLE);
		}  
		takt_mode_temp = takt_mode;
	}
}



