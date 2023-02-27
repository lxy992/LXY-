/**
  ******************************************************************************
  * @file    oled_parallel.c
  * @author  LL
  * @version V0.0.0
  * @date    2016-09-02
  * @brief   �ӿڷ�ʽ��8080
  ******************************************************************************
  ******************************************************************************
***/

#include "config.h"


#define START_X   2
#define START_Y   0

#define ENERGY_POSX START_X+1
#define ENERGY_POSY START_Y

#define SHOCKCNT_POSX START_X
#define SHOCKCNT_POSY (ENERGY_POSY+24+16)

#define FREQUENCY_POSX START_X+1
#define FREQUENCY_POSY (SHOCKCNT_POSY+24)

#define RUNSTATUS_POSX  START_X
#define RUNSTATUS_POSY (FREQUENCY_POSY+24+16+4)


#define OLED_CMD  0 //д����
#define OLED_DATA 1 //д����
#define DELAY_NOP // {nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();}

//#define DATAOUT(data) GPIOB->ODR = data

/****************��λ*********************/
#define OLED_RST_OUT GPIO_Init(GPIOD,GPIO_PIN_2,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_RST_Set() GPIO_WriteHigh(GPIOD,GPIO_PIN_2)
#define OLED_RST_Clr() GPIO_WriteLow(GPIOD,GPIO_PIN_2)
/****************����/����*********************/
#define OLED_DC_OUT GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_DC_Set() GPIO_WriteHigh(GPIOD,GPIO_PIN_3)
#define OLED_DC_Clr() GPIO_WriteLow(GPIOD,GPIO_PIN_3)
/****************Ƭѡ*********************/
#define OLED_CS_OUT GPIO_Init(GPIOD,GPIO_PIN_0,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_CS_Set()  GPIO_WriteHigh(GPIOD,GPIO_PIN_0)
#define OLED_CS_Clr() GPIO_WriteLow(GPIOD,GPIO_PIN_0)
/****************��д********************/
#define OLED_WR_OUT GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_WR_Set() GPIO_WriteHigh(GPIOD,GPIO_PIN_4)
#define OLED_WR_Clr() GPIO_WriteLow(GPIOD,GPIO_PIN_4)
/****************ʹ��********************/
#define OLED_ENABLE_OUT GPIO_Init(GPIOD,GPIO_PIN_7,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_ENABLE_Set() GPIO_WriteHigh(GPIOD,GPIO_PIN_7)
#define OLED_ENABLE_Clr() GPIO_WriteLow(GPIOD,GPIO_PIN_7)
/****************����0********************/
#define  OLED_D0_OUT GPIO_Init(GPIOE,GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_D0_Set() GPIO_WriteHigh(GPIOE,GPIO_PIN_5)
#define OLED_D0_Clr() GPIO_WriteLow(GPIOE,GPIO_PIN_5)

/****************����1********************/
#define OLED_D1_OUT  GPIO_Init(GPIOC,GPIO_PIN_1,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_D1_Set() GPIO_WriteHigh(GPIOC,GPIO_PIN_1)
#define OLED_D1_Clr() GPIO_WriteLow(GPIOC,GPIO_PIN_1)

/****************����2********************/
#define OLED_D2_OUT  GPIO_Init(GPIOC,GPIO_PIN_2,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_D2_Set() GPIO_WriteHigh(GPIOC,GPIO_PIN_2)
#define OLED_D2_Clr() GPIO_WriteLow(GPIOC,GPIO_PIN_2)

/****************����3********************/
#define OLED_D3_OUT  GPIO_Init(GPIOC,GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_D3_Set() GPIO_WriteHigh(GPIOC,GPIO_PIN_3)
#define OLED_D3_Clr() GPIO_WriteLow(GPIOC,GPIO_PIN_3)

/****************����4********************/
#define OLED_D4_OUT  GPIO_Init(GPIOC,GPIO_PIN_4,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_D4_Set() GPIO_WriteHigh(GPIOC,GPIO_PIN_4)
#define OLED_D4_Clr() GPIO_WriteLow(GPIOC,GPIO_PIN_4)

