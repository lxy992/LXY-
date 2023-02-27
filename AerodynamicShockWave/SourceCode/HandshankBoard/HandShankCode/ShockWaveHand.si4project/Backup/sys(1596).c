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

Flag_StructType Flag;  //ȫ�ֱ�־����
GlobalVariable_StructType GlobalVar;
u32 g_Start_ShockValue ;
u32 g_Stop_ShockValue ;

uint8_t HexToAsciiTab[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/*******************************************************************************
* Function Name  : NumberToAscii
* Description    : ��һ������ת��Ϊ�ַ���������Ϊ�����Ҳ�ᰴҪ���λ������ת����
                   ���磺������0�����Ҫ��ת��Ϊ2λ����ת��Ϊ"00"��
                         ������1�����Ҫ��ת��Ϊ6λ����ת��Ϊ"000001"��
                         ������2014�����Ҫ��ת��Ϊ6λ����ת��Ϊ"002014"��
                         ������123456789�����Ҫ��ת��Ϊ6λ����ת��Ϊ"456789"
                    ���lenΪ0xFF��������ת�� �Զ���λ��ת�� 0ת��Ϊ0
                    ���磺198 ת��Ϊ"198"
                   ע��32λ���ε����Ϊ4294967295��10λ��
* Input          : buf:�ַ����洢��
                   number:Ҫת��������
                   len:���ֵ�λ��
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
* Description    : Write_EEPROM,��λ��ص�ַ����λ��ߵ�ַ,ע:�����³��򣬻����EEPROM
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
        FLASH_ProgramByte(0x4000  + i, *(p_data + i));//��0X4000��ʼѰַ
    }

    FLASH_Lock(FLASH_MEMTYPE_DATA);
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */


