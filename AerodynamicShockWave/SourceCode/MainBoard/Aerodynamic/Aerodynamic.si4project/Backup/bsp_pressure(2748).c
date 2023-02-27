
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
* Description    : 缁х數鍣ㄧ殑寮�鍏冲嚱鏁?
* Notice         : NO.
* Param[in]      : NewState---涓烘灇涓剧被鍨嬶紝ENABLE寮�鍚户鐢靛櫒锛孌ISABLE鍏抽棴缁х數鍣?
                     鍏崇户鐢靛櫒锛屼富缂告皵鍘嬪澶э紱寮�缁х數鍣紝涓荤几娉勬皵鍘?
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
* Description    : 鍒濆鍖栭檺鍘?
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
* Description    : 闄愬帇澶勭悊
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



