/*************************************************************************************************/
/*-----------------------------------  文件名 ： Bace.c  --------------------------------------*/
/*-----------------------------  功能 ：基础功能，延时和数据处理 ---------------------------*/
/*************************************************************************************************/

#include "Include.h"		//包括延时、数据算术处理等

void delay(int delaytime)
	{//软件延时，24M总线频率时延时时间约为10us
	     int i;unsigned char j;
	     for (i=0;i<delaytime;i++)
	      for (j=0;j<24;j++)               
	       ; 	
	}
	
void longdelay(int delaytime)
	{//软件延时，24M总线频率时延时时间约为1ms
		 int i;
	     for (i=0;i<delaytime;i++)
	       delay(100);
	}

/*----------------------------求数组中中间值函数-------------------------------*/	
//函数返回值unsigned char bijiao[n/2];
//输入参数int bijiao[],unsigned char n
//求中值，bijiao[]为原始数据，n为数据量
/*---------------------------------------------------------------------------------*/	
int Midnum(int bijiao[],unsigned char n)
	{
		 int i,j,temp;
		 if(n==1)
		  return bijiao[0];
		 if(n==2)
		  return (bijiao[0] + bijiao[1])/2;
		 for(j=1;j<n;j++)
		   for(i=0;i<n-j;i++)
		   	{	   		 
		     if(bijiao[i]<bijiao[i+1])
		    	{
		    	  temp=bijiao[i];
		    	  bijiao[i]=bijiao[i+1];
		    	  bijiao[i+1]=temp;
		    	}
		   	}
		 if(n%2)
		 	return bijiao[(n-1)/2];
		 else
		 	return (bijiao[n/2] + bijiao[n/2 - 1])/2;
	}
	
/*-----------------------------求数组中最大值函数-------------------------------*/	
//函数返回值unsigned char temp
//输入参数int bijiao[],unsigned char n
//求最大值，bijiao[]为原始数据，n为数据量
/*---------------------------------------------------------------------------------*/	
unsigned char Max(int bijiao[],unsigned char n)
	{
      unsigned char i,temp=0;
	  for(i=0;i<n;i++)
	  	{
          if(bijiao[temp]<bijiao[i])
          temp=i;	  	
  	  	}
	  return(temp);
	}
	/*-----------------------------求数组平均值函数-------------------------------*/	
//函数返回值数组平均值
//输入参数int bijiao[],unsigned char n
//求平均值，bijiao[]为原始数据，n为数据量
/*---------------------------------------------------------------------------------*/
	int Average(int bijiao[],unsigned char n)//建立了一个原来数组的副本，否则应该是指针*int
	{
		unsigned char i = 0 ;
		int temp=0;
		for(i=0 ; i< n ; i++)
			{
				temp += bijiao[i] ;
			}
		return (temp/n) ;
	}
	
/*-----------------------------求数组中最小值函数-------------------------------*/	
//函数返回值unsigned char temp
//输入参数int bijiao[],unsigned char n
//求最小值bijiao[]为原始数据，n为数据量
/*---------------------------------------------------------------------------------*/
unsigned char Min(int bijiao[],unsigned char n)
	{
	  unsigned char i,temp=0;
	  for(i=0;i<n;i++)
	  	{
          if(bijiao[temp]>bijiao[i])
		  temp = i;
        }
	  return(temp);
	} //得到数组前n个元素中最小一个数的下标
	
/*----------------------------------数组排序按下标-------------------------------------*/
//函数返回值void
//输入参数int bijiao[],int flag[],int n
//参数 bijiao[] 原始数据,flag[]排序后的标号数组,n数组长度
/*----------------------------------------------------------------------------------------*/
void MaxtoMin(int bijiao[],int flag[],int n) 
	{       
	    int temp[10]={0};
	    int i;
	    if(n > 10)
	    	n = 10;
	    for(i = 0;i < n;i++)
	    	 temp[i] = bijiao[i];
 	    for(i = 0;i < n;i++)
		    {
		       flag[i] = Max(temp,n);
		       temp[flag[i]] = 1;//给一个很小的数值
		    }     
	}//将数组bijiao[]中数字从小到大排列的下标存入数组flag
/*----------------------------------绝对值函数-------------------------------------*/
//函数返回值int
//输入参数int
//返回输入数字的绝对值
/*----------------------------------------------------------------------------------------*/
int abs(int i) 
{
  int j;
  if(i>=0)
   j=i;
  else
   j=0-i;
  return(j);
}
/*----------------------------------PID定义部分-------------------------------------*/
//函数返回值int
//输入参数int bijiao[],int flag[],int n
//参数 bijiao[] 原始数据,flag[]排序后的标号数组,n数组长度
/*----------------------------------------------------------------------------------------*/

int PIDcalc1(int Pcoe,int Icoe,int Dcoe,int Error,int LastError,int PrevError,int SumError,int iError_max)//位置pid 用于驱动舵机
{ 
  int  dError,iError,pError;
  PrevError=LastError;
  LastError=Error;
  SumError+=Error;
  dError=Dcoe*(LastError-PrevError);
  iError=Icoe*(SumError);
  pError=Pcoe*Error;
  if(iError>iError_max)
    iError=0;
  return(pError+iError+dError);
}
int PIDcalc2(int PcoeS,int IcoeS,int DcoeS,int SetPoint,int CurrentPoint,int LastErrorS,int PrevErrorS,int PrevPrevErrorS,int Iterm_max)//增量式PID 用于驱动马达  currentpoint 当前测量值
{
   int Pterm,Iterm,Dterm;
   PrevPrevErrorS=PrevErrorS;
   PrevErrorS=LastErrorS;
   LastErrorS=SetPoint-CurrentPoint;
   Pterm=PcoeS*(LastErrorS-PrevErrorS);
   Iterm=IcoeS*LastErrorS;
   Dterm=IcoeS*(LastErrorS-2*PrevErrorS+PrevPrevErrorS);
   if(Iterm>Iterm_max)
    Iterm=0; 
   return(Pterm+Iterm+Dterm);   
}