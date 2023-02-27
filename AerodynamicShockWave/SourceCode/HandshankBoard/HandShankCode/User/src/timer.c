/**
  ******************************************************************************
  * @file    timer.c
  * @author  LL
  * @version V0.0.0
  * @date    2016-03-21
  * @brief
  ******************************************************************************
  ******************************************************************************
***/

#include "config.h"

typedef struct
{
    u16 Heartbeat;
} Delay_StructType;

Delay_StructType Delay;
unsigned char syscnt_10Ms;//ϵͳ10Ms����

#define T_10MS 1
#define T_50MS T_10MS*5
#define T_100MS T_10MS*10
#define T_200MS T_10MS*20
#define T_300MS T_10MS*30
#define T_500MS T_10MS*50
#define T_1S T_500MS*2
#define T_2S T_1S*2
#define T_3S T_1S*3
#define T_30S T_1S*30
#define T_1M T_1S*60

#define PWMFRE 18432
#define T16CONT 65536




/*******************************************************************************
* Function Name  : Timer4_Init
* Description    : ��ʼ����ʱ�����ж�����10mS


��ʱ��10mS��ʱ���㷽��
ת��ʱ��Hz�����㣬����8MHz = 8000000Hz
�����붨ʱ��ϵ 10mS 100, 50mS 20, 100mS 10, 1S 1
��Ϊ1Hz��1S��Ӧ������1S/10mS=100��
��Ƶ��Ϊ100Hzʱ������100/��1S/10mS�� = 1�ξ���10���롣
��Ƶ��Ϊ25KHzʱ������25K/��1S/10mS�� = 250�ξ���10���롣
���� x = 100
��ʱ��ʱ��Ƶ��Focs = 7.3728M
Ԥ��ƵֵPrescal = 1024
����ֵT
  T = Fosc/Prescal/x = 7372800/1024/100 = 72
�������ϼ�������Ķ�ʱ����������TCNT = 0xFF - T = 0xFF-72
���������Զ����صģ�����Ҫ���ж�������������TCNT�������Զ����ص���Ҫ��������

* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Timer4_Init(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 125);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    TIM4_Cmd(ENABLE);
}
/*
��ʱ��4����жϺ���������ϵͳ��ʱ��
*/
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
    static u16 Temperature_TimeCnt = 100;//1s��һ���¶�
    static u8  send_eeprom_value_flag = 0;
	
    TIM4->SR1 = ~TIM4_SR1_UIF;

    syscnt_10Ms++;
    if(syscnt_10Ms < 10) return;
    syscnt_10Ms = 0;

    Flag.key_scan = TRUE;
    Flag.ADC_Scan = TRUE;

    GlobalVar.SysTime++;

    Delay.Heartbeat ++;
    if(Delay.Heartbeat >= T_2S)
    {
        Flag.Heartbeat = TRUE;
        Delay.Heartbeat = 0;
		#ifdef MCU_UPG_PROC
		if(UPG_ProcTime) UPG_ProcTime--;
		#endif
    }

    if(1 == (Temperature_TimeCnt --))
    {
        Temperature_Read();
		if(send_eeprom_value_flag < 2)
		{
		  SendEepromShockCnt();
		   send_eeprom_value_flag++;
		}
		
        Temperature_TimeCnt = 100;
    }

}
/*******************************************************************************
* Function Name  : Timer_Init
* Description    : ��ʼ����ʱ��
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-21
* Author         : LL
*******************************************************************************/
void Timer_Init(void)
{
    Timer4_Init();
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */

