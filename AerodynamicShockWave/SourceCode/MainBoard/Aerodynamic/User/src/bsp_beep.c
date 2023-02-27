
/*
*********************************************************************************************************
*
*	模块名称 : 蜂鸣器驱动模块
*	文件名称 : beep.c
*	版    本 : V1.0
*	说    明 : 驱动蜂鸣器.
*********************************************************************************************************
*/

#include  "config.h"

/**
 * @brief       beep GPIO初始化
 * @param       无
 * @retval      无
 */
void  BEEP_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     /* Enable peripheral clocks --------------------------------------------------*/
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	 /****** PA1 ********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	 GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief       beep 处理
 * @param       无
 * @retval      无
 */
void BEEP_Handle(SystemParams_t *p)
{
	static uint8_t  beep_count = 0;
    static uint8_t  beep_num = 0;
	
	uint8_t  beep_set_num_value = 0;
	
	if(p->ucBeepFlag == 0)
    {
       return;
	}

	if(p->ucBeepFlag == 1)    /*****响一声*****/
	{
	    if(++beep_count <= 10) 
		{  
            if(beep_count == 1)
            {
               BEEP_ON();
			}
		}
		else 
		{
		   BEEP_OFF();
		   beep_count = 0;
		   p->ucBeepFlag = 0; 
		}
	}
	else if(p->ucBeepFlag == 2)   //响两声
	{
        if(++beep_count<=16)
        {
            if(beep_count == 1)
            {
               BEEP_ON();
			}
		}
		else
		{
            BEEP_OFF();
		 	if(beep_count>=33)
		 	{
               beep_count = 0;
			   if(++beep_num == 2)
			   {
                  beep_num = 0;
				  p->ucBeepFlag  = 0;
			   }
			}
		}
	}
	else if(p->ucBeepFlag == 3)  
	{
        if(++beep_count <= 20) 
		{  
           if(beep_count == 1)
           {
               BEEP_ON();
		   }
		}
		else 
		{
           if(beep_count == 21) 
           {
              BEEP_OFF();
		   }
		   if(beep_count>=40)
		   {
              beep_count = 0;
			  p->ucBeepFlag = 0; 
		   }
		} 
	}
	else if(p->ucBeepFlag >= 4)  
	{
        if(p->ucBeepFlag == 4)
        {
            beep_set_num_value = 2;
		}
		else if(p->ucBeepFlag == 5)
		{
            beep_set_num_value = 3;
		}
		
		if(++beep_count <= 60) 
		{  
           if(beep_count == 1)
           {
               BEEP_ON();
		   }
		}
		else 
		{
           if(beep_count == 61) 
           {
              BEEP_OFF();
		   }
		   
		   if(beep_count>=120)
		   {
              beep_count = 0;
			  if(++beep_num>=beep_set_num_value)
			  {
                beep_num = 0;
				p->ucBeepFlag = 0; 
			  }
		   }
		} 
	}
}



