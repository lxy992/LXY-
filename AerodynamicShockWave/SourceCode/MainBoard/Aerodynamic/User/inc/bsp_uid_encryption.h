
#pragma once
#include "string.h"

typedef enum 
{
    LENGTH_4 = 4,
    LENGTH_8 = 8,
    LENGTH_12 = 12
    
}eKeyLengthType;

typedef enum 
{
    LITTLE_ENDIA = 0,
    BIG_ENDIA
    
}eEndiaType;


typedef enum 
{
    ALGORITHM_0 = 0,
    ALGORITHM_1,
    ALGORITHM_2,
    ALGORITHM_3,
    ALGORITHM_4
    
}eAlgorithmType;


/****************** 调用以下函数完成密钥验证 ******************/
/*
    输入：
        pKey, 指向密钥的存储区域的指针，密钥的起始存储地址
        pUID, 指向UID的存储区域的指针， UID的起始存储地址
        pCustomID, 指向用户设定的自定义ID存储区域的指针
        keyLength, 密钥的字节长度，可选项参考枚举类型 eKeyLengthType的定义
        endiaType, 大小端序选择，可选项参考枚举类型 eEndiaType的定义
        AlgorithmNum，计算UID加密密钥的算法选择，可选项参考枚举类型 eAlgorithmType的定义
    输出：
        无
    返回：
        返回密钥验证的结果：
        0，正确
        1，错误
*/
char UID_Encryption_Key_Check(void *pKey,                      //[IN]
                              void *pUID,                      //[IN]
                              void *pCustomID,                 //[IN]
                              eKeyLengthType keyLength,        //[IN]
                              eEndiaType endiaType,            //[IN]
                              eAlgorithmType AlgorithmNum);    //[IN]
/****************** 调用以下函数完成密钥计算 ******************/
/*
    输入：
        pUID, 指向UID的存储区域的指针用户应该预先分配好， UID的起始存储地址
        pCustomID, 指向用户设定的自定义ID存储区域的指针
        keyLength, 密钥的字节长度，可选项参考枚举类型 eKeyLengthType的定义
        endiaType, 大小端序选择，可选项参考枚举类型 eEndiaType的定义
        AlgorithmNum，计算UID加密密钥的算法选择，可选项参考枚举类型 eAlgorithmType的定义
    输出：
        pKey, 指向一块内存区域，用于存放计算出的密钥
    返回：
        返回密钥验证的结果：
        0，正确
        1，错误
*/
void UID_Encryption_Key_Calculate(void *pKey,                  //[OUT]
                                  void *pUID,                  //[IN]
                                  void *pCustomID,             //[IN]
                                  eKeyLengthType keyLength,    //[IN]
                                  eEndiaType endiaType,        //[IN]
                                  eAlgorithmType AlgorithmNum);//[IN]
								  

