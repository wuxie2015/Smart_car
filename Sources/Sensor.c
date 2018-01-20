/*************************************************************************************************/
/*------------------------------文件名：Sensor.c-------------------------------------------------*/
/*---------------------------功能：获取处理传感器数据---------------------------------------------*/
/*************************************************************************************************/
#include "Include.h"

extern int Tingcheflag;
int Crossflag=0;
/*------------------------------以下传感器相关参数------------------------------*/
int ADdataorigenal[FRONTNUM][TESTTIME];//传感器数据初值
int ADdatacur[FRONTNUM];//传感器数据有效值
int ADdatahis[HISTORYNUM][FRONTNUM];//传感器历史数值，0为目前值,为二值化之后的值
int ADdataflag[FRONTNUM];//传感器二值化之后大小排序
int ADdata[FRONTNUM];//传感器数据最终值
int ADdataOkflag;

/*-----------------------------原始数据采集测试-----------------------------*/	
//函数返回值void
//输入参数void 
//[确定最佳采样频率和每次采集量，获取传感器的原始数据]
/*-----------------------------------------------------------------------------*/

void GetADdata(int testevetime) 
{
  unsigned char i,j,k;
  for(i=0;i<=FRONTNUM;i++)
    for(j=0;j<=testevetime;j++) 
    {
      ADdataorigenal[i][j]=GetATD0(i);    //第i个传感器取第j次
    }
  for(k=0;k<=FRONTNUM;k++)
  {
    ADdatacur[k]=Average(ADdataorigenal[k],testevetime);//Average(ADdataorigenal[i],TESTTIME);    //对多次取的值去最值后求平均值作为有效值
  }
  ATD0STAT0_SCF=1;                                    //AD数据清零                   
}
/*--------------------------------数据归一化-----------------------------------*/	
//函数返回值void
//输入参数void 
//存储历史数据
/*-----------------------------------------------------------------------------*/

void Getordinarydata()
{
    char i,j,temp,temp1;
    for(i=HISTORYNUM;i>1;i--) 
      for(j=FRONTNUM;j>=0;j--)
        ADdatahis[i-1][j]=ADdatahis[i-2][j];//历史数据前移一位
    for(j=0;j<FRONTNUM;j++)
       ADdatahis[0][j]=ADdatacur[j]*COE;
    /*temp=ADdatahis[1][0]-ADdatahis[0][0];
    temp1=abs(temp);
    if(temp1>10) 
    {
      ADdatahis[0][0]=ADdatahis[1][0];
    }   */
}
/*----------------------------数据有效性校验----------------------------*/	
//函数返回值void
//入口参数void 返回值ADdataokflag
//确定AD数据是否有效
/*-----------------------------------------------------------------------------*/
void ADdataOk(void) 
{

    int flag,temp;
    temp=Max(ADdatahis[0],10);
    if(ADdatahis[0][temp]<=3)
      ADdataOkflag=0;              //数据无效
    else ADdataOkflag=1;      
    /*
      if(abs((duojiLastError-Carlocation))>6)
        ADdataOkflag=0;
    */
}
/*-------------------------获取运行时归一化后的数据-----------------------*/	
//函数返回值void
//输入参数void
//作用：将归一化后的数据存入ADdata
/*-----------------------------------------------------------------------------*/
void Process() 
{
  unsigned char i;     
    GetADdata(TESTTIME);
    Getordinarydata();
    ADdataOk();
    if(ADdataOkflag) 
    {      
     for(i=0;i<FRONTNUM;i++)
        ADdata[i]=ADdatahis[0][i];       //如果数据无效则AD值不更新
        //ADdata[7]=(ADdata[7]*28)/43;
    }
}

