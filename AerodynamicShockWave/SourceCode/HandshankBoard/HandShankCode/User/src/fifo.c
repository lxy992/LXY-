/**
  ******************************************************************************
  * @file    fifo.c
  * @author  LL
  * @version V0.0.0
  * @date    2016-03-21
  * @brief

  使用方法：
  定义数据类型在头文件中
  #define __SIZE 16 //缓冲区大小
  #define  DATATYPE unsigned char //缓冲区类型
  #define  DATATYPESIZE unsigned int//缓冲区大小类型

  定义缓冲区及操作变量，在本文件中
  DATA Buffer[__SIZE];
  FiFo_StructType Rx;
  FiFo_StructType *pRx = &Rx;

  在头文件中声明
  extern DATA Buffer[];
  extern FiFo_StructType *pRx;

  函数使用，包含头文件即可
  FIFO_Init(pRx,__SIZE,Buffer);//初始化
  FIFO_PutOne(pRx, Data);//放数据到缓冲区
  FIFO_GetCount(pRx);//得到缓冲区数据数量
  FIFO_GetOne(pRx, &Data);//从缓冲区取数据
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
	
    if (FiFo->Head >= FiFo->Tail)           //计算缓冲区剩余的空间
        DataCount = FiFo->Head - FiFo->Tail;
    else
        DataCount = FiFo->len  +  FiFo->Head - FiFo->Tail;
    _EI();
    if (DataCount < (FiFo->len - 1))
    {
        _DI();
        *(FiFo->Head) = Data;
        /* "*FIFO_RxFiFoHead = Data;"需要在关中断下进行,字符型的操作可能引起错误 */
        if (FiFo->Head == (FiFo->pBuf + (FiFo->len - 1))) //指针返回队列头
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