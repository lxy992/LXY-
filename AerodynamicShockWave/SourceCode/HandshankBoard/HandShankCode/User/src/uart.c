
/**
  ******************************************************************************
  * @file    uart.h
  * @author  LL
  * @version V0.0.0
  * @brief
  ******************************************************************************
  ******************************************************************************
***/


#include "config.h"

#define TX_SIZE 128
#define RX_SIZE 56

#define TX0_SIZE 128
#define RX0_SIZE 56

DATATYPE Tx_Buffer[TX_SIZE];
static FiFo_StructType Tx;
FiFo_StructType *pTx = &Tx;

DATATYPE Rx_Buffer[RX_SIZE];
static FiFo_StructType Rx;
FiFo_StructType *pRx = &Rx;


void put_char(unsigned char data)
{
    while ((UART2->SR & UART2_SR_TXE) == RESET);
    UART2->DR = data;
}

/*
UART2接收中断函数
*/
INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{

    uint8_t data = 0;
    static uint8_t rx_count = 0;

    if(UART2_GetFlagStatus(UART2_FLAG_RXNE) != RESET)
    {
        UART2_ClearITPendingBit(UART2_IT_RXNE);

        data = UART2_ReceiveData8();

        FIFO_PutOne(pRx, data);
//#ifdef MCU_UPG_PROC
     //   if(UPG_ProcTime)
     //   {
            if(data == 'U')
                rx_count = 0;

            UPG_Buf[rx_count++] = data;

            if(rx_count == 4)
            {
                rx_count = 0;
                if(UPG_Proc())
                {
                    ClearBuffer();
                    UPG_Reset();
                }

                ClearBuffer();
            }
       // }
//#endif
    }
}

/*******************************************************************************
* Function Name  : UART_Init
* Description    : 串口初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-03-16
* Author         : LL
*******************************************************************************/
void UART_Init(void)
{
    GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);//TX
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);//RX

    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, ENABLE);
    UART2_Init(9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE,UART2_MODE_TXRX_ENABLE); 
    UART2_Cmd(ENABLE);
    UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
    FIFO_Init(pRx, RX_SIZE, Rx_Buffer);
    FIFO_Init(pTx, TX_SIZE, Tx_Buffer);
    Protocol_Init();

}

/*******************************************************************************
* Function Name  : UART_SendChar
* Description    : 将串口中缓冲区的数据发送出去
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-05-14
* Author         : LL
*******************************************************************************/
static void UART_SendChar(void)
{
    uint8_t data;

    if (FIFO_GetCount(pTx))
    {
        FIFO_GetOne(pTx, &data);
        put_char(data);
    }
}

/*******************************************************************************
* Function Name  : Recive_DataProcess
* Description    : 串口接收数据处理
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-05-14
* Author         : LL
*******************************************************************************/
static void UART_ReciveDataProcess(void)
{
    uint8_t data;

    if (FIFO_GetCount(pRx))//接收到数据
    {

        FIFO_GetOne(pRx, &data);
        Protocol_DataDecode(data, pProtocol_DataPort1);

    }
}

/*******************************************************************************
* Function Name  : USART_Process
* Description    : 串口处理
* Input          : None
* Output         : None
* Return         : None
* Date           : 2014-10-23
* Author         : LL
*******************************************************************************/
void USART_Process(void)
{
    UART_ReciveDataProcess();
    UART_SendChar();

}

/*******************************************************************************
* Function Name  : UART_PrintfNormal
* Description    : 将要发送到串口的数据打包处理，并写入发送缓冲区
* Input          : buf:要发送的信息
                   len:信息长度
* Output         : None
* Return         : None
* Date           : 2014-08-26
* Author         : LL
*******************************************************************************/
void UART_PrintfNormal(uint8_t *buf, uint8_t len)
{

    uint16_t i;
    uint8_t crcx;
    uint16_t crc16x;

    FIFO_PutOne(pTx, 0x53);
    FIFO_PutOne(pTx, 0x47);
    FIFO_PutOne(pTx, len);
    for (i = 0; i < len; i++)
    {
        FIFO_PutOne(pTx, buf[i]);
    }
    crc16x = CRC_16ALL_Len1Byte(buf, len);
    crcx = (crc16x >> 8) & 0xFF;
    FIFO_PutOne(pTx, crcx);
    crcx = crc16x & 0xFF;
    FIFO_PutOne(pTx, crcx);
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */

