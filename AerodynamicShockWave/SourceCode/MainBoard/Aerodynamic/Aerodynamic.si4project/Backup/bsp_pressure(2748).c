
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

	if(SystemParams.uiSetPressureValue)
    {
       Pressure_Custom(SystemParams.uiSetPressureValue);
	   if(SystemParams.ucCurrentFlowModeBuffer != 5)
	   {
	       SystemParams.ucCurrentTaktMode = SystemParams.ucCurrentTaktModeBuffer;
	       SystemParams.ucCurrentFlowMode = SystemParams.ucCurrentFlowModeBuffer;
	   }
	   time_count = 0;
    }
	else
	{
        if(SystemParams.ucCurrentFlowModeBuffer == 5)
	    {
            if(++time_count>=40)
            {
               SystemParams.ucCurrentTaktMode = 0;
	           SystemParams.ucCurrentFlowMode = SystemParams.ucCurrentFlowModeBuffer;
			   time_count = 0;
			}
	    } 
		else
		{
           SystemParams.ucCurrentTaktMode = SystemParams.ucCurrentTaktModeBuffer;
	       SystemParams.ucCurrentFlowMode = SystemParams.ucCurrentFlowModeBuffer;  
		}
	} 

}

/*******************************************************************************
* FunctionName   : MasterAirCylinder_Switch
* Description    : 继电器的开关函�?
* Notice         : NO.
* Param[in]      : NewState---为枚举类型，ENABLE开启继电器，DISABLE关闭继电�?
                     关继电器，主缸气压增大；开继电器，主缸泄气�?
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void  MasterAirCylinder_Switch(FunctionalState NewState)
{
    if(NewState)
    {
      // MASTER_CYLINDER_ON();
	   #if 1
	   MASTER_CYLINDER_LIMIT1_ON();
	   #else
	   MASTER_CYLINDER_LIMIT2_ON();
	   #endif
	}
	else
	{
      // MASTER_CYLINDER_OFF();
        #if 1
	   MASTER_CYLINDER_LIMIT1_OFF();
	   #else
	   MASTER_CYLINDER_LIMIT2_OFF();
	   #endif
	}
}

/*******************************************************************************
* FunctionName   : LimitPressure_Init
* Description    : 初始化限�?
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void LimitPressure_Init(void) 
{
    MasterAirCylinder_Switch(DISABLE); 
    SystemParams.uiMasterCompareCylinder = MAXDRUCK;  
    SystemParams.ucLimitPressureSwitch = 1;
}

/*******************************************************************************
* FunctionName   : LimitPressure_Handle
* Description    : 限压处理
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void LimitPressure_Handle(void)          
{   
    u16  pressure =0;
	static u8 master_detetct_time_count = 0;
    static u8 venting_count = 0;
	
    if(SystemParams.ucLimitPressureSwitch == 1)
    {
        if(++master_detetct_time_count>=3)     //75MS    
		{
		    master_detetct_time_count = 0;
		}
		else
		{
            return;
		}
            
		pressure = SystemParams.uiMasterAirCylinder;    
		
        if(pressure>=SystemParams.uiMasterCompareCylinder) 
        {
              if(++venting_count>=2) 
              {
                MasterAirCylinder_Switch(ENABLE); 
				venting_count = 0;
			  } 
        }
        else
        {
            MasterAirCylinder_Switch(DISABLE);     
			venting_count = 0;
        }
    }

	#if 0
	if(SystemParams.uiMasterAirCylinder>=2700)
	{
       //MASTER_CYLINDER_LIMIT1_ON(); 
	   MASTER_CYLINDER_LIMIT2_ON();
	}
	else
	{
       MASTER_CYLINDER_LIMIT1_OFF();
	}
	#endif
}



