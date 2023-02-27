
/*
*********************************************************************************************************
*
*	模块名称 : 系统初始化
*	文件名称 : bsp_sys.c
*	版    本 : V1.0
*	说    明 : 系统初始化.
*********************************************************************************************************
*/

#include  "config.h"


/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;

    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if(HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

        /* Flash 2 wait state */
        FLASH_SetLatency(FLASH_Latency_2);

        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div1);

        /* PCLK1 = HCLK/2 */
        RCC_PCLK1Config(RCC_HCLK_Div2);

        /* PLLCLK = 8MHz * 6 = 48MHz */
        RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);
        /* Enable PLL */
        RCC_PLLCmd(ENABLE);

        /* Wait till PLL is ready */
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while(RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
	
    /* Enable PWR and BKP clock */
   // RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
   
   // RCC_MCOConfig(RCC_MCO_SYSCLK);   /*output clock signal*/
}
/*******************************************************************************
* Function Name  :TIM2_Timer_Init(u16 arr,u16 psc)
* Description    :  timer2 init 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_Timer_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//TIM2初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //周期值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 
	
	TIM_Cmd(TIM2, ENABLE);  //ê1?üTIMx					 
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures NVIC and Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    //NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
    #if UPDATE
       NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);
	#else 
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);
	#endif

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

	/* Disable the OTG_FS_IRQn Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);

	/* Enable the USARTz Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    /* Enable the USARTz Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	/* Enable the USARTz Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Enable the USARTz Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
    
      /* Enable the TIM2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);  
    
}


/*******************************************************************************
* Function Name  :void SysTick_Configuration(void)
* Description    : Config the systick timer for 1ms interrupt    
* Input          : None
* Output         : None
* Return         : None
* Author         :baolin.yang
*******************************************************************************/
void SysTick_Configuration(void)              //1ms
{
    /* Setup SysTick Timer for 1 msec interrupts  */
    if (SysTick_Config(48000))   //SysTick配置函?
    {
        /* Capture error */
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);//SysTick窒断雅先级设置

}

void IWDG_Init(u8 prer, u16 rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    IWDG_SetPrescaler(prer);

    IWDG_SetReload(rlr);

    IWDG_ReloadCounter();

    IWDG_Enable();
}

void IWDG_Feed(void)
{
    IWDG_ReloadCounter();
}

int fputc(int ch, FILE *f)
{
	  USART_SendData(UART4, (unsigned char) ch);// UART5 可以换成 UART5 等
	  while (!(UART4->SR & USART_FLAG_TXE));
	  return (ch);
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
     ADC_GPIO_Init();
	 BEEP_GPIO_Init();
     KEY_GPIO_Init();
	 PressureValve_GPIO_Init();
     Takt_GPIO_Init();
	 HandStatusDetect_GPIO_Init();
     EN_12V_GPIO_Init();
     DWIN_POWER_GPIO_Init();
     AirPump_GPIO_Init();
     LED_GPIO_Init();
     FAN_GPIO_Init();
     HAND_POWER_GPIO_Init();
}
/*******************************************************************************
* Function Name  : void DeviceInit(void)
* Description    : System Device Init
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SystemDeviceInit(void)
{
    ADC1_Init();
	DMA1_Init();
	TIM4_PWM_Init(1000-1,48000-1);       // output frq 1Hz  用于控制节拍1
    TIM3_PWM_Init(1000-1,48000-1);       // output frq 1Hz  用于控制节拍2
    TIM2_Timer_Init(1000-1,4800-1);      //100MS
    UART_Init(USART1 ,115200);           //Uart1
    UART_Init(USART2 ,115200);           //Uart2
    UART_Init(USART3,115200);           //Uart3
    UART_Init(UART4 ,115200);           //Uart4  BaudRate 115200  Debug
}

/*******************************************************************************
* Function Name  : void SystemHardInit(void)
* Description    : MCU' RCC,NVIC,TIMER,GPIO Init
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SystemHardInit(void)
{
    /*System clocks configuration ---------------------------------------------*/
    RCC_Configuration();
    /*NVIC configuration ------------------------------------------------------*/
    NVIC_Configuration();
    /*Timer Configuration------------------------------------------------------*/
    SysTick_Configuration();
    /*GPIO Configuration------------------------------------------------------*/
    GPIO_Configuration();
}

/*---------------------------------------------------------------------------
* 函数名  : SystemSoftInit
* 描述    : 软件系统初始化
* 输入    : 无
* 输出    : 无
* 返回    : 无
---------------------------------------------------------------------------*/	
void SystemSoftInit(void)
{
    System_Init();
	UART_ReceiveValueInit(&DWIN_COM,UART_DAT_LENGTH);
    DEVICE_VarInit(&DWIN_Code);
	UART_ReceiveValueInit(&LEFT_HAND_COM,UART_DAT_LENGTH);
    DEVICE_VarInit(&LEFT_HAND_Code);
	UART_ReceiveValueInit(&RIGHT_HAND_COM,UART_DAT_LENGTH);
    DEVICE_VarInit(&RIGHT_HAND_Code);
}

/*********字符型转成16进制*************/
char ConvertHexChart(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;  // 0x30 对应 ‘0’
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return ch-ch;//不在0-f范围内的会发送成0
}



