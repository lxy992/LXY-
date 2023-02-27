/**
  ******************************************************************************
  * @file    crc16.c
  * @author  LL
  * @version V0.0.0
  * @date    2016-11-16
  * @brief
  ******************************************************************************
  ******************************************************************************
***/

#include "config.h"

#define CRC16_CHECKTABLE_EN 1

#if CRC16_CHECKTABLE_EN
//Table of CRC values for high-order byte
static unsigned char const auchCRCHi[] =
{

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40

} ;
//Table of CRC values for low-order byte
static unsigned char const auchCRCLo[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40
};

//CRC-16       x16+x15+x2+1
/*******************************************************************************
* Function Name  : CRC_16
* Description    : CRC16校验，校验与数据顺序有关
* Input          : puchMsg:校验的信息
                   usDataLen:信息长度
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t CRC_16(uint8_t *puchMsg, uint16_t usDataLen)
{
    uint8_t uchCRCHi; // high byte of CRC initialized
    uint8_t uchCRCLo; // low byte of CRC initialized
    uint16_t uIndex ; // will index into CRC lookup table

    uchCRCHi = 0xFF;
    uchCRCLo = 0xFF;

    while (usDataLen)
    {
        // Pass through message buffer
        uIndex = uchCRCHi ^ *puchMsg++ ; // calculate the CRC
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
        uchCRCLo = auchCRCLo[uIndex] ;
        usDataLen--;
    }
    return (((uint16_t)(uchCRCLo) << 8) | uchCRCHi);
}

#else

#define     CRC16_value     0xa001
/***************************************************************************
    CRC校验步骤：
1、设置CRC寄存器，并给其赋值FFFF(hex)
2、将数据的第一个8-bit字符与16位CRC寄存器的低8位进行异或，并把结果存入CRC寄存器
3、CRC寄存器向右移一位，MSB补零，移出并检查LSB
4、如果LSB为0，重复第三步；若LSB为1，CRC寄存器与多项式码相异或
5、重复第3与第4步直到8次移位全部完成。此时一个8-bit数据处理完毕
6、重复第2至第5步直到所有数据全部处理完成
7、最终CRC寄存器的内容即为CRC值
8、CRC(16位)多项式为 X16+X15+X2+1，其对应校验二进制位列为1 1000 0000 0000 0101
****************************************************************************/
uint16_t CRC_16(uint8_t *data, uint16_t len)
{
	uint8_t j;
	uint16_t i;
    uint16_t CRC_index = 0xffff;
    uint16_t buffer;
	
    for(i = 0; i < len; i++)
    {
        buffer = data[i]; //把数据取出来放在缓冲区
        CRC_index ^= buffer;
        for(j = 0; j < 8; j++)
        {
            if(CRC_index & 0x0001)
            {
                CRC_index >>= 1;
                CRC_index ^= CRC16_value;
            }
            else
            {
                CRC_index >>= 1;
            }
        }
    }
    return  CRC_index;
}

#endif

/*******************************************************************************
* Function Name  : CRC_16ALL_Len1Byte
* Description    :

CRC16校验，将信息和长度打包一起校验，使用于需要将长度一起校验的
数据长度是用1个字节表示

* Input          : puchMsg:校验的信息
                   usDataLen:信息长度
* Output         : None
* Return         : 16校验和
*******************************************************************************/
uint16_t CRC_16ALL_Len1Byte(uint8_t *puchMsg, uint8_t usDataLen)
{
    uint16_t i;
    uint8_t crc16buf[30];

    crc16buf[0] = usDataLen;
    for (i = 0; i < usDataLen; i++)
    {
        crc16buf[1 + i] = puchMsg[i];
    }
    return CRC_16(crc16buf, usDataLen + 1);
}

/*******************************************************************************
* Function Name  : CRC_16ALL_Len2Byte
* Description    :

CRC16校验，将信息和长度打包一起校验，使用于需要将长度一起校验的
数据长度是用2个字节表示

* Input          : puchMsg:校验的信息
                   usDataLen:信息长度
* Output         : None
* Return         : 16校验和
*******************************************************************************/
uint16_t CRC_16ALL_Len2Byte(uint8_t *puchMsg, uint16_t usDataLen)
{
    uint16_t i;
    uint8_t crc16buf[50];

    crc16buf[0] = (usDataLen >> 8) & 0xFF;
    crc16buf[1] = usDataLen & 0xFF;
    for (i = 0; i < usDataLen; i++)
    {
        crc16buf[2 + i] = puchMsg[i];
    }
    return CRC_16(crc16buf, usDataLen + 2);
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */


