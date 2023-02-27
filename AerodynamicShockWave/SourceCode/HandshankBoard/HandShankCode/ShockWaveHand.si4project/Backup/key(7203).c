
/**
******************************************************************************
* @file    key.c
* @author  LL
* @version V0.0.0
* @date    2014-04-23
* @brief
******************************************************************************
******************************************************************************
***/

#include "config.h"

#define KEY_NULL           0x00 //空
#define KEY_START          0x01
#define KEY_ENERGYUP       0x02
#define KEY_ENERGYDOWN     0x04
#define KEY_FREQUP         0x08
#define KEY_FREQDOWN       0x10

#define KEY_OLEDON         0XF1
#define KEY_HOLDBIT        0x80

#define KEY_GROUP_UDLR      (KEY_NULL|KEY_NULL)

#define KEY_GROUP_UDLRHOLD  (KEY_HOLDBIT|KEY_GROUP_UDLR)

#define KEY_NULLHOLD        (KEY_HOLDBIT|KEY_NULL)

enum
{
    CKSTEP_NULL,
    CKSTEP_PRESS,
    CKSTEP_HOLD,
    CKSTEP_HOLDRPT,
    CKSTEP_RLS
};
	
#define KEY_T_500MS 50
#define KEY_T_1S 170
#define KEY_T_2S 2*KEY_T_1S
#define KEY_T_3S 3*KEY_T_1S

//#define KEY_TEST //按键测试  1-5按键，energy+  energy-  start  frequency+  frequency-
#define KEY1_IN GPIO_Init(GPIOB,GPIO_PIN_5,GPIO_MODE_IN_PU_NO_IT)  //START
#define KEY1_PIN GPIO_ReadInputPin(GPIOB,GPIO_PIN_5)

#define KEY2_IN GPIO_Init(GPIOB,GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT)  //ENERGY +
#define KEY2_PIN GPIO_ReadInputPin(GPIOB,GPIO_PIN_2)

#define KEY3_IN GPIO_Init(GPIOB,GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT)  //ENERGY -
#define KEY3_PIN GPIO_ReadInputPin(GPIOB,GPIO_PIN_3)

#define KEY4_IN GPIO_Init(GPIOB,GPIO_PIN_4,GPIO_MODE_IN_PU_NO_IT)   //FREQUENCY +
#define KEY4_PIN GPIO_ReadInputPin(GPIOB,GPIO_PIN_4)

#define KEY5_IN GPIO_Init(GPIOB,GPIO_PIN_1,GPIO_MODE_IN_PU_NO_IT)   //FREQUENCY -
#define KEY5_PIN GPIO_ReadInputPin(GPIOB,GPIO_PIN_1)


/*
如果按键按下一次有多次动作，是因为按键抖动引起，可适当增加按键的扫描时间
和增加去抖动处理。
*/

static struct key_data
{
    unsigned  char closed;
    unsigned  char Kdata;
    unsigned  char step;
    unsigned  long count;
} keys;

void Key_Init(void)
{
    KEY1_IN;
    KEY2_IN;
    KEY3_IN;
    KEY4_IN;
    KEY5_IN;
}

