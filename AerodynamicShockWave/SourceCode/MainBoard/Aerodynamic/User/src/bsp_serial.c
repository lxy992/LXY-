
#include "config.h"

/*---------------------------------------------------------------------------
* ������  : UART_GPIO_Init(void)
* ����    : ����GPIO ��ʼ��
* ����    : ���ھ��
* ���    : ��
* ����    : ��
---------------------------------------------------------------------------*/
void UART_GPIO_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   
    /* Enable peripheral clocks --------------------------------------------------*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO |\
                           RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,ENABLE);
  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   
	  /*Uart1 TX*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /*Uart1 RX*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
  

       /*Uart2 TX*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /*Uart2 RX*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

 
      /*Uart3 TX*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  /*Uart3 RX*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

  
        /*Uart4 RX*/
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);

	 /*********Debug TX4********/
	  /*Uart4 TX*/
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);
  
}

/*---------------------------------------------------------------------------
* ������  : UART_Init(USART_TypeDef* USARTx ,u32 BaudRate)
* ����    : ���ڳ�ʼ��
* ����    : ���ڱ�� ������ 
* ���    : ��
* ����    : ��
---------------------------------------------------------------------------*/
void UART_Init(USART_TypeDef* USARTx,u32 BaudRate)
{
     USART_InitTypeDef USART_InitStructure;

	  if (USARTx == USART1)
	  {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE); 
	  }
	  else if (USARTx == USART2)
	  {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE); 
	  }
	  else if (USARTx == USART3)
	  {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE); 
	  }    
	  else if (USARTx == UART4)
	  {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4 , ENABLE); 
	  }    
	  else
	  {
	    if (USARTx == UART5)
	    { 
	      RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	    }
	  }
	  
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /* Configure USARTz */
    USART_Init(USARTx, &USART_InitStructure);
    /* Enable USARTz Receive and Transmit interrupts */
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
    /* Enable the USART */
    USART_Cmd(USARTx, ENABLE);
}

/*---------------------------------------------------------------------------
* ������  : UART_SendData
* ����    : ��������
* ����    : ���ڶ˺ţ��跢�͵��ַ��������͵��ֽ�����
* ���    : ��
* ����    : ��
---------------------------------------------------------------------------*/
void UART_SendData(USART_TypeDef* USARTx,u8 * buffer, u16 size)
{	
    u8 i = 0;
	
    for(i=0;i<size;i++)
    {
        /* Write a character to the USART */
        USART_SendData(USARTx, * buffer);
        /* Loop until the end of transmission */
       while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	   buffer++;
    }  
}

//�����豸���ո�ֵ��ʼ��
void UART_ReceiveValueInit(UsartReceiveData_t *Uart_Device_Rx,u8 Length)
{
    Uart_Device_Rx->receive_last = Uart_Device_Rx->Receive_Buf;
    Uart_Device_Rx->get_last = Uart_Device_Rx->Receive_Buf;
    Uart_Device_Rx->receive_length = Length;
}

//�����豸����
void  UART_Receive (UsartReceiveData_t *Uart_Device_Rx,u8 Data)
{
    if(!Uart_Device_Rx->receive_full_flag)
    {
        *(Uart_Device_Rx->receive_last) = Data;  //�����ݷ�����ָ��ĵ�ַ
        Uart_Device_Rx->receive_last++;
        Uart_Device_Rx->receive_signal_flag = 1;

        if (Uart_Device_Rx->receive_last >= Uart_Device_Rx->Receive_Buf + Uart_Device_Rx->receive_length) // compare with address
            Uart_Device_Rx->receive_last = Uart_Device_Rx->Receive_Buf;

        if (Uart_Device_Rx->receive_last == Uart_Device_Rx->get_last)
            Uart_Device_Rx->receive_full_flag = 1;  
    }
}

//�����豸��ȡ�ֽ�����
u8 UART_GetByte (UsartReceiveData_t *Uart_Device_Rx)
{
    u8 data = 0;

    if (!Uart_Device_Rx->receive_signal_flag)  
    {  
      return 0;
    }
	
    data = *(Uart_Device_Rx->get_last);
    Uart_Device_Rx->get_last++;
    Uart_Device_Rx->receive_full_flag = 0;

    if (Uart_Device_Rx->get_last >= Uart_Device_Rx->Receive_Buf + Uart_Device_Rx->receive_length) 
        Uart_Device_Rx->get_last = Uart_Device_Rx->Receive_Buf;

    if (Uart_Device_Rx->get_last == Uart_Device_Rx->receive_last) //��ȡ��ʱ����ź�
       Uart_Device_Rx->receive_signal_flag = 0;

    return  data;
}


