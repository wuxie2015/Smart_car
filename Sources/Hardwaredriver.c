/*************************************************************************************************/
/*----------------------------------  文件名 ： HardDriver.c  --------------------------------*/
/*---------------------------------  功能1 ： 初始化系统参数 --------------------------------*/
/*-----------------------------------  功能2 ： 采样底层函数 ---------------------------------*/
/*************************************************************************************************/
#include "Include.h"
extern int LINE_V;
int levelflag; //A口数值
int wandaoflag;//赛道识别策略
/*---------------------------------速度模式初始化---------------------------------*/	
//函数返回值void
//输入参数
/*----------------------------------------------------------------------------------*/
void MOD_init(void)
{
 levelflag=PORTA;
  switch(levelflag&0b00010101)           //低3位确定速度基本档位
  {
    case 0b00000000:  	LINE_V=38; 	break;
		case 0b00000001:  	LINE_V=42; 	break;
		case 0b00000100:  	LINE_V=46; 	break;
		case 0b00000101:  	LINE_V=50;	  break;	
		case 0b00010000:  	LINE_V=54; 	break;
		case 0b00010001:  	LINE_V=58; 	break; 
		case 0b00010101:  	LINE_V=62;   break;
		default: LINE_V=50;  break;
  }
  
  switch(levelflag&0b01000000) 
  {
    case 0b01000000: wandaoflag=1;//弯道
    case 0b00000000: wandaoflag=0;//直道
  }
}
/*--------------------------------LED指示灯显示---------------------------------*/	
//函数返回值void
//输入参数unsigned char sta
/*----------------------------------------------------------------------------------*/
void Xianshi(unsigned char sta)
{
  switch(sta)
  {
    case 1: PORTB=0x01;
    case 2: PORTB=0x02;
    case 3: PORTB=0x04;
    case 4: PORTB=0x08;
    case 5: PORTB=0x10;
    case 6: PORTB=0x20;
    case 7: PORTB=0x40;
    case 8: PORTB=0x80;
  }
}
/*--------------------------------LED指示灯测试---------------------------------*/	
//函数返回值void
//输入参数void
/*----------------------------------------------------------------------------------*/
void LEDtest(void) 
{
  Xianshi(1);
  longdelay(300);
  Xianshi(2);
  longdelay(300);
  Xianshi(3);
  longdelay(300);
  Xianshi(4);
  longdelay(300);
  Xianshi(5);
  longdelay(300);
  Xianshi(6);
  longdelay(300);
  Xianshi(7);
  longdelay(300);
  Xianshi(8);
  longdelay(300);
}
