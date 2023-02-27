
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

/*******************************************************************************
* Function Name  : Pressure_PIDInit(PID_TypeDef *sptr)
* Description    : 气压PID初始化
* Input          : pid 参数
* Output         : None
* Return         : None
* Author         : sunny
*******************************************************************************/
void  Pressure_PIDInit(PID_PressureTypeDef *sptr)
{
	sptr->SetValue = SystemParams.uiSetPressureValue;   //设定气压目标值
	sptr->FdbValue = 0;               //反馈温度值
	sptr->iError = 0;                 //增量当前误差值
	sptr->iError_1 = 0;               //最近误差值 
    sptr->iError_2 = 0;              //上一个误差值
    
    sptr->Kp = 0.2f;                //比例常数
    sptr->Ki = 0.01f;               //积分常数
    sptr->Kd = 0.06f;               //微分常数
    sptr->LimitKi = 1;             //积分限制值
    sptr->iOut = 0;                //PID增量输出值
	sptr->LastOut = 10;             //PID增量最近输出值
	sptr->PresOut = 0;             //PID当前输出值
	sptr->OutBaseValue = 10;       //输出基准值
    sptr->OutMax = 500;           // 输出最大阀值控制值
    sptr->OutMin = 20;           // 输出最小阀值控制值
}

