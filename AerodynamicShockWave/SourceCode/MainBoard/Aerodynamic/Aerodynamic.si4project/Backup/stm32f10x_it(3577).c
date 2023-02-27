/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "config.h"  

extern void xPortSysTickHandler(void);

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
/*void SVC_Handler(void)
{
}*/

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*void PendSV_Handler(void)
{
}*/



/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)  //1ms        ������ϵͳ�ṩʱ�ӽ���
{  
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
    {
        xPortSysTickHandler();	
    }
}

//timer2  interrupt 
void TIM2_IRQHandler(void)  //100MS
{
    if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update) == 1)
    {	  
	   TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    }
}

//timer3  interrupt 
void TIM3_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == 1)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
    }
}

typedef  void (*pFunction)(void); 
pFunction Jump_To_Application;

/*************************************************************
  Function   : IAP_JumpToApplication 
  Description : ��ת����������
  Input      : none        
  return     : none    
*************************************************************/
void IAP_JumpToApplication(void)
{
	u32 JumpAddress;//��ת��ַ

	JumpAddress = *(__IO u32 *)(STM32_FLASH_BASE + 4);//��ȡ��λ��ַ	
	Jump_To_Application = (pFunction)JumpAddress;//����ָ��ָ��λ��ַ
	__set_MSP(*(__IO u32*)STM32_FLASH_BASE);//��������ջָ��MSPָ����������IAP_ADDR
	Jump_To_Application();//��ת���������봦
	
}

//�����������
void  UPDATE_CommandReceive(u8            data)
{
    static  u8  upg_count = 0;
    u8  upg_cmd[9] = {'F','W','U','P','G','R','A','D','E'};
	u16    update_command_flag = 0;
	
	if(data == upg_cmd[upg_count])
	{
       if(++upg_count==9)
       {
          upg_count = 0;
		  update_command_flag = 0x8080;
		  STMFLASH_Write(FLASH_SAVE_ADDR,&update_command_flag,1);
		  //NVIC_SystemReset();//����
		  USART_DeInit(USART2);
		  IAP_JumpToApplication();
	   }
	}
	else
	{
       upg_count = 0;
	}
}

//USART1   interrupt 
void USART1_IRQHandler(void)
{
    u8  res_data = 0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		
		res_data = (u8)USART_ReceiveData(USART1);

        UART_Receive(&DWIN_COM,res_data);
    }   
}

//USART2   interrupt 
void USART2_IRQHandler(void)
{
    u8  res_data = 0;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		
		res_data = (u8)USART_ReceiveData(USART2);

        UART_Receive(&LEFT_HAND_COM,res_data);
    }   
}

//USART3   interrupt 
void USART3_IRQHandler(void)
{
    u8  res_data = 0;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		
		res_data = (u8)USART_ReceiveData(USART3);

        UART_Receive(&RIGHT_HAND_COM,res_data);
    }   
}


