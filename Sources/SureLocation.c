/*************************************************************************************************/
/*------------------------------文件名：SureLocation.c-------------------------------------------*/
/*---------------------------功能：确定前后两排黑线相对位置--------------------------------------*/
/*************************************************************************************************/
#include "Include.h"
extern int Piancha;
extern int ADdatahis[HISTORYNUM][FRONTNUM];//传感器历史数值，0为目前值
extern int ADdata[FRONTNUM];//传感器数据最终值
extern char ADdataOkflag;
int location;                                     //主位置
/*-----------------------------主位置检测-----------------------------*/	
//函数返回值int
//输入参数void 
//获取前排位置
/*-----------------------------------------------------------------------------*/
int SureLocationTest(void)
{
   char loctemp,lastlocation;
   lastlocation=location;
   loctemp=Max(ADdata,FRONTNUM);                     //大区区段
   if(loctemp==7) 
   {
      if(ADdata[6]<37) 
      {
        if(ADdata[7]>30) 
        {
          if(ADdata[7]<42)
            location=5;
          if(ADdata[7]>42)
            location=4;
        }
        else
        {
          if(ADdata[7]>17)
            location=6;
          else
          {
            if(ADdata[7]>7)
              location=7;
            else
            {
              if(ADdata[7]>4)
                location=8;
              else 
                location=11;
            }
          }
        }
      } 
      else
      {
        if(ADdata[6]<48)
          location=3;
        else
          location=2;
      }
   }
   if(loctemp==6) 
   {
     if(ADdata[7]>47)
     {
        location=2;
     } 
     else
     {
        if(ADdata[6]>38)
          location=1;
        else 
        {
          location=0;
        }    
        
      } 
   }
   if(loctemp==2) 
   {
      location=0;    
   }
   if((loctemp==2)&&(ADdata[0]<45)) 
     {
       if(ADdata[2]>52)
        location=-2;
       else
       {
        location=-1;
       }
     }
   if((loctemp==2)&&(ADdata[0]>45)) 
      location=-3;
   if(loctemp==0&&(ADdata[1]>40)) 
   {
      if(ADdata[0]<56)
        location=-3;
      else
        location=-4;
   }
  if(loctemp==0&&(ADdata[1]<40)) 
   {
       PORTB=0XFF;
      if(ADdata[0]>28)
      {
        if(ADdata[0]>59)
        location=-5;
        else
        {
          if(ADdata[0]>47)
            location=-6;
          else
            location=-7;
        }
      }
      else 
      {
        if(ADdata[0]>11)
          location=-8;
        else
          location=-11;
      }
   }

if(ADdata[loctemp]<8) 
   {
      if((lastlocation<0)&&(Piancha>0)) //在跑道左边且向左转
        location=10;
      if((lastlocation<0)&&(Piancha<0)) //在跑道左边且向右转
        location=-10;
      if((lastlocation>0)&&(Piancha>0)) //在跑道右边且向左转
        location=10;
      if((lastlocation>0)&&(Piancha<0)) //在跑道右边且向右转
        location=-10;
   }
   return (location);
}