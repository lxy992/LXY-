/**
  ******************************************************************************
  * @file    protocol.h
  * @author  LL
  * @version V0.0.0
  * @date    2017-02-27
  * @brief   通信协议解析和处理
  ******************************************************************************
  ******************************************************************************
***/

#include "config.h"

#define DEVICE_NUM 0x0007  //设备ID

#define  VERSION_NUM  0x0A    //设备版本号

//命令类型
#define COMMANDTYPE_ORDER 0x43
#define COMMANDTYPE_DATA  0x44
#define COMMANDTYPE_READ  0x52

static Protocol_StructType Protocol_DataPort1;//端口1的协议数据
Protocol_StructType *pProtocol_DataPort1 = &Protocol_DataPort1;

/*******************************************************************************
* Function Name  : Protocol_PackageDeviceNum
* Description    : 打包设备ID
* Input          : buf:信息
                   len:信息长度
* Output         : None
* Return         : None
*******************************************************************************/
static void Protocol_PackageDeviceNum(uint8_t *buf, uint16_t len)
{
    uint8_t buft[15];
    uint8_t i;

    buft[1] = DEVICE_NUM & 0xFF;
    buft[0] = (DEVICE_NUM >> 8) & 0xFF; //高位在前

    for(i = 0; i < len; i++)
    {
        buft[i + 2] = buf[i];
    }
    UART_PrintfNormal(buft, len + 2);
}

/*******************************************************************************
* Function Name  : Protocol_DealRead
* Description    : 处理读功能
* Input          : data:接收到的数据
                   len:数据量
* Output         : None
* Return         : None
* Date           : 2017-02-28
* Author         : LL
*******************************************************************************/
static void Protocol_DealRead(uint8_t *data, uint16_t len)
{
    if (0 == len) return;

    if (0x02 != data[0]) return;

    switch (data[1])
    {
	    case 0x18:
			data[0] = COMMANDTYPE_READ;
			data[1] = COMMANDSTATE_RECIVEOK;
			data[2] = 0x18;
			data[3] = VERSION_NUM;
			Protocol_PackageDeviceNum(data,4);
	        break;
        default:
            DEBUG_PRINTFMASSAGES("DealRead err");
            DEBUG_PRINTFNUMBERHEX(data[1]);
            break;
    }
}
/*******************************************************************************
* Function Name  : Treatment_Response
* Description    : deal the response from Main PC
* Input          : no
* Author         : PXZ
* Time           : 2017.6.29
*******************************************************************************/
void Treat_Response(u8 *data)
{
    Flag.OLED_Flash = TRUE;
    switch (data[1])
    {
        case 0x02:

            if (0x01 == data[2]) //收到响应是01，即显示start
            {


//                  Flag.AppStart=0;
                Flag.Start = TRUE;

            }
            else
            {
                Flag.Start = FALSE;
            }
            break;

        case 0x24:

            GlobalVar.FrequencyValue = data[2];
            break;

        case 0x25:

            GlobalVar.EnergyValue = data[2];
            break;
        case 0x33:
            if( 0x01 == data [2])
            {
                Flag.OLED_Flash = TRUE;
                Flag.power = TRUE;
                Show_MenuInit();
            }
            break;
        default :
            break;
    }
}
/*******************************************************************************
* Function Name  : Treatment_DataAndStatus
* Description    : deal the response from Main PC
* Input          : no
*******************************************************************************/
void Treat_DataAndStatus(u8 *data)
{
    u32 temp;
	u32 Read_EEPROM_ShockCnt_Temp = 0;	
	
    switch (data[1])
    {
        case 0x02:         //运行与暂停

            if(data[2] == 0x01) //收到响应是01，即显示start
            {
                Flag.AppStart = TRUE;
                Protocol_PrintfCommand(COMMANDSTATE_RECIVEOK, 0x02, 0x01);
            }
            else
            {
                Flag.OLED_Flash = TRUE;
                Flag.AppStart = FALSE;
                Flag.Start = FALSE;
                Protocol_PrintfCommand(COMMANDSTATE_RECIVEOK, 0x02, 0x00);
            }
			
            break;
        case 0x06:
			if (0x01 == data[2])  
			{
				Write_EEPROM (0x4000,(u8*)&Read_EEPROM_ShockCnt_Temp, 4);
                Protocol_PrintfData(COMMANDSTATE_SEND, 0x34,(u8*)&Read_EEPROM_ShockCnt_Temp, 4);	
			}
		    break;
        case 0x08: //主机收到温度返回响应，不做处理

            break;
        case 0x24:
            Flag.OLED_Flash = TRUE;
            GlobalVar.FrequencyValue = data[2];
            break;
        case 0x25:
            Flag.OLED_Flash = TRUE;
            GlobalVar.EnergyValue = data[2];
            break;
        case 0x26:
            Flag.OLED_Flash = TRUE;
            GlobalVar.ShockCnt = data[2];
            GlobalVar.ShockCnt = (GlobalVar.ShockCnt << 16);
            temp = data[3];
            temp <<= 8;
            GlobalVar.ShockCnt |= temp;
            temp = data[4];
            GlobalVar.ShockCnt |= temp;
            break;
        default:
            DEBUG_PRINTFMASSAGES("DealData err");
            DEBUG_PRINTFNUMBERHEX(data[1]);
            break;
    }
}

/*******************************************************************************
* Function Name  : Protocol_DealData
* Description    : 处理接收到的数据
* Input          : data:接收到的数据
                   len:数据量
* Output         : None
* Return         : None
* Date           : 2017-02-28
* Author         : LL
*******************************************************************************/
static void Protocol_DealData(uint8_t *data, uint16_t len)
{

    if (0 == len) return;

//      if (0x02 != data[0]) return;
    switch(data[0])
    {
        case 0x01:    //处理收到的响应3201
            Treat_Response(data);
            break;

        case 0x02:    //处理收到的数据状态3202
            Treat_DataAndStatus(data);
            break;
    }

}

/*******************************************************************************
* Function Name  : Protocol_DealOrder
* Description    : 处理接收到的指令
* Input          : data:接收到的数据
                   len:数据量
* Output         : None
* Return         : None
* Date           : 2017-02-28
* Author         : LL
*******************************************************************************/

static void Protocol_DealOrder(uint8_t *data, uint16_t len)
{


    if (0 == len) return;

    if (COMMANDSTATE_SEND != data[0]) return;

    switch (data[1])
    {
        case 0x01:
            if (0x01 == data[2]) //收到响应是01，接收到主机上开机了
            {
                Flag.MainBoardPower = TRUE;
            }
            else
            {
                Flag.MainBoardPower = FALSE;
                GlobalVar.EnergyValue = 0;
                GlobalVar.FrequencyValue = 0;
                GlobalVar.ShockCnt = 0;
                Flag.Start = FALSE;
                Flag.power = FALSE;
                OLED_Clear();
            }
            break;
        case 0x33:
            if (0x00 == data[2]) //
            {

                Flag.Start = FALSE;
                Flag.power = FALSE;
                OLED_Clear();
            }
        case 0x08:           //send temperature
//	        	if( !Flag.MainBoardPower )break;
//	            Temperature_Read();
            break;
        case 0x0C:

            break;

        default:
            DEBUG_PRINTFMASSAGES("DealOrder err");
            DEBUG_PRINTFNUMBERHEX(data[0]);
            break;
    }
}

/*******************************************************************************
* Function Name  : Protocol_CommandType
* Description    : 处理命令类型
* Input          : data:接收到的数据
                   len:数据量
* Output         : None
* Return         : None
* Date           : 2017-02-28
* Author         : LL
*******************************************************************************/
static void Protocol_CommandType(uint8_t *data, uint16_t len)
{

    if (0 == len) return;

    switch (data[0])
    {
        case COMMANDTYPE_ORDER:     //43
            Protocol_DealOrder(data + 1, len - 1);
            break;
        case COMMANDTYPE_DATA:      //44
            Protocol_DealData(data + 1, len - 1);
            break;
        case COMMANDTYPE_READ:      //52
            Protocol_DealRead(data + 1, len - 1);
            break;
        default:
            DEBUG_PRINTFMASSAGES("CommandType err");
            DEBUG_PRINTFNUMBERHEX(data[0]);
            break;
    }
}

/*******************************************************************************
* Function Name  : Protocol_DeviceNum
* Description    : 处理设备号
* Input          : data:接收到的数据
                   len:数据量
* Output         : None
* Return         : None
* Date           : 2017-02-27
* Author         : LL
*******************************************************************************/
static void Protocol_DeviceNum(uint8_t *data, uint16_t len)
{
    uint16_t devicenum;

    if (len < 2) return;

    devicenum = data[1] | (data[0] << 8);

    switch (devicenum)
    {
        case DEVICE_NUM:
            Protocol_CommandType(data + 2, len - 2);
            break;
        default:
            DEBUG_PRINTFMASSAGES("Device err");
            DEBUG_PRINTFNUMBERHEX(devicenum);
            break;
    }
}

/*******************************************************************************
* Function Name  : Protocol_PrintfData
* Description    : 将数据打包，并写入发送缓冲区
* Input          : sta:状态
                   cmd:命令信息，功能码
                   para:参数
                   len:参数长度
* Output         : None
* Return         : None
* Date           : 2014-08-26
* Author         : LL
*******************************************************************************/
void Protocol_PrintfData(uint8_t sta, uint8_t cmd, uint8_t *para, uint16_t len)
{
    uint8_t i;
    uint8_t buf[10];

    buf[0] = COMMANDTYPE_DATA;
    buf[1] = sta;
    for (i = 0; i < len; i++)
    {
        buf[i + 3] = para[i];
    }
    switch (cmd)
    {	
        case 0x08:
            buf[2] = cmd;
            Protocol_PackageDeviceNum(buf, len + 3);
            break;
        case 0x34:
            buf[2] = cmd;
            buf[3] = *((u32*)para) >> 24;
            buf[4] = *((u32*)para) >> 16;
            buf[5] = *((u32*)para) >> 8;
            buf[6] = *((u32*)para);
            Protocol_PackageDeviceNum(buf, len + 3);
            break;
		case 0x22:
			buf[2] = cmd;
			buf[3] = *para;
            Protocol_PackageDeviceNum(buf, len + 3);
			break;

        default:
            break;
    }
}

/*******************************************************************************
* Function Name  : Protocol_PrintfCommand
* Description    : 将命令打包，并写入发送缓冲区
* Input          : sta:状态
                   cmd:命令信息，功能码
                   para:参数
* Output         : None
* Return         : None
* Date           : 2017-03-01
* Author         : LL
*******************************************************************************/
void Protocol_PrintfCommand(uint8_t sta, uint8_t cmd, uint32_t para)
{
    uint8_t buf[5];
    uint8_t len;

    buf[0] = COMMANDTYPE_ORDER;
    buf[1] = sta;
    //len = (COMMANDSTATE_SEND==sta) ? 4 : 3;
    len = 4;

    switch (cmd)
    {
        case 0x01:
            buf[2] = cmd;
            buf[3] = para;
            Protocol_PackageDeviceNum(buf, len);
            break;
        case 0x0C:
            buf[2] = cmd;
            len = 3;
            Protocol_PackageDeviceNum(buf, len);
            break;
        case 0x02:
        case 0x25:                  //energy
            buf[2] = cmd;
            buf[3] = para;
            Protocol_PackageDeviceNum(buf, len);
            break;
        case 0x24:                  //frequency value Not equal to 0
            buf[2] = cmd;
            buf[3] = para;
            Protocol_PackageDeviceNum(buf, len );
            break;
        case 0x26:
            buf[2] = cmd;
            buf[3] = para >> 16;
            buf[4] = para >> 8;
            break;
        case 0x33:
            buf[2] = cmd;
            buf[3] = para;
            Protocol_PackageDeviceNum(buf, len);
            break;
        default:
            break;
    }
}


/*******************************************************************************
* Function Name  : Protocol_DataDecode
* Description    : 接收数据解码，对数据拆包处理,自定义协议格式
* Input          : data:要解码的数据
* Output         : None
* Return         : None
* Date           : 2017-02-27
* Author         : LL
*******************************************************************************/
void Protocol_Init(void)
{
    pProtocol_DataPort1 = &Protocol_DataPort1;
    pProtocol_DataPort1->step = 0;
    pProtocol_DataPort1->cnt = 0;
    pProtocol_DataPort1->len = 0;
}


/*******************************************************************************
* Function Name  : Protocol_DataDecode
* Description    : 接收数据解码，对数据拆包处理,自定义协议格式
* Input          : data:要解码的数据
* Output         : None
* Return         : None
* Date           : 2017-02-27
* Author         : LL
*******************************************************************************/
void Protocol_DataDecode(uint8_t data, Protocol_StructType *p)
{
	
    uint16_t crc16_check;

    switch (p->step)
    {
        case 0:
            if (0x53 == data)
            {
                p->step = 1;
            }
            else
            {
                p->step = 0;
            }
            break;
        case 1:
            if (0x47 == data)
            {
                p->step = 2;
            }
            else
            {
                p->step = 0;
            }
            break;
        case 2:
            p->len = data;
            p->cnt = 0;
            p->step = 3;
            if (p->len > 128) p->len = 128;
            if(0 == (p->len))
            {
                p->step = 0;
            }
            break;
        case 3:
            if (p->cnt < p->len)
            {
                p->DataCodeBuff[p->cnt++] = data;
            }
            else
            {
                p->step = 4;
                p->crc16_h = data;
            }
            break;
        case 4:
            p->crc16_l = data;
            crc16_check = CRC_16ALL_Len1Byte(p->DataCodeBuff, p->len);
            if (((p->crc16_h << 8) | p->crc16_l) == crc16_check)        //如果CRC校验正确，就解码数据
            {
                Protocol_DeviceNum(p->DataCodeBuff, p->len);
				
            }
            else
            {
                DEBUG_PRINTFMASSAGES("");
                DEBUG_PRINTFMASSAGES("**LRC check error**");
                DEBUG_PRINTFMASSAGES("Recive CRC:");
                DEBUG_PRINTFNUMBERHEX(((p->crc16_h << 8) | p->crc16_l) & 0xFFFF);
                DEBUG_PRINTFMASSAGES("Check CRC:");
                DEBUG_PRINTFNUMBERHEX(crc16_check);
                DEBUG_PRINTFMASSAGES("Data:");
                DEBUG_PRINTFMASSAGESHEX(p->DataCodeBuff, p->len);
            }
            p->step = 0;
            break;
        default:
            p->step = 0;
//              DEBUG_PRINTFMASSAGES("error");
            break;
    }

}


/* ************ ****** ************ THE FILE END  ************ ****** ************ */
