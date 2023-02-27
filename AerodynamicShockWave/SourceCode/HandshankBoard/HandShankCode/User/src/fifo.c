/**
  ******************************************************************************
  * @file    fifo.c
  * @author  LL
  * @version V0.0.0
  * @date    2016-03-21
  * @brief

  ʹ�÷�����
  ��������������ͷ�ļ���
  #define __SIZE 16 //��������С
  #define  DATATYPE unsigned char //����������
  #define  DATATYPESIZE unsigned int//��������С����

  ���建�����������������ڱ��ļ���
  DATA Buffer[__SIZE];
  FiFo_StructType Rx;
  FiFo_StructType *pRx = &Rx;

  ��ͷ�ļ�������
  extern DATA Buffer[];
  extern FiFo_StructType *pRx;

  ����ʹ�ã�����ͷ�ļ�����
  FIFO_Init(pRx,__SIZE,Buffer);//��ʼ��
  FIFO_PutOne(pRx, Data);//�����ݵ�������
  FIFO_GetCount(pRx);//�õ���������������
  FIFO_GetOne(pRx, &Data);//�ӻ�����ȡ����
  ******************************************************************************
  ******************************************************************************
***/

#include "config.h"

DATATYPESIZE FIFO_GetCount(FiFo_StructType *FiFo)
{
    DATATYPESIZE temp;
    _DI();
    if (FiFo->Head >= FiFo->Tail)
        temp = FiFo->Head - FiFo->Tail;
    else
        temp = FiFo->Head + FiFo->len - FiFo->Tail;
    _EI();
    return temp;
}

/* Put one data in FIFO outside ISR. */
unsigned char FIFO_PutOne(FiFo_StructType *FiFo, DATATYPE Data)
{
    DATATYPESIZE DataCount;
    _DI();
	
    if (FiFo->Head >= FiFo->Tail)           //���㻺����ʣ��Ŀռ�
        DataCount = FiFo->Head - FiFo->Tail;
    else
        DataCount = FiFo->len  +  FiFo->Head - FiFo->Tail;
    _EI();
    if (DataCount < (FiFo->len - 1))
    {
        _DI();
        *(FiFo->Head) = Data;
        /* "*FIFO_RxFiFoHead = Data;"��Ҫ�ڹ��ж��½���,�ַ��͵Ĳ�������������� */
        if (FiFo->Head == (FiFo->pBuf + (FiFo->len - 1))) //ָ�뷵�ض���ͷ
            FiFo->Head = FiFo->pBuf;
        else
            FiFo->Head++;
        _EI();
        return 1;
    }
    return 0;
}

/* Get one data from FIFO outside ISR. */
unsigned char FIFO_GetOne(FiFo_StructType *FiFo, DATATYPE *pData)
{
    _DI();
    if (FiFo->Tail != FiFo->Head)
    {
        *pData = *(FiFo->Tail);
        if (FiFo->Tail == (FiFo->pBuf + (FiFo->len - 1)))
            FiFo->Tail = FiFo->pBuf;
        else
            FiFo->Tail++;
        _EI();
        return 1;
    }
    _EI();
    return 0;
}
/* Dump some data of FIFO */
void FIFO_Init(FiFo_StructType *FiFo, DATATYPESIZE Size, DATATYPE *Buf)
{
    _DI();
    FiFo->pBuf = Buf;
    FiFo->Head = FiFo->pBuf;
    FiFo->Tail = FiFo->pBuf;
    FiFo->len = Size;
    _EI();
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */