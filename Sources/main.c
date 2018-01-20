/**************************************************************************************************/
/*-----------------------------  飞思卡尔杯智能小车大赛   省赛版本---------------------------------*/
/*----------------------------  制作人：张霖 张凯淇 张阳 -----------------------------------------*/
/**************************************************************************************************/

/**************************************************************************************************/
/*-----------------------------------  文件名 ： main.c  ---------------------------------------*/
/*----------------------------  功能 主函数和定时器所在，核心文件 --------------------------*/
/**************************************************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Include.h"


extern int ADdataokflag; //AD数据有效
extern int Crossflag;
extern int Wandaoflag;                                               
extern int duojizhankong; 
extern int modeflag;
int forecast[15];//弯道识别
int Tingcheflag;  //停车标志位
int LINE_V;
int TC0count; 
unsigned int f1,f2;
int startline=0;
unsigned int speed;
unsigned int modecount;

/*----------------------------------主函数--------------------------------------*/	
//main.c : Start program  here ...
/*-------------------------------------------------------------------------------*/
void main(void) 
{                                  
  DisableInterrupts;  
  init();  
  //LEDtest(); 
  longdelay(2000);   //2s延时
  PTT_PTT7=1;
	PTM_PTM0=1;
  //TC0=11320; //54ms
  modeflag=0;

  
  EnableInterrupts;//离线调试的时候如果不禁止此项就会因为18号中断而中止程序执行
	while(1)
	{
	  MOD_init();
	 /*if(PORTA_PA1==0)//干簧管扫描 
	 {
	    longdelay(1);//机械开关防抖
	    startline++;
      if(startline==3)
        Tingcheflag=1;
	 }
	 if(Tingcheflag) 
  	 {
  	  while(1){
  	    DisableInterrupts;
  	    PWMDTY23=2400;
  	 }调试禁止*/ 
	 }
}
	
//#pragma CODE_SEG __NEAR_SEG NON_BANKED //禁止随便用近调用
#pragma CODE_SEG NON_BANKED
void interrupt 8 timer0(void)
{
   DisableInterrupts;
   TFLG1&=0x01 ;
   PACTL_PAEN=0;
   TC0=(TC0+187)%65525;                 //定时1ms 则1125
   f1=PACNT; //先把PACNT值存进变量中 再在实时调试时读取 否则PACNT分高低位读取会出错误
   /*--------------------此参数需整定------------------------------------------ */
   speed=f1*4320/1260;  //(f1/200*1*10^(-3))*(40/105)*0.18   速度单位：厘米/秒 
   /*轮周长180mm 传动比105/40
      车速2m/s时 车轮转速11r/s  编码器转速 29.17r/s   脉冲数5833/s 脉冲周期0.1714ms 每次脉冲数35个*/                
   PITTF_PTF=1;//清中断标志位 
   PACTL_PAEN=1;
   PACNT=0x00; //计数器归零
   Process();
   Control();
  EnableInterrupts;    
}
//#pragma CODE_SEG __NEAR_SEG NON_BANKED
#pragma CODE_SEG NON_BANKED
void interrupt 24 externinterrupt(void)                       
{
  DisableInterrupts;
  //PIEJ=0x00;
  longdelay(1);                            //干簧管是机械开关 要防抖 以后的干簧管要加电容防抖
  PIFJ=0x80;
  startline++;
  if(startline==2)
    Tingcheflag=1;
  EnableInterrupts;
  
} 