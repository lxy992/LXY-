
#ifndef _SERIAL_H
#define _SERIAL_H

#define UART_DAT_LENGTH      80

typedef struct
{
    u8   Receive_Buf[UART_DAT_LENGTH];  //数据缓存区
    u8  *receive_last;                  //接收末尾
    u8  *get_last;                      //获取末尾
    u8   receive_signal_flag;           //有收到数据标志位 
    u8   receive_full_flag;             //接收满标志位
    u8   receive_length;                //接收长度
}UsartReceiveData_t;

extern  UsartReceiveData_t  DWIN_COM;
extern  UsartReceiveData_t  LEFT_HAND_COM;
extern  UsartReceiveData_t  RIGHT_HAND_COM;

void UART_Init(USART_TypeDef* USARTx ,u32 baud_rate);
void UART_SendData(USART_TypeDef* USARTx,u8 * buffer, u16 size);
void UART_ReceiveValueInit(UsartReceiveData_t *Uart_Device_Rx ,u8 Length);
u8   UART_GetByte (UsartReceiveData_t *Uart_Device_Rx);
void UART_Receive (UsartReceiveData_t *Uart_Device_Rx,u8 Data);

#endif

