
#ifndef _ADC_H
#define _ADC_H

#define  ADC1_DR_Address    ((uint32_t)0x4001244C)

#define  SAMPLE_TIME   10
#define  CHANNEL_NUM   5

#define  MASTER_AIR_CYLINDER_CHANNEL   2
#define  SLAVE_AIR_CYLINDER_CHANNEL    1
#define  FLOW_AIR_CYLINDER_CHANNEL     0
#define  NTC1_CHANNEL     3
#define  NTC2_CHANNEL     4


void  DMA1_Init(void);
void  ADC1_Init(void);
void  ADC_SamplingDataGet(void);

#endif

