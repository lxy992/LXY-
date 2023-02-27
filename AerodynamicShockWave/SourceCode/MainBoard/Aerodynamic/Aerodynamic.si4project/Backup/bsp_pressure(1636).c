
/*
*********************************************************************************************************
*
*	模块名称 : 气压驱动模块
*	文件名称 : pressure.c
*	版    本   : V0.1
*	说    明   : 气压输出.
*********************************************************************************************************
*/

#include  "config.h"

PressureParams_t PressureParams = PressureParams_Defaults;

PID_PressureTypeDef  Pressure_Ctrl;

/**
 * @brief       气压阀GPIO初始化
 * @param       PID句柄
 * @retval      无
 */
void PressureValve_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     /* Enable peripheral clocks --------------------------------------------------*/
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 
	 /****** PA6、PA7 ********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	 GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief       气压值PID初始化
 * @param       PID句柄
 * @retval      无
 */
void  Pressure_PIDInit(PID_PressureTypeDef *sptr)
{
	sptr->SetValue = SystemParams.uiSetPressureValue;   
	sptr->FdbValue = 0;             
	sptr->iError = 0;                
	sptr->iError_1 = 0;            
    sptr->iError_2 = 0;              
    
    sptr->Kp = 0.2f;              
    sptr->Ki = 0.01f;           
    sptr->Kd = 0.06f;              
    sptr->LimitKi = 1;           
    sptr->iOut = 0;             
	sptr->LastOut = 10;            
	sptr->PresOut = 0;          
	sptr->OutBaseValue = 10;     
    sptr->OutMax = 500;        
    sptr->OutMin = 20;        
}

/**
 * @brief       气压PID计算
 * @param       PID句柄，气压ADC值
 * @retval      无
 */
int Pressure_PIDCalc(PID_PressureTypeDef *sptr,int pressure_adc_value)
{
	sptr->FdbValue = pressure_adc_value;
    sptr->iError =  sptr->SetValue - sptr->FdbValue;  

    if(((sptr->LastOut>=sptr->OutMax)&&(sptr->iError>=0))||((sptr->LastOut<=sptr->OutMin)&&(sptr->iError<=0)))
      sptr->LimitKi=0;
    else
      sptr->LimitKi=1; 
	
    sptr->iOut = (int)(sptr->Kp*(float)(sptr->iError-sptr->iError_1) + (float)sptr->LimitKi*sptr->Ki*(float)sptr->iError 
            + sptr->Kd*(float)(sptr->iError-2*sptr->iError_1+sptr->iError_2));
	
    sptr->PresOut = sptr->LastOut + sptr->iOut;

    sptr->iError_2 = sptr->iError_1;
    sptr->iError_1 = sptr->iError;
    sptr->LastOut = sptr->PresOut; 
	
	sptr->OutMax = 500;
	
    sptr->OutMin = 20;	

    if(sptr->PresOut >= sptr->OutMax)
    {
        return (sptr->OutMax);
    }
    else if(sptr->PresOut <= sptr->OutMin)
    {
       return (sptr->OutMin);
    } 
	
    return  (sptr->PresOut); 
}

/**
 * @brief       排气开关
 * @param       状态
 * @retval      无
 */
void  Venting_Switch(FunctionalState NewState)
{
     if(NewState)
     {
        EN_V2_ON();
	 }
	 else
	 {
        EN_V2_OFF();
	 }
}

/**
 * @brief       气压初始化
 * @param       无
 * @retval      无
 */
void Pressure_Init(void)
{
    Venting_Switch(ENABLE);      
}

/**
 * @brief       气压处理
 * @param       无
 * @retval      无
 */
void Pressure_Handle(void)
{
    static u16 OldValue=0;
	static u8  time_count = 0;
    u16 NewValue=0;
    
    NewValue = SystemParams.uiSetPressureValue;
	
    if(OldValue != NewValue) 
    {
        if(SystemParams.uiSetPressureValue == 0) 
        {
            Pressure_Init();
        }
		
        OldValue = NewValue;
    }

}