/****************����5********************/
#define OLED_D5_OUT  GPIO_Init(GPIOC,GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_D5_Set() GPIO_WriteHigh(GPIOC,GPIO_PIN_5)
#define OLED_D5_Clr() GPIO_WriteLow(GPIOC,GPIO_PIN_5)

/****************����6********************/
#define OLED_D6_OUT  GPIO_Init(GPIOC,GPIO_PIN_6,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_D6_Set() GPIO_WriteHigh(GPIOC,GPIO_PIN_6)
#define OLED_D6_Clr() GPIO_WriteLow(GPIOC,GPIO_PIN_6)

/****************����7********************/
#define OLED_D7_OUT  GPIO_Init(GPIOC,GPIO_PIN_7,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_D7_Set() GPIO_WriteHigh(GPIOC,GPIO_PIN_7)
#define OLED_D7_Clr() GPIO_WriteLow(GPIOC,GPIO_PIN_7)

/***************************LCD ����*****************************/
#define OLED_BK_OUT   GPIO_Init(GPIOF,GPIO_PIN_4,GPIO_MODE_OUT_PP_HIGH_SLOW)
#define OLED_BK_Set() GPIO_WriteHigh(GPIOF,GPIO_PIN_4)
#define OLED_BK_Clr() GPIO_WriteLow(GPIOF,GPIO_PIN_4)


#define DATAOUT(data) {\
  if(data&0x01)  OLED_D0_Set(); else OLED_D0_Clr();\
  if(data&0x02)  OLED_D1_Set(); else OLED_D1_Clr();\
  if(data&0x04)  OLED_D2_Set(); else OLED_D2_Clr();\
  if(data&0x08)  OLED_D3_Set(); else OLED_D3_Clr();\
  if(data&0x10)  OLED_D4_Set(); else OLED_D4_Clr();\
  if(data&0x20)  OLED_D5_Set(); else OLED_D5_Clr();\
  if(data&0x40)  OLED_D6_Set(); else OLED_D6_Clr();\
  if(data&0x80)  OLED_D7_Set(); else OLED_D7_Clr();\
}

//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat, u8 cmd)
{
    DATAOUT(dat);
    if(cmd)
    {
        OLED_DC_Set();
        DELAY_NOP;
    }
    else
    {
        OLED_DC_Clr();
        DELAY_NOP;
    }

    DELAY_NOP;
    OLED_CS_Clr();
    DELAY_NOP;
    OLED_WR_Clr();
    DELAY_NOP;
    OLED_WR_Set();
    DELAY_NOP;
    OLED_CS_Set();
    DELAY_NOP;
    OLED_DC_Set();
    DELAY_NOP;
}
#define CUN13
//#define CUN096
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD);
#ifdef CUN096
    OLED_WR_Byte((x & 0x0f) + 0x02, OLED_CMD);
#endif

#ifdef CUN13
    OLED_WR_Byte(x & 0x0f, OLED_CMD);
#endif
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(void)
{
    u8 i, n;
    for(i = 0; i < 8; i++)
    {
        OLED_WR_Byte (0xb0 + i, OLED_CMD); //����ҳ��ַ��0~7��
        OLED_WR_Byte (0x02, OLED_CMD);     //������ʾλ�á��е͵�ַ
        OLED_WR_Byte (0x10, OLED_CMD);     //������ʾλ�á��иߵ�ַ
        for(n = 0; n < 128; n++)
        {
            OLED_WR_Byte(0, OLED_DATA);
        }
    } //������ʾ
}

