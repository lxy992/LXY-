/**
  ******************************************************************************
  * @file    uart_printf.h
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/



#ifndef __UART_PRINTF_H_
#define __UART_PRINTF_H_

#define UART_PRINTF_EN 1 //使能打印函数
#define UART_PRINTF_DEBUG 1//使能打印宏定义

#if UART_PRINTF_DEBUG

#if UART_PRINTF_EN
extern void PrintfNumberHEX(unsigned long int num);
extern void PrintfNumberBCD(unsigned long int num);
extern void PrintfNumberBCDNOCR(unsigned long int num);
extern void PrintfMassages(char *buf);
extern void PrintfMassagesNOCR(char *buf);
extern void PrintfMassagesHEX(unsigned char *buf, unsigned int len);
#endif /* UART_PRINTF_EN */

#define DEBUG_PRINTFMASSAGES(buf)         PrintfMassages(buf)
#define DEBUG_PRINTFMASSAGESNOCR(buf)     PrintfMassagesNOCR(buf)
#define DEBUG_PRINTFMASSAGESHEX(buf,len)  PrintfMassagesHEX(buf,len)
#define DEBUG_PRINTFNUMBERHEX(d)          PrintfNumberHEX(d) 
#define DEBUG_PRINTFNUMBERBCD(d)          PrintfNumberBCD(d)
#define DEBUG_PRINTFNUMBERBCDNOCR(d)      PrintfNumberBCDNOCR(d)
#else
#define DEBUG_PRINTFMASSAGES(buf) 
#define DEBUG_PRINTFMASSAGESNOCR(buf) 
#define DEBUG_PRINTFMASSAGESHEX(buf,len) 
#define DEBUG_PRINTFNUMBERHEX(d)
#define DEBUG_PRINTFNUMBERBCD(d)
#define DEBUG_PRINTFNUMBERBCDNOCR(d)
#endif /*UART_PRINTF_DEBUG*/

#endif /* __UART_PRINTF_H_ */
