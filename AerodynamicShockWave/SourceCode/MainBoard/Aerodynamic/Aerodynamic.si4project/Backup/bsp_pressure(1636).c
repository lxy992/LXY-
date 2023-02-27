
/*
*********************************************************************************************************
*
*	ģ������ : ��ѹ����ģ��
*	�ļ����� : pressure.c
*	��    ��   : V0.1
*	˵    ��   : ��ѹ���.
*********************************************************************************************************
*/

#include  "config.h"

PressureParams_t PressureParams = PressureParams_Defaults;

PID_PressureTypeDef  Pressure_Ctrl;

/**
 * @brief       ��ѹ��GPIO��ʼ��
 * @param       PID���
 * @retval      ��
 */
void PressureValve_GPIO_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     /* Enable peripheral clocks --------------------------------------------------*/
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 
	 /****** PA6��PA7 ********/  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;		     
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	 GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief       ��ѹֵPID��ʼ��
 * @param       PID���
 * @retval      ��
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
 * @brief       ��ѹPID����
 * @param       PID�������ѹADCֵ
 * @retval      ��
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
 * @brief       ��������
 * @param       ״̬
 * @retval      ��
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
 * @brief       ��ѹ��ʼ��
 * @param       ��
 * @retval      ��
 */
void Pressure_Init(void)
{
    Venting_Switch(ENABLE);      
}

/**
 * @brief       ��ѹ����
 * @param       ��
 * @retval      ��
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







