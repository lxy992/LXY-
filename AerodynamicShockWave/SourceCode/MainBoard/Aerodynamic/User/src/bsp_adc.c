
/*
*********************************************************************************************************
*
*	模块名称 : ADC 气压采集模块
*	文件名称 : adc.c
*	版    本   : V0.1
*	说    明   : 采集主缸气压值.
*********************************************************************************************************
*/

#include "config.h"

u16 ActualAalue[CHANNEL_NUM];
u16 AD_Value[SAMPLE_TIME][CHANNEL_NUM];

/**
 * @brief       ADC GPIO初始化
 * @param       无
 * @retval      无
 */
void ADC_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,  ENABLE);  
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	 
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	 /****** PB0 IN8 ********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 
 	 GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief       ADC1初始化
 * @param       无
 * @retval      无
 */
void ADC1_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
	
    /* Enable peripheral clocks --------------------------------------------------*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,  ENABLE);    
	
	/* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure); 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5 );/*Detect master air cylinder */
}

/**
 * @brief       DMA1初始化
 * @param       无
 * @retval      无
 */
void DMA1_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /* DMA1 channel1 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AD_Value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = SAMPLE_TIME*CHANNEL_NUM;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* Enable DMA1 channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

	 /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    /* Enable ADC1 reset calibration register */
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));
    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));
    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
}

void ADC1_Filter(void)
{
    u16 sum = 0;

    for(u8 i = 0; i < CHANNEL_NUM; i++)
    {
        for ( u8 count = 0; count < SAMPLE_TIME; count++)
        {
            sum += AD_Value[count][i];
        }

        ActualAalue[i] = sum / SAMPLE_TIME;
        sum = 0;
    }
}

u16 GET_Voltage(u16 advalue)
{
     
	return (advalue * 3300 / 4096);
	
}

/**
 * @brief       检测主缸气压值
 * @param       主缸电压
 * @retval      无
 */
void Detect_MasterAirCylinderVolt(u16 *MasterVolt)
{
    u32 adc_value = 0;
	
	if(ActualAalue[MASTER_AIR_CYLINDER_CHANNEL] < 161)
    {
        adc_value = 161;
    }
	else
	{
       adc_value = ActualAalue[MASTER_AIR_CYLINDER_CHANNEL];
	}
	
    adc_value  = adc_value-161;        
    adc_value = adc_value*33000/40960; 
    adc_value = adc_value*147/100;
	
	*MasterVolt = adc_value;
}


/**
 * @brief       adc采样数据获取
 * @param       主缸电压
 * @retval      无
 */
void ADC_SamplingAirPressure(void)
{
	
	ADC1_Filter();
	
	/**********主缸气压ADC值***********/
	Detect_MasterAirCylinderVolt(&SystemParams.uiMasterAirCylinderAdc);
}