//********************************************************************************
//按键处理
//********************************************************************************
void Key_All_Serve(void)
{
	u32 Read_EEPROM_ShockCnt_Temp;
	
    if(FALSE == Flag.MainBoardPower) return;

    switch(keys.closed)
    {
        case KEY_OLEDON:  //长按开机
            Flag.power = Flag.power ? FALSE : TRUE;
            if(Flag.power)
            {          
                Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x33, 0x01);  //开机响应
                Read_EEPROM_ShockCnt_Temp = Read_EEPROM_ShockCnt();
                Protocol_PrintfData(COMMANDSTATE_SEND,0x34,(u8*)& Read_EEPROM_ShockCnt_Temp,4);
            }
            else
            {
                Flag.power = FALSE;
                OLED_Clear();
                Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x33, 0x00);
				Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x02, 0x00);
            }
            break;
		

        case KEY_START:
          
            if ((0 == GlobalVar.EnergyValue) || (0 == GlobalVar.ShockCnt) || (0 == Flag.AppStart) || ( FALSE == Flag.power))
            {
                Flag.Start = FALSE;
                break;
            }
            Flag.Start = Flag.Start ? FALSE : TRUE;
            if(Flag.Start)
            {
                Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x02, 0x01);
            }
            else
            {
                Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x02, 0x00);
            }
            break;
			
        case KEY_FREQUP:
            if (GlobalVar.FrequencyValue < FREQUENCY_NUM)
			{
				GlobalVar.FrequencyValue++;
				if((GlobalVar.FrequencyValue > 10) && (GlobalVar.FrequencyValue < 17))
				{
					if(GlobalVar.EnergyValue > 7)
					{
						GlobalVar.EnergyValue = 7;
						Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x25, GlobalVar.EnergyValue);
					}
				}
				else if(GlobalVar.FrequencyValue > 16)
				{
					if(GlobalVar.EnergyValue > 4)
					{
						GlobalVar.EnergyValue = 4;
						Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x25, GlobalVar.EnergyValue);
					}
				}
			}
            else 
			{
				GlobalVar.FrequencyValue = FREQUENCY_NUM;
				if(GlobalVar.EnergyValue > 4)
				{
					GlobalVar.EnergyValue = 4;
					Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x25, GlobalVar.EnergyValue);
				}
			}
            Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x24, GlobalVar.FrequencyValue);
			
            break;

        case KEY_FREQDOWN:
            if (GlobalVar.FrequencyValue > 1) GlobalVar.FrequencyValue--;
            else GlobalVar.FrequencyValue = 1;
            Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x24, GlobalVar.FrequencyValue);
            break;
        case KEY_ENERGYUP:
            if (GlobalVar.EnergyValue < ENERGY_NUM) 
			{
				GlobalVar.EnergyValue++;
				if((GlobalVar.EnergyValue > 4) && (GlobalVar.EnergyValue < 8))
				{
					if(GlobalVar.FrequencyValue > 16)
					{
						GlobalVar.FrequencyValue = 16;
						Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x24, GlobalVar.FrequencyValue);
					}
				}
				else if(GlobalVar.EnergyValue > 7)
				{
					if(GlobalVar.FrequencyValue > 10)
					{
						GlobalVar.FrequencyValue = 10;
						Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x24, GlobalVar.FrequencyValue);
					}
				}
			}
            else 
			{
				GlobalVar.EnergyValue = ENERGY_NUM;
				if(GlobalVar.FrequencyValue > 10)
				{
					GlobalVar.FrequencyValue = 10;
					Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x24, GlobalVar.FrequencyValue);
				}
			}
            Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x25, GlobalVar.EnergyValue);
            break;

        case KEY_ENERGYDOWN:
           if (GlobalVar.EnergyValue > 1) GlobalVar.EnergyValue--;
            else GlobalVar.EnergyValue = 1;
            Protocol_PrintfCommand(COMMANDSTATE_SEND, 0x25, GlobalVar.EnergyValue);
            break;

        default:
            break;
    }

}


//********************************************************************************
//key scan
//********************************************************************************
void Key_Read(void)
{
    unsigned char temp;
    BOOL ref = FALSE;

    if(FALSE == Flag.key_scan) return;

    if (0 == KEY1_PIN) temp = 0x01;       //多个按键检测
    else temp = 0x00;
    if (0 == KEY2_PIN) temp |= 0x02;
    if (0 == KEY3_PIN) temp |= 0x04;
    if (0 == KEY4_PIN) temp |= 0x08;
    if (0 == KEY5_PIN) temp |= 0x10;

    switch(keys.step)
    {
        case CKSTEP_NULL:
            if (KEY_NULL != temp)
            {
                keys.Kdata = temp;
                keys.step = CKSTEP_PRESS;
            }
            break;
        case CKSTEP_PRESS:
            if (temp != keys.Kdata)
            {
                keys.step = CKSTEP_NULL;
                break;
            }
            keys.step = CKSTEP_HOLD;
            keys.count = 0;
            break;
        case CKSTEP_HOLD:
            if (temp != keys.Kdata)
            {
                keys.step = CKSTEP_RLS;
            }
            keys.count++;
            if (keys.count > KEY_T_1S)
            {
                keys.closed = 0xf0 + keys.Kdata;
                ref = TRUE;
                keys.step = CKSTEP_RLS;
            }
            break;
        case CKSTEP_RLS:
            if (temp == KEY_NULL)
            {
                if (keys.count < KEY_T_500MS)
                {
                    keys.closed = keys.Kdata;
                    ref = TRUE;
                }
                keys.step = CKSTEP_NULL;
            }
            if (temp != keys.Kdata)
            {
                keys.step = CKSTEP_NULL;
            }
            break;
        default:
            keys.step = CKSTEP_NULL;
            break;
    }

    if (ref)
    {
        Key_All_Serve();
        keys.closed = 0;
    }

    Flag.key_scan = FALSE;
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */
