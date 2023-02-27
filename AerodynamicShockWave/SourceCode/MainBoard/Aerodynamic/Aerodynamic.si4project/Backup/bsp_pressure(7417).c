
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
 * @brief       KEY GPIO��ʼ��
 * @param       ��
 * @retval      ��
 */

void  Pressure_PIDInit(PID_PressureTypeDef *sptr)
{
	sptr->SetValue = SystemParams.uiSetPressureValue;   //设定气压目标�?
	sptr->FdbValue = 0;               //反馈温度�?
	sptr->iError = 0;                 //增量当前误差�?
	sptr->iError_1 = 0;               //最近误差�?
    sptr->iError_2 = 0;              //上一个误差�?
    
    sptr->Kp = 0.2f;                //比例常数
    sptr->Ki = 0.01f;               //积分常数
    sptr->Kd = 0.06f;               //微分常数
    sptr->LimitKi = 1;             //积分限制�?
    sptr->iOut = 0;                //PID增量输出�?
	sptr->LastOut = 10;             //PID增量最近输出�?
	sptr->PresOut = 0;             //PID当前输出�?
	sptr->OutBaseValue = 10;       //输出基准�?
    sptr->OutMax = 500;           // 输出最大阀值控制�?
    sptr->OutMin = 20;           // 输出最小阀值控制�?
}

/*******************************************************************************
* Function Name  : Pressure_PIDCalc(PID_TypeDef *sptr,int pressure_adc_value)
* Description    : PID计算
* Input          : PID 参数 ，adc�?
* Output         : 导通时�?
* Return         : None
* Author         : sunny
*******************************************************************************/
int Pressure_PIDCalc(PID_PressureTypeDef *sptr,int pressure_adc_value)
{
	sptr->FdbValue = pressure_adc_value;
    sptr->iError =  sptr->SetValue - sptr->FdbValue;  //当前误差�?

    /*遇限削弱积分即控制量Uk进入饱和区，便停止进行增大的积分项运算，而只进行使积分减少的运算*/
    if(((sptr->LastOut>=sptr->OutMax)&&(sptr->iError>=0))||((sptr->LastOut<=sptr->OutMin)&&(sptr->iError<=0)))
      sptr->LimitKi=0;
    else
      sptr->LimitKi=1; 
	
   /*PID计算*/
    sptr->iOut = (int)(sptr->Kp*(float)(sptr->iError-sptr->iError_1) + (float)sptr->LimitKi*sptr->Ki*(float)sptr->iError 
            + sptr->Kd*(float)(sptr->iError-2*sptr->iError_1+sptr->iError_2));
	
    /* 存储误差，用于下次计�?*/
   
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

/*******************************************************************************
* FunctionName   : Venting_Switch()
* Description    : 排气开�?
* Notice         : NO.
* Param[in]      : NewState--- 若ENABLE，开启排�?若DISABLE，关闭排�?
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
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

/*******************************************************************************
* FunctionName   : Intake_Switch()
* Description    : 进气开�?
* Notice         : NO.
* Param[in]      : NewState--- 若ENABLE，开启进�?若DISABLE，关闭进�?
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void  Intake_Switch(FunctionalState NewState)
{
     if(NewState)
     {
        EN_V1_ON();
	 }
	 else
	 {
        EN_V1_OFF();
	 }
}

/*******************************************************************************
* FunctionName   : Pressure_Init
* Description    : 初始化气压配�?
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Pressure_Init(void)
{
    Venting_Switch(ENABLE);        //开启排�?
    Intake_Switch(DISABLE);        //关闭进气
}

/*******************************************************************************
* FunctionName   : Pressure_Handle
* Description    : 气压处理.
* Notice         : NO.
* Param[in]      : Target---传入一个气压目标�?
* Output         : 稳定到目标�?
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Pressure_Handle(void)
{
    static u16 OldValue=0;
	static u8  time_count = 0;
    u16 NewValue=0;
    
    NewValue = SystemParams.uiSetPressureValue;
	
    if(OldValue != NewValue) //若值发生变化就处理
    {
        if(SystemParams.uiSetPressureValue == 0) //若值为0，则处理
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
* FunctionName   : Pressure_IntakeTimeCount
* Description    : 气压进气时间计数.
* Notice         : 1MS中断时间计数
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void  Pressure_IntakeTimeCount(void)
{

    if(PressureParams.TimerStart)//进气开始在定时器开�?
    {
        PressureParams.TimeMin++;
        if (((PressureParams.TimeMax-PressureParams.TimeMin)==10) && (PressureParams.InAirStatus==1))
        {
            PressureParams.ReadPressureValueFlag = 1;
        }
        if(PressureParams.TimeMin >= PressureParams.TimeMax)
        {
            PressureParams.TimeMin      = 0;//定时器最小�?
            PressureParams.InAirStatus  = 0;//进气的状态为0表示结束，为1表示进气�?
            PressureParams.OutAirStatus = 0;//排气的状态为0表示结束，为1表示排气�?
            PressureParams.TimerStart     = 0;//时间到，自动关闭定时�?
            PressureParams.Time500ms = 0;  //500ms计时清零
        }
    }
	
    PressureParams.Time500ms++;
    if(PressureParams.Time500ms >= 500)
    {
        PressureParams.Time500ms = 0;
        PressureParams.ReadPressureValueFlag = 1;
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
    MasterAirCylinder_Switch(DISABLE); //关闭限压开�?
    SystemParams.uiMasterCompareCylinder = MAXDRUCK;  //主缸气压�?
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
void LimitPressure_Handle(void)          //y=14x+620   x:气压�?y:采样电压�?
{   
    u16  pressure =0;
	static u8 master_detetct_time_count = 0;
    static u8 venting_count = 0;
	
    if(SystemParams.ucLimitPressureSwitch == 1) //打开限压开�?  
    {
        if(++master_detetct_time_count>=3)     //75MS    
		{
		    master_detetct_time_count = 0;
		}
		else
		{
            return;
		}
            
		pressure = SystemParams.uiMasterAirCylinder;      //主缸气压
		
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