/*******************************************************************************
* Function Name  : OLED_Show8X16Num
* Description    : OLED������ʾ�ַ�������Խϳ��Ĳ������ŷ��ã�Ϊ����
* Input          : x:��ʼ��
                   y:��ʼ��
                   displaymode:��ʾģʽ��0������1����
                   chr:��ʾ���ַ�

* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-09-07
* Author         : LL
*******************************************************************************/
void OLED_Show8X16Num(u8 x, u8 y, u8 displaymode, u8 chr)
{
    unsigned char data;
    unsigned char c, i;

    c = chr - '0'; //�õ�ƫ�ƺ��ֵ
    if (c > 9) c = 10;

    OLED_Set_Pos(y, x);
    for(i = 0; i < 16; i++)
    {
        if (10 == c)
            data = 0x00;
        else
            data = F8X16NUM[c * 16 + i];
        if (1 == displaymode) data ^= 0xFF;
        OLED_WR_Byte(data, OLED_DATA);
    }
}
/*******************************************************************************
* Function Name  : OLED_ShowString8x16SH
* Description    : OLED������ʾ�ַ�������Խϳ��Ĳ������ŷ��ã�Ϊ����
* Input          : x:��ʼ�� ȡֵ��Χ0-7
                   y:��ʼ�� ȡֵ��Χ0-127
                   displaymode:��ʾģʽ��0������1����
                   chr:��ʾ���ַ���

* Output         : None
* Return         : None
* Date           : 2016-09-07
* Author         : LL
*******************************************************************************/

void OLED_ShowString8x16Num(u8 x, u8 y, u8 displaymode, u8 *chr)
{
    x = 7 - x;
    while (*chr != '\0')
    {
        OLED_Show8X16Num(x, y, displaymode, *chr);
        x--;
        chr++;
    }
}

/*******************************************************************************
* Function Name  : OLED_ShowCharSHO
* Description    : OLED������ʾ�ַ�������Խϳ��Ĳ������ŷ��ã�Ϊ����
* Input          : x:��ʼ��
                   y:��ʼ��
                   displaymode:��ʾģʽ��0������1����
                   num:��ʾ���ַ�λ��
                   char_type:1,��ʾ��F8X16HO��0����ʾF8X16NUM
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-09-07
* Author         : LL
*******************************************************************************/

void OLED_ShowCharSHO(u8 x, u8 y, u8 displaymode, u8 num)
{
    unsigned char data;
    unsigned char c, i;

    c = num; //�õ�ƫ�ƺ��ֵ
    OLED_Set_Pos(y, x);
    for(i = 0; i < 16; i++)
    {
        data = F8X16HO[c * 16 + i];
        if (1 == displaymode) data ^= 0xFF;
        OLED_WR_Byte(data, OLED_DATA);
    }

}
/*******************************************************************************
* Function Name  : OLED_ShowStringSHO
* Description    : OLED������ʾ�ַ�������Խϳ��Ĳ������ŷ��ã�Ϊ����
* Input          : x:��ʼ�� ȡֵ��Χ0-7
                   y:��ʼ�� ȡֵ��Χ0-127
                   displaymode:��ʾģʽ��0������1����
                   num:��ʾ���ַ������
* Output         : None
* Return         : None
* Date           : 2016-09-07
* Author         : LL
*******************************************************************************/
void OLED_ShowStringSHO(u8 x, u8 y, u8 displaymode, u8 num)
{
    u8 i, n;

    switch(num)
    {
        case 1:
        case 6:
            n = 4;
            break;
        case 11:
            n = 1;
            break;
		case 13:
			n = 2;
			break;
        case 15:
            n = 8;
            break;
        default:
            n = 0;
            break;
    }
    x = 7 - x;

    for(i = 0; i <= n; i++)
    {
        OLED_ShowCharSHO(x, y, displaymode, num + i);

        if(0 == x) return;
        x--;

    }
}





/*******************************************************************************
* Function Name  : OLED_ShowChar16x24SH
* Description    : OLED������ʾ�ַ�������Խϳ��Ĳ������ŷ��ã�Ϊ����
* Input          : x:��ʼ��
                   y:��ʼ��
                   displaymode:��ʾģʽ��0������1����
                   chr:��ʾ���ַ�
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-09-07
* Author         : LL
*******************************************************************************/
void OLED_ShowChar16x24SH(u8 x, u8 y, u8 displaymode, u8 chr)
{
    unsigned char data;
    unsigned char c, i;

    c = chr - '0'; //�õ�ƫ�ƺ��ֵ
    if (c > 9) c = 10;

    OLED_Set_Pos(y, x);
    for(i = 0; i < 24; i++)
    {
        if (10 == c)
            data = 0x00;
        else
            data = F16X24H[c * 48 + i];
        if (1 == displaymode) data ^= 0xFF;
        OLED_WR_Byte(data, OLED_DATA);
    }
    OLED_Set_Pos(y, x - 1);
    for(i = 0; i < 24; i++)
    {
        if (10 == c)
            data = 0x00;
        else
            data = F16X24H[c * 48 + i + 24];
        if (1 == displaymode) data ^= 0xFF;
        OLED_WR_Byte(data, OLED_DATA);
    }
}

