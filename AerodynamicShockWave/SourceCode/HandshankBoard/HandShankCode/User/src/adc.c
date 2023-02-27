/**
  ******************************************************************************
  * @file    adc.c
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/
#include "config.h"

/*******************************************************************
FUNCTION:��β�����ƽ��ֵ������18��
*******************************************************************/
unsigned int ADC_ConverStable(void)
{
	unsigned char i;
	unsigned int adcval;
	unsigned int minval=0xFFFF;
	unsigned int maxval=0;
	unsigned int averageval;
	unsigned long int totalval=0;
	
	for (i=0; i<18; i++)
	{
		adcval = ADC_ConverSingle();
		totalval += adcval;
		if (minval>adcval) minval = adcval;
		if (maxval<adcval) maxval = adcval;
	}
	totalval = totalval - minval - maxval;
	averageval = totalval >> 4;
	
	return averageval;
}

/*******************************************************************************
* Function Name  : ADC_ConverSingle 
* Description    : ADC����һ��ת��
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-04-11
* Author         : LL
*******************************************************************************/
unsigned int ADC_ConverSingle(void)
{
	unsigned int adc_value;

	ADC1_StartConversion();//����һ��ת��
	while(!(ADC1->CSR&0x80));//�ȴ�ת�����
	ADC1_ClearFlag(ADC1_FLAG_EOC); //EOC���㣬�����¿�ʼת��
	adc_value = ADC1->DRL;
	adc_value += ADC1->DRH<<8;
	ADC1_StartConversion();//����һ��ת��
	while(!(ADC1->CSR&0x80));//�ȴ�ת�����
	ADC1_ClearFlag(ADC1_FLAG_EOC); //EOC����
	adc_value = ADC1->DRL;
	adc_value += ADC1->DRH<<8;
	
	return adc_value;
}

/*******************************************************************************
* Function Name  : ADC_Init 
* Description    : ADC��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-04-11
* Author         : LL
*******************************************************************************/
void ADC_Init(void)
{
    GPIO_Init(GPIOB,GPIO_PIN_0,GPIO_MODE_IN_FL_NO_IT);
    //ѡ��ADCͨ��12������ת���������Ҷ��룬���ⲿ��������ֹʩ���ش�����
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC,ENABLE);
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_0, ADC1_PRESSEL_FCPU_D8, \
            ADC1_EXTTRIG_GPIO, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL0 ,\
            DISABLE);
	ADC1_ITConfig (ADC1_IT_EOC,DISABLE);//��ֹת�������ж�
	ADC1_Cmd (ENABLE);
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */

