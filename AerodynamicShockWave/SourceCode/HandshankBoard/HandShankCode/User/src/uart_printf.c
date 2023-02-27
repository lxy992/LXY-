
  /******************************************************************************
  * @file    uart_printf.c
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   
  ******************************************************************************
  ******************************************************************************/

#include "config.h"


#if UART_PRINTF_EN
/*******************************************************************************
* Function Name  : PrintfMassages
* Description    : 打印信息到缓冲区,自动加回车换行
                   例如：PrintfMassages("this is printfchar");
                         PrintfMassages("");打印一个回车换行。
* Input          : buf:要打印的信息          
* Output         : None
* Return         : None
* Date           : 2016-03-17
* Author         : LL
*******************************************************************************/
void PrintfMassages(char *buf)
{
  while (*buf)
  {
	  put_char(*buf);
	  buf++;
  }
	put_char(0x0D);
	put_char(0x0A);
}

/*******************************************************************************
* Function Name  : PrintfMassagesNOCR
* Description    : 打印信息到缓冲区，不加回车换行
                   例如：PrintfMassages("this is printfchar");
* Input          : buf:要打印的信息          
* Output         : None
* Return         : None
* Date           : 2016-03-17
* Author         : LL
*******************************************************************************/
void PrintfMassagesNOCR(char *buf)
{
  while (*buf)
  {
	  put_char(*buf);
	  buf++;
  }
}

/*******************************************************************************
* Function Name  : PrintfMassagesHEX
* Description    : 将数据信息转化为16进制打印，自动加回车换行
                   
* Input          : buf:要打印的信息 
                   len:信息长度
* Output         : None
* Return         : None
* Date           : 2016-03-17
* Author         : LL
*******************************************************************************/
void PrintfMassagesHEX(unsigned char *buf, unsigned int len)
{ 
  unsigned char i,x;
  for (i=0; i<len; i++)
  {
    x = (buf[i]>>4)&0x0F;
    x = HexToAsciiTab[x];
    put_char(x);
    x = buf[i]&0x0F;
    x = HexToAsciiTab[x];
    put_char(x);
	put_char(0x20);//加空格
  }
	put_char(0x0D);
	put_char(0x0A);
}

/*******************************************************************************
* Function Name  : PrintfNumberHEX
* Description    : 将数据以十六进制字符发送，自动加回车换行
                   
* Input          : buf:要打印的信息          
* Output         : None
* Return         : None
* Date           : 2016-03-17
* Author         : LL
*******************************************************************************/
void PrintfNumberHEX(unsigned long int num)
{
  unsigned char i,j;
  unsigned char buf[11];
  buf[0] = '0';
  buf[1] = 'x';
  for(i=0; i<8; i++)
  {
    j= (num>>(7-i)*4) & 0x0f;
    buf[i+2] = HexToAsciiTab[j]; 
  }
  buf[10] = '\0';
  PrintfMassages((char *)buf);
}

/*******************************************************************************
* Function Name  : PrintfNumberBCD
* Description    : 将数据以十进制字符发送，自动加回车换行
                   
* Input          : num:要打印的数值          
* Output         : None
* Return         : None
* Date           : 2016-03-17
* Author         : LL
*******************************************************************************/
void PrintfNumberBCD(unsigned long int num)
{
  unsigned char buf[11];
  NumberToAscii(buf,num,0xFF);
  PrintfMassages((char *)buf);
}

/*******************************************************************************
* Function Name  : PrintfNumberHEX
* Description    : 将数据以十进制字符发送，自动加逗号
                   
* Input          : num:要打印的数值          
* Output         : None
* Return         : None
* Date           : 2016-03-17
* Author         : LL
*******************************************************************************/
void PrintfNumberBCDNOCR(unsigned long int num)
{
  unsigned char buf[11];
  NumberToAscii(buf,num,0xFF);
  PrintfMassagesNOCR((char *)buf);
  put_char(',');
}
#endif //UART_PRINTF_EN

/* ************ ****** ************ THE FILE END  ************ ****** ************ */				