/*******************************************************************************
* Function Name  : OLED_ShowString16x24SH
* Description    : OLED������ʾ�ַ�������Խϳ��Ĳ������ŷ��ã�Ϊ����
* Input          : x:��ʼ�� ȡֵ��Χ0-7
                   y:��ʼ�� ȡֵ��Χ0-127
                   displaymode:��ʾģʽ��0������1����
                   chr:��ʾ���ַ���
* Output         : None
* Return         : None
* Date           : 2016-09-07
* Author         : LL
*******************************************************************************/
void OLED_ShowString16x24SH(u8 x, u8 y, u8 displaymode, u8 *chr)
{
    x = 7 - x;
    while (*chr != '\0')
    {
        OLED_ShowChar16x24SH(x, y, displaymode, *chr);
        x--;
        if(x == 0) return;
        x--;
        if(x == 0) return;
        chr++;
    }
}
/*******************************************************************************
* Function Name  : OLED_ShowFillSH
* Description    : OLED�����������ʾ
* Input          :
x:��ʼ�У�ȡֵ��Χ0-7
y:��ʼ�У�ȡֵ��Χ0-127
width:��ȣ�ȡֵ��Χ1-8
height:�߶� ȡֵ��Χ1-128
* Output         : None
* Return         : None
* Date           : 2016-09-07
* Author         : LL
*******************************************************************************/
void OLED_ShowFillSH(u8 x, u8 y, u8 width, u8 height)
{
    u8 i, n;

    for(i = width; i > 0; i--)
    {
        OLED_Set_Pos(y, i + 7 - width - x);
        for(n = 0; n < height; n++)
        {
            OLED_WR_Byte(0xFF, OLED_DATA);
        }
    }
}

/*******************************************************************************
* Function Name  : Show_Menu
* Description    : ��ʾ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t t;

static void Show_Menu(void)
{
	 
	
	if(FALSE == Flag.power)return;
	
    unsigned char x, y;
    unsigned char buf[5];
    unsigned char 

    //��ʾ��������
    x = ENERGY_POSX;
    y = ENERGY_POSY+8;
    t = GlobalVar.PressureValue;
	if (t > 9) x = 2;
    NumberToAscii(buf, t, 0xFF);
	buf[2] = buf[1];
	buf[1] = '.';
    OLED_ShowString16x24SH(0, y, 0, "       ");
    //OLED_ShowString16x24SH(x, y, 0, buf);
    OLED_ShowChar16x24SH(x, y, 0, *chr);
    OLED_ShowStringSHO(ENERGY_POSX, ENERGY_POSY+8+24, 1, 13);	

    x = 3;
    y = SHOCKCNT_POSY;
    if(GlobalVar.ShockCnt > 999999 )
    {
        OLED_ShowString16x24SH(0, y,  1, "       ");
        OLED_ShowStringSHO(0, 44, 1, 15);
    }
    else
    {
        if(GlobalVar.ShockCnt > 9999) x = 1;
        else if (GlobalVar.ShockCnt > 999) x = 2;
        else if (GlobalVar.ShockCnt > 99) x = 3;
        else if (GlobalVar.ShockCnt > 9) x = 3;
        t = GlobalVar.ShockCnt;
        NumberToAscii(buf, t, 0xFF);
        OLED_ShowString16x24SH(0, y,  1, "      ");
        OLED_ShowString8x16Num(x, 44, 1, buf);
    }

    x = FREQUENCY_POSX;
    y = FREQUENCY_POSY;
    t = GlobalVar.FrequencyValue ;
    if (t > 9) x = 2;
    NumberToAscii(buf, t, 0xFF);
    OLED_ShowString16x24SH(0, y, 0, "      ");
    OLED_ShowString16x24SH(x, y, 0, buf);


    x = RUNSTATUS_POSX;
    y = RUNSTATUS_POSY;
    if (Flag.Start)
    {
    	OLED_ShowStringSHO(x, y, 1, 6);	
    }
    else
    {
        OLED_ShowStringSHO(x, y, 1, 1);		
		
    }

  //  OLED_ShowStringSHO(FREQUENCY_POSX, FREQUENCY_POSY + 24, 0, 11);

	Record_StartAndStop_Value();
}

/*******************************************************************************
* Function Name  : Show_Menu
* Description    : ��ʾ������
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-09-07
* Author         : LL
*******************************************************************************/
 void Show_MenuInit(void)
{
    OLED_ShowFillSH(0, SHOCKCNT_POSY, 8, 24);
    OLED_ShowFillSH(0, 104, 8, 24);
    //OLED_ShowStringSHO(ENERGY_POSX, ENERGY_POSY + 24, 0, 13);
    OLED_ShowStringSHO(FREQUENCY_POSX, FREQUENCY_POSY + 24, 0, 11);
}

