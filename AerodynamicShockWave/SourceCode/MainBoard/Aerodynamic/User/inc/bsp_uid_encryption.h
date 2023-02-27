
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


/****************** �������º��������Կ��֤ ******************/
/*
    ���룺
        pKey, ָ����Կ�Ĵ洢�����ָ�룬��Կ����ʼ�洢��ַ
        pUID, ָ��UID�Ĵ洢�����ָ�룬 UID����ʼ�洢��ַ
        pCustomID, ָ���û��趨���Զ���ID�洢�����ָ��
        keyLength, ��Կ���ֽڳ��ȣ���ѡ��ο�ö������ eKeyLengthType�Ķ���
        endiaType, ��С����ѡ�񣬿�ѡ��ο�ö������ eEndiaType�Ķ���
        AlgorithmNum������UID������Կ���㷨ѡ�񣬿�ѡ��ο�ö������ eAlgorithmType�Ķ���
    �����
        ��
    ���أ�
        ������Կ��֤�Ľ����
        0����ȷ
        1������
*/
char UID_Encryption_Key_Check(void *pKey,                      //[IN]
                              void *pUID,                      //[IN]
                              void *pCustomID,                 //[IN]
                              eKeyLengthType keyLength,        //[IN]
                              eEndiaType endiaType,            //[IN]
                              eAlgorithmType AlgorithmNum);    //[IN]
/****************** �������º��������Կ���� ******************/
/*
    ���룺
        pUID, ָ��UID�Ĵ洢�����ָ���û�Ӧ��Ԥ�ȷ���ã� UID����ʼ�洢��ַ
        pCustomID, ָ���û��趨���Զ���ID�洢�����ָ��
        keyLength, ��Կ���ֽڳ��ȣ���ѡ��ο�ö������ eKeyLengthType�Ķ���
        endiaType, ��С����ѡ�񣬿�ѡ��ο�ö������ eEndiaType�Ķ���
        AlgorithmNum������UID������Կ���㷨ѡ�񣬿�ѡ��ο�ö������ eAlgorithmType�Ķ���
    �����
        pKey, ָ��һ���ڴ��������ڴ�ż��������Կ
    ���أ�
        ������Կ��֤�Ľ����
        0����ȷ
        1������
*/
void UID_Encryption_Key_Calculate(void *pKey,                  //[OUT]
                                  void *pUID,                  //[IN]
                                  void *pCustomID,             //[IN]
                                  eKeyLengthType keyLength,    //[IN]
                                  eEndiaType endiaType,        //[IN]
                                  eAlgorithmType AlgorithmNum);//[IN]
								  

