
/*
 * uart0.h
 *
 * Created: 2016/3/16 16:14:16
 *  Author: Administrator
 */ 


#ifndef __UART_H_
#define __UART_H_


extern void put_char(unsigned char data);
extern void UART_PrintfNormal(uint8_t *buf, uint8_t len);
extern void UART_Init(void);
extern void USART_Process(void);

#endif /* UART0_H_ */