/*******************************************************************************
* Function Name  : OLED_Init
* Description    : OLED��ʼ��

����IC SH1106G
��ʾ������132*64����ʾ������128*64����������ʾ��ַ��ʱ����Ҫ����������ʽ
��ʼ�е�ַ��2�������е�ַ��129

* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-09-07
* Author         : LL
*******************************************************************************/
#if 1
void OLED_Init(void)
{
    //�˿ڳ�ʼ��
    OLED_D0_OUT;
    OLED_D1_OUT;
    OLED_D2_OUT;
    OLED_D3_OUT;
    OLED_D4_OUT;
    OLED_D5_OUT;
    OLED_D6_OUT;
    OLED_D7_OUT;
    OLED_RST_OUT;
    OLED_DC_OUT;
    OLED_CS_OUT;
    OLED_WR_OUT;
    OLED_ENABLE_OUT;
	OLED_BK_OUT;
    
    OLED_BK_Set();
		
    OLED_ENABLE_Set();
    OLED_RST_Set();

    OLED_RST_Clr();

    OLED_RST_Set();


    OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
    OLED_WR_Byte(0x02, OLED_CMD); //---set low column address
    OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
    OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0xB0, OLED_CMD); //-Set Page Addressing Mode
    OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
    OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
    OLED_WR_Byte(0xA0, OLED_CMD); //--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    OLED_WR_Byte(0xC0, OLED_CMD); //Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    OLED_WR_Byte(0xA6, OLED_CMD); //--set normal display
    OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
    OLED_WR_Byte(0xAD, OLED_CMD); //--set Charge Pump enable/disable //VCC�����ڲ��������磬��ӵ���
    OLED_WR_Byte(0x8B, OLED_CMD); //--set(0x8A) disable(0x8B) enable
    OLED_WR_Byte(0x32, OLED_CMD); ///*0X30---0X33 set VPP 8V */
    OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset   Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD); //-not offset
    OLED_WR_Byte(0xD5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
    OLED_WR_Byte(0xF1, OLED_CMD); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
    OLED_WR_Byte(0x40, OLED_CMD); //Set VCOM Deselect Level
    OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
     
    OLED_Clear();
    OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
}
#endif
/*******************************************************************************
* Function Name  : OLED_Test
* Description    : OLED����
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-09-07
* Author         : LL
*******************************************************************************/
void OLED_Test(void)
{
    OLED_ShowStringSHO(0, 0, 1, 6);

}
/*******************************************************************************
* Function Name  : OLED_Process
* Description    : ��ʾ������
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-09-07
* Author         : LL
*******************************************************************************/
//	void Sreen_Init(void)
//	{
//	static bool temp =FALSE;
//	bool NowValue =FALSE;
//		temp =Flag.power;
//	if()
//	
//	}
/*******************************************************************************
* Function Name  : OLED_Process
* Description    : ��ʾ������
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-09-07
* Author         : LL
*******************************************************************************/
void OLED_Process(void)
{
	
    if (Flag.OLED_Flash)
    {
        Flag.OLED_Flash = FALSE;

        Show_Menu();
    }
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */
