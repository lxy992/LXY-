
#ifndef _ADC_H
#define _ADC_H

#define  ADC1_DR_Address    ((uint32_t)0x4001244C)

#define  SAMPLE_TIME   10
#define  CHANNEL_NUM   1

#define  MASTER_AIR_CYLINDER_CHANNEL   0


void  ADC_GPIO_Init(void);
void  DMA1_Init(void);
void  ADC1_Init(void);
void  ADC_SamplingAirPressure(void);

#endif

