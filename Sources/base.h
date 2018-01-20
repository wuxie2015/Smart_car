/*************************************************************************************************/
/*-----------------------------------  文件名 ： Bace.h  --------------------------------------*/
/*-------------------------------  功能 ：Base.c 对应头文件 ---------------------------------*/
/*************************************************************************************************/
                             //结构体定义在头文件中
void delay(int delaytime);				//延时大约3us
void longdelay(int delaytime);		//长延时大约3ms,没有细算，本应该用定时器的，O(∩_∩)O哈哈~
unsigned char Max(int bijiao[],unsigned char n); //最大值，返回数组下标
unsigned char Min(int bijiao[],unsigned char n); //最小值，返回数组下标
int Midnum(int bijiao[],unsigned char n);//中值，返回中间数下标
void MaxtoMin(int bijiao[],char  flag[],int n);
int Average(int bijiao[],unsigned char n);//求中间值
int abs(int i);
int PIDcalc1(int Pcoe,int Icoe,int Dcoe,int Error,int LastError,int PrevError,int SumError,int iError_max);//位置pid 用于驱动舵机;//位置式PID 用于驱动舵机 
int PIDcalc2(int PcoeS,int IcoeS,int DcoeS,int SetPoint,int CurrentPoint,int LastErrorS,int PrevErrorS,int PrevPrevErrorS,int Iterm_max);//增量式PID 用于驱动马达  currentpoint 当前测量值