/*******************************************************************************
* Function Name  : Pressure_PIDCalc(PID_TypeDef *sptr,int pressure_adc_value)
* Description    : PID计算
* Input          : PID 参数 ，adc值
* Output         : 导通时间
* Return         : None
* Author         : sunny
*******************************************************************************/
int Pressure_PIDCalc(PID_PressureTypeDef *sptr,int pressure_adc_value)
{
	sptr->FdbValue = pressure_adc_value;
    sptr->iError =  sptr->SetValue - sptr->FdbValue;  //当前误差值

    /*遇限削弱积分即控制量Uk进入饱和区，便停止进行增大的积分项运算，而只进行使积分减少的运算*/
    if(((sptr->LastOut>=sptr->OutMax)&&(sptr->iError>=0))||((sptr->LastOut<=sptr->OutMin)&&(sptr->iError<=0)))
      sptr->LimitKi=0;
    else
      sptr->LimitKi=1; 
	
   /*PID计算*/
    sptr->iOut = (int)(sptr->Kp*(float)(sptr->iError-sptr->iError_1) + (float)sptr->LimitKi*sptr->Ki*(float)sptr->iError 
            + sptr->Kd*(float)(sptr->iError-2*sptr->iError_1+sptr->iError_2));
	
    /* 存储误差，用于下次计算 */
   
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
* Description    : 排气开关
* Notice         : NO.
* Param[in]      : NewState--- 若ENABLE，开启排气;若DISABLE，关闭排气
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
* Description    : 进气开关
* Notice         : NO.
* Param[in]      : NewState--- 若ENABLE，开启进气;若DISABLE，关闭进气
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
* Description    : 初始化气压配置
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Pressure_Init(void)
{
    Venting_Switch(ENABLE);        //开启排气
    Intake_Switch(DISABLE);        //关闭进气
}

/*******************************************************************************
* FunctionName   : Pressure_Custom
* Description    : 自定义气压值，传入一个需要的气压值，它会通过进气和排气匹配稳定
                     到传入值.
* Notice         : 此函数必须一直在大循环中工作,因为它要实时的检测比较气压值
* Param[in]      : Target---传入一个气压目标值
* Output         : 稳定到目标值
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void Pressure_Custom(u16 Target)  //          y=19.3x-115  x气压值    y电压值 
{
    u16 adc_temp = 0;    //比较AD数值临时变量
    int pid_control_time_value = 0;
	float target_slave_pressure_value = 0;
	u16 target_slave_air_cylinder_value = 0;
	u16 target_temp = 0;
	
    if(PressureParams.OutAirStatus == 0)
    {
       Venting_Switch(DISABLE); //关闭排气
    }
	
    if(PressureParams.InAirStatus == 0)
    {
       Intake_Switch(DISABLE); //关闭进气
    }

    if(PressureParams.ReadPressureValueFlag == 1)
    {
           target_temp = Target;
		   
		   if(target_temp>0)
		   {
		      target_slave_pressure_value = (float)((float)(target_temp+115)/19.3f); 
		   }
		   
		 //  printf("target_slave_pressure_value:%.1f \r\n",target_slave_pressure_value);
		   //y=6.48x-103  x气压值    y电压值
		   if(target_slave_pressure_value>=16)
		   {
             target_slave_air_cylinder_value = (u16)(6.48f*target_slave_pressure_value-103.0f);
		   }
		   else
		   {
              if(target_slave_pressure_value>0)
              {
                 target_slave_air_cylinder_value = 20;
			  }
			  else
			  {
                 target_slave_air_cylinder_value = 0;
			  }
			  
		   }

		Pressure_Ctrl.SetValue = target_slave_air_cylinder_value; 
		
	    pid_control_time_value = Pressure_PIDCalc(&Pressure_Ctrl,SystemParams.uiSlaveAirCylinder);

		#if PID_DEBUG_WINDOW

		printf("SetPressureValue:%d  SlaveAirCylinder:%d  pressure_control_time:%d\r\n",Pressure_Ctrl.SetValue,SystemParams.uiSlaveAirCylinder,pid_control_time_value);

		#endif
		
		PressureParams.Time500ms = 0;
		
        //判断是否需要排气
        if(SystemParams.uiSlaveAirCylinder >= Pressure_Ctrl.SetValue)
        {
            Intake_Switch(DISABLE);          //关闭进气
            PressureParams.TimerStart    = 0;
            PressureParams.InAirStatus = 0;
            
            adc_temp = SystemParams.uiSlaveAirCylinder-Pressure_Ctrl.SetValue;
            if(adc_temp >=50)   //容许误差20mV
            {
                Venting_Switch(ENABLE); //开启排气
                PressureParams.TimeMin      = 0;//设定 1ms 定时器初值为0
                PressureParams.TimeMax = 20;
                PressureParams.TimerStart     = 1;
                PressureParams.OutAirStatus = 1;
            }
        }
        else//判断是否需要进气
        {
            Venting_Switch(DISABLE);         //关闭排气
            
            PressureParams.TimerStart    = 0;
            PressureParams.OutAirStatus  = 0;
            
            adc_temp = Pressure_Ctrl.SetValue - SystemParams.uiSlaveAirCylinder;
            if (adc_temp < 50)
            {
                Intake_Switch(DISABLE);      //关闭进气
                PressureParams.InAirStatus = 0;
            }
            else 
            {
                Intake_Switch(ENABLE);       //开启进气
                PressureParams.TimeMin      = 0;//设定 1ms 定时器初值为0
                PressureParams.TimeMax      = pid_control_time_value;//设定 1ms 定时器计数最大值 100ms
                PressureParams.TimerStart     = 1;
                PressureParams.InAirStatus  = 1;
            }
        }
		
        PressureParams.ReadPressureValueFlag = 0;
    }   
}

/*******************************************************************************
* FunctionName   : Pressure_Handle
* Description    : 气压处理.
* Notice         : NO.
* Param[in]      : Target---传入一个气压目标值
* Output         : 稳定到目标值
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

    if(PressureParams.TimerStart)//进气开始在定时器开关
    {
        PressureParams.TimeMin++;
        if (((PressureParams.TimeMax-PressureParams.TimeMin)==10) && (PressureParams.InAirStatus==1))
        {
            PressureParams.ReadPressureValueFlag = 1;
        }
        if(PressureParams.TimeMin >= PressureParams.TimeMax)
        {
            PressureParams.TimeMin      = 0;//定时器最小值
            PressureParams.InAirStatus  = 0;//进气的状态为0表示结束，为1表示进气中
            PressureParams.OutAirStatus = 0;//排气的状态为0表示结束，为1表示排气中
            PressureParams.TimerStart     = 0;//时间到，自动关闭定时器0
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
* Description    : 继电器的开关函数
* Notice         : NO.
* Param[in]      : NewState---为枚举类型，ENABLE开启继电器，DISABLE关闭继电器
                     关继电器，主缸气压增大；开继电器，主缸泄气压
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
* Description    : 初始化限压
* Notice         : NO.
* Param[in]      : NO.
* Output         : NO.
* Return         : NO.
* Author         : sunny
*******************************************************************************/
void LimitPressure_Init(void) 
{
    MasterAirCylinder_Switch(DISABLE); //关闭限压开关
    SystemParams.uiMasterCompareCylinder = MAXDRUCK;  //主缸气压值
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
void LimitPressure_Handle(void)          //y=14x+620   x:气压值 y:采样电压值
{   
    u16  pressure =0;
	static u8 master_detetct_time_count = 0;
    static u8 venting_count = 0;
	
    if(SystemParams.ucLimitPressureSwitch == 1) //打开限压开关   
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



