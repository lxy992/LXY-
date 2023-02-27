
/*
*********************************************************************************************************
*
*	模块名称 : 节拍控制模块
*	文件名称 : takt.h
*
*********************************************************************************************************
*/

#ifndef __TAKT_H
#define __TAKT_H

#define  EN_V3_ON()        GPIOB->BSRR=1<<5
#define  EN_V3_OFF()       GPIOB->BRR=1<<5

#define  EN_V4_ON()        GPIOB->BSRR=1<<6
#define  EN_V4_OFF()       GPIOB->BRR=1<<6

#define  LEFT_HAND     1
#define  RIGHT_HAND    2

void  Takt_GPIO_Init(void);
void  TIM4_PWM_Init(u16 arr,u16 psc);
void  TIM3_PWM_Init(u16 arr,u16 psc);
void  Takt_SetFrq(u8 id,u8 takt_mode);
void  HAND_StartWork();

#endif

