/**
  ******************************************************************************
  * @file    oled_parallel.c
  * @author  LL
  * @version V0.0.0
  * @date    2016-09-02
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/
#ifndef __OLED_PARALLEL_H__
#define __OLED_PARALLEL_H__			  	 

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED端口定义----------------  					   

void delay_ms(unsigned int ms);
		     

//OLED控制用函数
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_ShowCharSHO(u8 x, u8 y, u8 displaymode, u8 num);
void OLED_ShowString8x16Num(u8 x, u8 y, u8 displaymode, u8 *chr);
void OLED_Show8X16Num(u8 x, u8 y, u8 displaymode, u8 chr);

void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
extern void OLED_Test(void);
extern void OLED_Process(void);
void Show_MenuInit(void);
extern void OLED_Clear(void);
#endif /* __OLED_PARALLEL_H__ */ 

/* ************ ****** ************ THE FILE END  ************ ****** ************ */				
