
/**
 ******************************************************************************
 * @file    main.c
 * @brief
 ******************************************************************************
 ******************************************************************************
***/

#include "config.h"


void CLOCK_Init(void)
{
    CLK_DeInit();
    CLK_HSICmd(ENABLE);
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
    CLK_CCOCmd(DISABLE);
}

void Interrupt_Configration(void)
{
    disableInterrupts();//关中断
    //设置中断优先级，复位默认的优先级最高。
    //ITC_PRIORITYLEVEL_1---->ITC_PRIORITYLEVEL_3 低到高
    ITC_SetSoftwarePriority(ITC_IRQ_TIM4_OVF, ITC_PRIORITYLEVEL_2);
//      asm("rim");//使能中断
}

/*******************************************************************************
* Function Name  : IWDG_Reload
* Description    : 看门狗喂狗
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IWDG_Reload(void)
{
    IWDG_ReloadCounter();
}

#if 1
void IWDG_Init(void)
{
    //时钟128K的LSI,看门狗预分频256，重装载周期500MS
    IWDG_Enable();
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload(250);
    IWDG_ReloadCounter();
}
#endif

/*******************************************************************************
* Function Name  : Write_Bootloader_Enable
* Description    : upgrade program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Write_Bootloader_Enable(void)
{
    FLASH_Unlock(FLASH_MEMTYPE_DATA);    //if(FLASH_ReadByte(0x4010)!=0x55)
    if(FLASH_ReadOptionByte(0x487E) != 0x55)
    {
        FLASH_ProgramOptionByte(0x487E, 0x55);
        FLASH_ProgramOptionByte(0x487F, 0xAA);
    }
    FLASH_Lock(FLASH_MEMTYPE_DATA);
}

void EEPROM_INIT(void)
{
    FLASH_DeInit();
//  FLASH_Unlock(FLASH_MEMTYPE_DATA);
//  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
}

int main( void )
{
    CLOCK_Init();
    EEPROM_INIT();
    Key_Init();
    OLED_Init();
    Timer_Init();
    UART_Init();
    ADC_Init();
    Paremeter_Init();
    Interrupt_Configration();
    UPG_ProcTime = 120;
    IWDG_Init();
	Read_EEPROM_Data();
	
    enableInterrupts();//开中断

	
    while(1)
    {
        if(Flag.HandConnect)
        {
            Protocol_PrintfData(COMMANDSTATE_SEND, 0x22, &Flag.HandConnect, 1);//主动给手柄发确认连接
            Flag.HandConnect = 0;
        }
        Key_Read();
        USART_Process();
        OLED_Process();
        IWDG_Reload();
    }
}

#ifdef   USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    //printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    /* Infinite loop */
    //  while (1)
    // {
    // }
}

#endif

