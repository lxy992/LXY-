
#ifndef _SERIAL_H
#define _SERIAL_H

#define UART_DAT_LENGTH      80

typedef struct
{
    u8   Receive_Buf[UART_DAT_LENGTH];  //���ݻ�����
    u8  *receive_last;                  //����ĩβ
    u8  *get_last;                      //��ȡĩβ
    u8   receive_signal_flag;           //���յ����ݱ�־λ 
    u8   receive_full_flag;             //��������־λ
    u8   receive_length;                //���ճ���
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

