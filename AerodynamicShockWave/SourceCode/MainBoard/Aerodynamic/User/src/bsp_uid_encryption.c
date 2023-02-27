
#include "config.h"

/****************************************** UID�����㷨 start ******************************************/
/* UID�����㷨0 */
static void Algorithm_0(char *pCustomID, char *pUID, char KeyLength, char *pKey)
{
    pKey[0] = (pUID[0]    ^ (pCustomID[3] & pCustomID[2])) ^ pCustomID[1];
    pKey[1] = pCustomID[1] | (pUID[2]    ^ (pCustomID[3] & pCustomID[3]));
    pKey[2] = (pCustomID[2] ^ pCustomID[1]) | (pUID[0]    ^ pCustomID[0]);
    pKey[3] = ((pCustomID[3] & pCustomID[0]) ^ pCustomID[1]) | pUID[2];
    
    if(KeyLength == 8)
    {
        pKey[4] = (pUID[4]    ^ (pCustomID[7] & pCustomID[5])) ^ pCustomID[5];
        pKey[5] = pCustomID[5] | (pUID[6]    ^ (pCustomID[4] & pCustomID[7]));
        pKey[6] = (pCustomID[6] ^ pCustomID[5]) | (pUID[7]    ^ pCustomID[4]);
        pKey[7] = ((pCustomID[7] & pCustomID[4]) ^ pCustomID[6]) | pUID[6];
    }
    if(KeyLength == 12)
    {
        pKey[0] = (pUID[0]    ^ (pCustomID[1] & pCustomID[2])) ^ pCustomID[3];
        pKey[1] = pCustomID[1] | (pUID[1]    ^ (pCustomID[3] & pCustomID[4]));
        pKey[2] = (pCustomID[2] ^ pCustomID[1]) | (pUID[2]    ^ pCustomID[5]);
        pKey[3] = ((pCustomID[3] & pCustomID[1]) ^ pCustomID[5]) | pUID[3];
        pKey[4] = (pUID[4]    ^ (pCustomID[5] & pCustomID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[5]    ^ (pCustomID[7] & pCustomID[8]));
        pKey[6] = (pCustomID[6] ^ pCustomID[7]) | (pUID[6]    ^ pCustomID[9]);
        pKey[7] = ((pCustomID[7] & pCustomID[8]) ^ pCustomID[9]) | pUID[7];
        pKey[8] = (pUID[8]    ^ (pCustomID[9] & pCustomID[10])) ^ pCustomID[11];
        pKey[9] = pCustomID[9] | (pUID[9]    ^ (pCustomID[11] & pCustomID[0]));
        pKey[10]= (pCustomID[10] ^ pCustomID[11]) | (pUID[0]   ^ pCustomID[1]);
        pKey[11]= ((pCustomID[11] & pCustomID[0]) ^ pCustomID[1]) | pUID[2];
    }
       
}

/* UID�����㷨1 */
static void Algorithm_1(char *pCustomID, char *pUID, char KeyLength, char *pKey)
{
    pKey[0] = ((pUID[0]    ^ pCustomID[1]) ^ pCustomID[2]) ^ pCustomID[3];
    pKey[1] = ((pCustomID[1] ^ pUID[1])    ^ pCustomID[3]) ^ pCustomID[0];
    pKey[2] = ((pCustomID[2] ^ pCustomID[1]) ^ pUID[2])    ^ pCustomID[3];
    pKey[3] = ((pCustomID[3] ^ pCustomID[1]) ^ pCustomID[0]) ^ pUID[3];
    
    if(KeyLength == 8)
    {
        pKey[4] = ((pUID[4]    ^ pCustomID[5]) ^ pCustomID[6]) ^ pCustomID[7];
        pKey[5] = ((pCustomID[5] ^ pUID[5])    ^ pCustomID[7]) ^ pCustomID[2];
        pKey[6] = ((pCustomID[6] ^ pCustomID[7]) ^ pUID[6])    ^ pCustomID[3];
        pKey[7] = ((pCustomID[7] ^ pCustomID[4]) ^ pCustomID[5]) ^ pUID[4];
    }
    if(KeyLength == 12)
    {
        pKey[0] = ((pUID[0]    ^ pCustomID[1]) ^ pCustomID[2]) ^ pCustomID[3];
        pKey[1] = ((pCustomID[1] ^ pUID[1])    ^ pCustomID[3]) ^ pCustomID[4];
        pKey[2] = ((pCustomID[2] ^ pCustomID[1]) ^ pUID[2])    ^ pCustomID[5];
        pKey[3] = ((pCustomID[3] ^ pCustomID[1]) ^ pCustomID[5]) ^ pUID[3];
        pKey[4] = ((pUID[4]    ^ pCustomID[5]) ^ pCustomID[6]) ^ pCustomID[7];
        pKey[5] = ((pCustomID[5] ^ pUID[5])    ^ pCustomID[7]) ^ pCustomID[8];
        pKey[6] = ((pCustomID[6] ^ pCustomID[7]) ^ pUID[6])    ^ pCustomID[9];
        pKey[7] = ((pCustomID[7] ^ pCustomID[8]) ^ pCustomID[9]) ^ pUID[7];
        pKey[8] = ((pUID[8]    ^ pCustomID[9]) ^ pCustomID[10]) ^ pCustomID[11];
        pKey[9] = ((pCustomID[9] ^ pUID[9])    ^ pCustomID[11]) ^ pCustomID[0];
        pKey[10]= ((pCustomID[10] ^ pCustomID[11]) ^ pUID[0])   ^ pCustomID[1];
        pKey[11]= ((pCustomID[11] ^ pCustomID[0]) ^ pCustomID[1]) ^ pUID[2];
    }
}


/* UID�����㷨2 */
static void Algorithm_2(char *pCustomID, char *pUID, char KeyLength, char *pKey)
{
    pKey[0] = ((pUID[0]    & pCustomID[1]) ^ pCustomID[2]) ^ pCustomID[3];
    pKey[1] = (pCustomID[1] ^ (pUID[1]    & pCustomID[3])) ^ pCustomID[0];
    pKey[2] = (pCustomID[2] | (pCustomID[1] ^ pUID[2]))    | pCustomID[3];
    pKey[3] = (pCustomID[3] ^ pCustomID[1]) | (pCustomID[0] ^ pUID[3]);
    
    if(KeyLength == 8)  
    {
        pKey[4] = (pUID[4]    ^ (pCustomID[5] & pCustomID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[5]    ^ (pCustomID[7] & pCustomID[2]));
        pKey[6] = ((pCustomID[6] & pCustomID[7]) ^ pUID[6])    | pCustomID[3];
        pKey[7] = (pCustomID[7] ^ pCustomID[4]) | (pCustomID[1] ^ pUID[2]);
    }
    if(KeyLength == 12)
    {
        pKey[0] = ((pUID[0]    & pCustomID[1]) ^ pCustomID[2]) ^ pCustomID[3];
        pKey[1] = (pCustomID[1] ^ (pUID[1]    & pCustomID[3])) ^ pCustomID[4];
        pKey[2] = (pCustomID[2] | (pCustomID[1] ^ pUID[2]))    | pCustomID[5];
        pKey[3] = (pCustomID[3] ^ pCustomID[1]) | (pCustomID[5] ^ pUID[3]);
        pKey[4] = (pUID[4]    ^ (pCustomID[5] & pCustomID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[5]    ^ (pCustomID[7] & pCustomID[8]));
        pKey[6] = ((pCustomID[6] & pCustomID[7]) ^ pUID[6])    | pCustomID[9];
        pKey[7] = (pCustomID[7] ^ pCustomID[8]) | (pCustomID[9] ^ pUID[7]);
        pKey[8] = (pUID[8]    ^ (pCustomID[9] & pCustomID[10])) ^ pCustomID[11];
        pKey[9] = (pCustomID[9] ^ pUID[9])    | (pCustomID[11] & pCustomID[0]);
        pKey[10]= ((pCustomID[10] & pCustomID[11]) ^ pUID[0])   ^ pCustomID[1];
        pKey[11]= (pCustomID[11] ^ (pCustomID[0] & pCustomID[1])) ^ pUID[2];
    }
}

/* UID�����㷨3 */
static void Algorithm_3(char *pCustomID, char *pUID, char KeyLength, char *pKey)
{
    pKey[0] = ((pUID[0] & pCustomID[1]) ^ pCustomID[2]) ^ pCustomID[3];
    pKey[1] = (pUID[1] ^ (pCustomID[2] & pCustomID[3])) ^ pCustomID[4];
    pKey[2] = (pUID[2] | (pCustomID[3] ^ pCustomID[4])) | pCustomID[5];
    pKey[3] = (pUID[3] ^ pCustomID[4]) | (pCustomID[5] ^ pCustomID[6]);
    
    if(KeyLength == 8)  
    {
        pKey[4] = (pCustomID[4] ^ (pUID[5] & pCustomID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[6] ^ (pCustomID[7] & pCustomID[4]));
        pKey[6] = ((pCustomID[6] & pUID[7]) ^ pCustomID[6]) | pCustomID[3];
        pKey[7] = (pCustomID[7] ^ pUID[4]) | (pCustomID[5] ^ pCustomID[6]);
    }
    if(KeyLength == 12)
    {
        pKey[0] = ((pUID[0] & pCustomID[1]) ^ pCustomID[2]) ^ pCustomID[3];
        pKey[1] = (pUID[1] ^ (pCustomID[2] & pCustomID[3])) ^ pCustomID[4];
        pKey[2] = (pUID[2] | (pCustomID[3] ^ pCustomID[4])) | pCustomID[5];
        pKey[3] = (pUID[3] ^ pCustomID[4]) | (pCustomID[5] ^ pCustomID[6]);
        pKey[4] = (pCustomID[4] ^ (pUID[5] & pCustomID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[6] ^ (pCustomID[7] & pCustomID[8]));
        pKey[6] = ((pCustomID[6] & pUID[7]) ^ pCustomID[6]) | pCustomID[9];
        pKey[7] = (pCustomID[7] ^ pUID[8]) | (pCustomID[9] ^ pCustomID[10]);
        pKey[8] = (pCustomID[8]  ^ (pCustomID[9]  & pUID[10])) ^ pCustomID[11];
        pKey[9] = (pCustomID[9]  ^ pCustomID[10]) | (pUID[11] & pCustomID[0]);
        pKey[10]= ((pCustomID[10] & pCustomID[11]) ^ pUID[0])  ^ pCustomID[1];
        pKey[11]= (pCustomID[11] ^ (pCustomID[0]  & pUID[1]))  ^ pCustomID[2];
    }
}


/* UID�����㷨4 */
static void Algorithm_4(char *pCustomID, char *pUID, char KeyLength, char *pKey) 
{
    pKey[0] = (pUID[0] & pCustomID[1]) ^ (pUID[2] & pCustomID[3]);
    pKey[1] = (pCustomID[1] ^ (pUID[2] & pCustomID[3])) ^ pUID[4];
    pKey[2] = (pCustomID[2] | (pUID[3] ^ pUID[4])) | pCustomID[3];
    pKey[3] = (pUID[3] - pCustomID[0]) | (pCustomID[2] ^ pUID[3]);
    
    if(KeyLength == 8)  
    {
        pKey[4] = (pUID[4] ^ (pCustomID[5] & pUID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[6] ^ (pCustomID[7] & pUID[8]));
        pKey[6] = ((pCustomID[6] & pUID[7]) ^ pCustomID[6]) | pUID[9];
        pKey[7] = (pUID[7] - pCustomID[3]) | (pUID[9] ^ pCustomID[5]);
    }
    if(KeyLength == 12)
    {
        pKey[0] = (pUID[0] & pCustomID[1]) ^ (pUID[2] & pCustomID[3]);
        pKey[1] = (pCustomID[1] ^ (pUID[2] & pCustomID[3])) ^ pUID[4];
        pKey[2] = (pCustomID[2] | (pUID[3] ^ pUID[4])) | pCustomID[5];
        pKey[3] = (pUID[3] - pCustomID[4]) | (pCustomID[5] ^ pUID[6]);
        pKey[4] = (pUID[4] ^ (pCustomID[5] & pUID[6])) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[6] ^ (pCustomID[7] & pUID[8]));
        pKey[6] = ((pCustomID[6] & pUID[7]) ^ pCustomID[6]) | pUID[9];
        pKey[7] = (pUID[7] - pCustomID[8]) | (pUID[9] ^ pCustomID[10]);
        pKey[8] = (pCustomID[8] ^ (pUID[9] & pCustomID[10])) ^ pUID[11];
        pKey[9] = (pUID[9] ^ pCustomID[10]) | (pUID[11] & pCustomID[0]);
        pKey[10]= ((pUID[10] & pCustomID[11]) ^ pUID[0])  ^ pCustomID[1];
        pKey[11]= ((pCustomID[11] - pUID[0]) & pCustomID[1])  ^ pUID[2];
    }
}



/* ����UID�����㷨 ����ָ������ */
typedef void (*Algorithm_Fun_Typedef)(char *pCustomID, char *pUID, char KeyLength, char *pKey);
/* ����UID�����㷨 ����ָ������ */
static const Algorithm_Fun_Typedef Algorithm_Fun_Array[5] = 
{
    Algorithm_0,
    Algorithm_1,
    Algorithm_2,
    Algorithm_3,
    Algorithm_4,
};
	
/****************************************** UID�����㷨 end ******************************************/    
/* ��С�����л� */      
#define BigLittleSwap32(A)          ((((unsigned int)(A) & 0xFF000000) >> 24) | \
                                     (((unsigned int)(A) & 0x00FF0000) >> 8)  | \
                                     (((unsigned int)(A) & 0x0000FF00) << 8)  | \
                                     (((unsigned int)(A) & 0x000000FF) << 24)) 
                                        
void LittleEndia_BigEndia_Interconvert_32(unsigned int* pBuffer, unsigned int WordLen)
{
    unsigned int i;
    
    for(i = 0; i < WordLen; i ++)
    {
        pBuffer[i] = BigLittleSwap32(pBuffer[i]);
    }
    return;
}  



static Algorithm_Fun_Typedef   pAlgorithm_Fun;
/****************** UID �㷨������Կ��֤ ******************/
char UID_Encryption_Key_Check(void *pKey,
                              void *pUID,
                              void *pCustomID,
                              eKeyLengthType keyLength, 
                              eEndiaType endiaType, 
                              eAlgorithmType AlgorithmNum)
{   
    char KeyBuf[12] = {0};//��ʱ�洢����õ���Key
    
    
    /* ѡ���㷨��ʽ */
    pAlgorithm_Fun = Algorithm_Fun_Array[AlgorithmNum];
    /* ����KEY */
    (*pAlgorithm_Fun)(pCustomID, pUID, keyLength, KeyBuf);   

    if(endiaType == BIG_ENDIA)
    {
        LittleEndia_BigEndia_Interconvert_32((unsigned int*)KeyBuf, keyLength/sizeof(unsigned int));
    }
    /* �Ƚ��ѻ���������¼��Key��ʹ����ͬ�㷨����õ���key�Ƿ���ͬ */
    if(memcmp(pKey, KeyBuf, keyLength)) 
    {
        return 1;
    }
    
    return 0; 
}
							  
/****************** UID �㷨������Կ���� ******************/
void UID_Encryption_Key_Calculate(void *pKey,
                                 void *pUID,
                                 void *pCustomID,
                                 eKeyLengthType keyLength, 
                                 eEndiaType endiaType, 
                                 eAlgorithmType AlgorithmNum)
{    
    /* ѡ���㷨��ʽ */
    pAlgorithm_Fun = Algorithm_Fun_Array[AlgorithmNum];
    /* ����KEY */
    (*pAlgorithm_Fun)(pCustomID, pUID, keyLength, pKey);   

    if(endiaType == BIG_ENDIA)
    {
        LittleEndia_BigEndia_Interconvert_32((unsigned int*)pKey, keyLength/sizeof(unsigned int));
    }
    
    return; 
}





