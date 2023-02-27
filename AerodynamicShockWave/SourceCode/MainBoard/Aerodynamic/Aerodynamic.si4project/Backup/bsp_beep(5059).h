
/*
*********************************************************************************************************
*
*	模块名称 : 蜂鸣器模块
*	文件名称 : beep.h
*
*********************************************************************************************************
*/

#ifndef __BEEP_H
#define __BEEP_H
   
#define     BEEP_ON()       GPIOB->BSRR=1<<1
#define     BEEP_OFF()      GPIOB->BRR=1<<1 

void  TIM3_Init(u16 psc,u16 arr);
void  BEEP_Handle(SystemParams_t *p);

#endif

