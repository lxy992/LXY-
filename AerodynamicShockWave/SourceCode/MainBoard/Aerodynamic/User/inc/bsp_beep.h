
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
   
#define     BEEP_ON()       GPIOA->BSRR=1<<1
#define     BEEP_OFF()      GPIOA->BRR=1<<1 

void  BEEP_GPIO_Init(void);
void  BEEP_Handle(SystemParams_t *p);

#endif

