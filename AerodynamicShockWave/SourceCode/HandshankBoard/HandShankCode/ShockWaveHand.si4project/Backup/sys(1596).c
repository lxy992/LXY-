/**
  ******************************************************************************
  * @file    sys.c
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief
  ******************************************************************************
  ******************************************************************************
***/

#include "config.h"

Flag_StructType Flag;  //全局标志变量
GlobalVariable_StructType GlobalVar;
u32 g_Start_ShockValue ;
u32 g_Stop_ShockValue ;

uint8_t HexToAsciiTab[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/*******************************************************************************
* Function Name  : NumberToAscii
* Description    : 将一个数字转化为字符串，对于为零的数也会按要求的位数进行转化。
                   例如：对数字0，如果要求转化为2位，则转化为"00"。
                         对数字1，如果要求转化为6位，则转化为"000001"。
                         对数字2014，如果要求转化为6位，则转化为"002014"。
                         对数字123456789，如果要求转化为6位，则转化为"456789"
                    如果len为0xFF则按照自由转化 自动按位数转化 0转化为0
                    例如：198 转化为"198"
                   注意32位整形的最大为4294967295，10位数
* Input          : buf:字符串存储区
                   number:要转化的数字
                   len:数字的位数
* Output         : None
* Return         : None
* Date           : 2014-09-05
* Author         : LL
*******************************************************************************/
void NumberToAscii(uint8_t *buf, uint32_t number, uint8_t len)
{
    uint8_t i;
    uint32_t temp;

    if (0xFF == len)
    {
        i = 0;
        temp = number;
        while(1)
        {
            i++;
            temp = temp / 10;
            if (0 == temp) break;
        }
        len = i;
    }
    if (len > 9) len = 10;

    for (i = 0; i < len; i++)
    {
        buf[len - 1 - i] = number % 10 + '0';
        number = number / 10;
    }
    buf[i] = '\0';
}

void Paremeter_Init(void)
{
    Flag.AppStart = FALSE;
    Flag.MainBoardPower = FALSE;
    Flag.power = FALSE;
    Flag.Start = FALSE;

    GlobalVar.EnergyValue = 0;
    GlobalVar.FrequencyValue = 0;
    GlobalVar.ShockCnt = 0;
    Flag.Start = FALSE;
    Flag.power = FALSE;
	Flag.HandConnect = 1;
	GlobalVar.EepromDataValue = 0;
    OLED_Clear();
}
/*******************************************************************************
* Function Name  : Write_EEPROM_ReData
* Description    : Write_EEPROM_ReData
* Input          : none
* Output         : None
* Return         : None
* Date           : 2017.7.14
* Author         : PXZ
*******************************************************************************/
void Record_StartAndStop_Value(void)
{
    static BOOL PreValue = FALSE;
    BOOL NewValue;

    NewValue = Flag.Start;

    if(NewValue != PreValue)
    {
        if(Flag.Start)
        {
            g_Start_ShockValue = GlobalVar.ShockCnt;
        }
        else
        {
            g_Stop_ShockValue = GlobalVar.ShockCnt;
            Write_EEPROM_NewData();
        }
        PreValue = NewValue;
    }
}
/*******************************************************************************
* Function Name  : Read_EEPROM_ShockCnt
* Description    : Read EEPROM old ShockCnt
* Input          : none
* Output         : None
* Return         : None
* Date           : 2017.7.14
* Author         : PXZ
*******************************************************************************/
u32 Read_EEPROM_ShockCnt (void)
{

    u32 EEPROM_OldValue_Temp;

    u8 i, buf[4];
    u32 Temp;
	
    for(i = 0; i < 4; i++)
    {
        buf[i] = FLASH_ReadByte(0x4000 + i);
    }
    Temp = buf[0];
    EEPROM_OldValue_Temp = Temp  << 24;
    Temp = buf[1] ;
    EEPROM_OldValue_Temp |=  (Temp << 16);
    Temp = buf[2] ;
    EEPROM_OldValue_Temp |=  (Temp << 8);;
    Temp = buf[3] ;
    EEPROM_OldValue_Temp |=  Temp;
    return EEPROM_OldValue_Temp;
}
/*******************************************************************************
* Function Name  :  Write_EEPROM_NewData
* Description    : Write new data to EEPROM
* Input          : none
* Output         : None
* Return         : None
* Date           : 2017.7.14
* Author         : PXZ
*******************************************************************************/
void Write_EEPROM_NewData(void)
{
    u32 ShockCnt_Temp;
    u32 EEPROM_NewValue_Temp;

    ShockCnt_Temp = g_Start_ShockValue - g_Stop_ShockValue;
	
    if( ShockCnt_Temp > 0)
    {
        EEPROM_NewValue_Temp = Read_EEPROM_ShockCnt() + ShockCnt_Temp;
        Write_EEPROM (0x4000, (u8*)&EEPROM_NewValue_Temp, 4);
        Protocol_PrintfData(COMMANDSTATE_SEND, 0x34, (u8*)&EEPROM_NewValue_Temp, 4);	
    }

}

void  Read_EEPROM_Data(void)
{
	 GlobalVar.EepromDataValue = Read_EEPROM_ShockCnt();
}

void  SendEepromShockCnt(void)
{
     Protocol_PrintfData(COMMANDSTATE_SEND, 0x34, (u8*)&GlobalVar.EepromDataValue, 4);	
}

/*******************************************************************************
* Function Name  : Write_EEPROM
* Description    : Write_EEPROM,高位存地地址，低位存高地址,注:升级新程序，会擦除EEPROM
* Input          : addr:address,p_data:data,len: data len
* Output         : None
* Return         : None
* Date           : 2017.7.13
* Author         : PXZ
*******************************************************************************/
void Write_EEPROM(u16 addr, u8* p_data, u16 len)
{
    u16 i;

    FLASH_Unlock(FLASH_MEMTYPE_DATA);

    for(i = 0; i < len; i++)
    {
        FLASH_ProgramByte(0x4000  + i, *(p_data + i));//从0X4000开始寻址
    }

    FLASH_Lock(FLASH_MEMTYPE_DATA);
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */


