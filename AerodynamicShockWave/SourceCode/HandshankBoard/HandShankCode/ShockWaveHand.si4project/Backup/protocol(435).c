/**
  ******************************************************************************
  * @file    protocol.h
  * @author  LL
  * @version V0.0.0
  * @date    2017-02-27
  * @brief   ͨ��Э������ʹ���
  ******************************************************************************
  ******************************************************************************
***/

#include "config.h"

#define DEVICE_NUM 0x0007  //�豸ID

#define  VERSION_NUM  0x0A    //�豸�汾��

//��������
#define COMMANDTYPE_ORDER 0x82
#define COMMANDTYPE_DATA  0x44
#define COMMANDTYPE_READ  0x52

static Protocol_StructType Protocol_DataPort1;//�˿�1��Э������
Protocol_StructType *pProtocol_DataPort1 = &Protocol_DataPort1;


/***********************************
�������ܣ��豸д����
���������������ַ,д������
�����������
��ע���豸��ȡ����
************************************/
void DeviceWriteData(u16 temp_addr,u8 *data)
{
    u8  send_data[10];

    send_data[0] = 0x5A;
    send_data[1] = 0xA5;
    send_data[2] = 0x05;
	send_data[3] = 0x82;
    send_data[4] = (temp_addr>>8);
	send_data[5] = (u8)(temp_addr&0x00ff);
    send_data[6] = data[0];
	send_data[7] = data[1];

	FIFO_PutOne(pTx, send_data[0]);
    FIFO_PutOne(pTx, send_data[1]);
    FIFO_PutOne(pTx, send_data[2]);
    FIFO_PutOne(pTx, send_data[3]);
    FIFO_PutOne(pTx, send_data[4]);
    FIFO_PutOne(pTx, send_data[5]);
    FIFO_PutOne(pTx, send_data[6]);
    FIFO_PutOne(pTx, send_data[7]);
}

/*******************************************************************************
* Function Name  : Protocol_DealOrder
* Description    : ������յ���ָ��
* Input          : data:���յ�������
                   len:������
* Output         : None
* Return         : None
*******************************************************************************/
static void Protocol_DealOrder(uint8_t *data, uint16_t len)
{

    if (0 == len) return;

    switch (data[0])
    {
        case 0x01:
            if (0x01 == data[3]) //�յ���Ӧ��01�����յ������Ͽ�����
            {
                Flag.MainBoardPower = TRUE;
            }
            else
            {
                Flag.MainBoardPower = FALSE;
                GlobalVar.PressureValue = 10;
                GlobalVar.FrequencyValue = 0;
                GlobalVar.ShockCnt = 0;
                Flag.Start = FALSE;
                Flag.power = FALSE;
                OLED_Clear();
            }
            break;
		 case 0x02:         //��������ͣ
            if(data[3] == 0x01) //�յ���Ӧ��01������ʾstart
            {
                Flag.AppStart = TRUE;
            }
            else
            {
                Flag.OLED_Flash = TRUE;
                Flag.AppStart = FALSE;
                Flag.Start = FALSE;
            }
            break;	
        case 0x33:
            if (0x00 == data[3]) 
            {
                Flag.Start = FALSE;
                Flag.power = FALSE;
                OLED_Clear();
            }
			else
			{
                Flag.OLED_Flash = TRUE;
                Flag.power = TRUE;
                Show_MenuInit();
			}
        case 0x04:
            Flag.OLED_Flash = TRUE;
            GlobalVar.FrequencyValue = data[3];
            break;
        case 0x05:
            Flag.OLED_Flash = TRUE;
            GlobalVar.PressureValue = data[3];
            break;
        case 0x06:
            Flag.OLED_Flash = TRUE;
            GlobalVar.ShockCnt = data[2];
            GlobalVar.ShockCnt = (GlobalVar.ShockCnt << 8);
            GlobalVar.ShockCnt |= data[3];
            break;
        default:
            DEBUG_PRINTFMASSAGES("DealOrder err");
            DEBUG_PRINTFNUMBERHEX(data[0]);
            break;
    }
}

/*******************************************************************************
* Function Name  : Protocol_CommandType
* Description    : ������������
* Input          : data:���յ�������
                   len:������
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
       // case COMMANDTYPE_DATA:      //44
         //   Protocol_DealData(data + 1, len - 1);
         //   break;
        default:
            DEBUG_PRINTFMASSAGES("CommandType err");
            DEBUG_PRINTFNUMBERHEX(data[0]);
            break;
    }
}

/*******************************************************************************
* Function Name  : Protocol_DeviceNum
* Description    : �����豸��
* Input          : data:���յ�������
                   len:������
* Output         : None
* Return         : None
* Date           : 2017-02-27
* Author         : LL
*******************************************************************************/
static void Protocol_DeviceNum(uint8_t *data, uint16_t len)
{
    uint16_t devicenum;

    if (len < 2) return;

    Protocol_CommandType(data, len);
  

}

/*******************************************************************************
* Function Name  : Protocol_DataDecode
* Description    : �������ݽ��룬�����ݲ������,�Զ���Э���ʽ
* Input          : data:Ҫ���������
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
* Description    : �������ݽ��룬�����ݲ������,�Զ���Э���ʽ
* Input          : data:Ҫ���������
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
            if (0x5A == data)
            {
                p->step = 1;
            }
            else
            {
                p->step = 0;
            }
            break;
        case 1:
            if (0xA5 == data)
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
			p->DataCodeBuff[p->cnt++] = data;
			if(p->cnt>=p->len)
			{
                Protocol_DeviceNum(p->DataCodeBuff, p->len);
				p->cnt = 0;
				p->step = 0;
			}
            break;
        default:
            p->step = 0;
//              DEBUG_PRINTFMASSAGES("error");
            break;
    }

}


/* ************ ****** ************ THE FILE END  ************ ****** ************ */

