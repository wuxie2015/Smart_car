/*************************************************************************************************/
/*----------------------------------  文件名 ： Control.c  --------------------------------------*/
/*-----------------------------  功能 ： 确定舵机和电机输出量 -----------------------------------*/
/*************************************************************************************************/
#include"Include.h"
extern int Wandaoflag;                            //弯道检测标志位  
extern int Crossflag;                             //交叉道检测
extern char startlineflag;                        //起跑线检测
int SafeStopEnableFlag;                    //防撞允许标志位，当速度超过STOPSAFTMIN时启动允许
extern int Tingcheflag;
extern int location;
extern int modecount;
extern int forecast[15];
extern int wandaoflag;
/*------------------------------舵机参数-----------------------------*/
int Carlocation=0;                                 //小车位置
int Piancha;
int duojizhankong;
int directionlost[3]={0,0,0};     //方向偏差
int duojiPcoe=0;
int duojiIcoe=0;
int duojiDcoe=0;
int duojiLastError=0;
int duojiPrevError=0;
int duojiSumError=0;
int duojiiError_max=0;
int curvecount=0;               //弯道计数
int speedlimitcount=0;
int speedlimit=0;
int curvetostraight=1;
int straignttocurve=2;
/*---------------------------驱动电机参数----------------------------*/
int I_Term_max = 10 ; 					//积分限幅值
int I_Term_min = -10 ;					//积分限幅值
int speedset = 50 ;						//速度设定值，这里给的初值关系不大
int speedlost[3] = {0,0,0} ; 			//速度偏差    
int umotor = 0 ;						//上一次的电机电压输
int uout=0;
extern int speed ;							//当前速度（可以不是全局变量，但为调试方便
extern int LINE_V;
extern int PWMMOTOTA;
unsigned char lastStrategy = 1 ;		//bang-bang为1，pid为0,初始为bang-bang
int BANGBANGtoPID = 10 ; 				//Bang-Bang To PID 阈值
int PIDtoBANGBANG = 10	; 				//PID To Bang-Bang 阈值 
int last_PIDV;                       //最后一次PID增量                 
int speedzero ;                    	//用于撞车后停车
int WAN_V=100;
int motorPcoe=0;
int motorIcoe=0;
int motorDcoe=0;                                                                            ;
int motorLastError=0;
int motorPrevError=0;
int motorPrevPrevError=0;
int motorSumError=0;
int motoriError_max=0;
int PIDmotor=0;                   //当前马达PID增量
int modeflag=1;//赛道模式
/*-----------------------------舵机控制主程序--------------------------*/	
//函数返回值void
//输入参数void
/*------------------------------------------------------------------------*/
void duojicontrol(void) 
{
  Carlocation=SureLocationTest();                  //小车位置量
	  if(modeflag==1)
	  {  
	  duojiPcoe=60;
    duojiIcoe=0;
    duojiDcoe=0;
	  } 
	  else
	  {  

    duojiPcoe=100;
    duojiIcoe=0;
    duojiDcoe=115;
	  }      
  Piancha=PIDcalc1(duojiPcoe,duojiIcoe,duojiDcoe,Carlocation,duojiLastError,duojiPrevError,duojiSumError,duojiiError_max);
  duojizhankong=CENTRAL-Piancha;   
  if(duojizhankong<MOSTRIGHT)
    duojizhankong=MOSTRIGHT;
  if(duojizhankong>MOSTLEFT)
    duojizhankong=MOSTLEFT;
  PWMDTY01=duojizhankong;//占空比=((PWMPER-duojizhankong)/PWMPER)*100%  
}
int speedwanted(void)
{

  int speedtemp;
  if(abs(location)>=2) 
  {
    modeflag=2; 
  } 
  else
    modeflag=1; 
  if((modeflag==1)) 
  {
    speedtemp=LINE_V;
  }
  if(modeflag==2)
    speedtemp=LINE_V+5;  
  return(speedtemp);
}         
void ControlMotor(int uin)
{
 	//if(uin>0 )
 	int temp;
 	temp=uin;
 PWMDTY23 = temp;		//PTM_PTM0 = 1;//电机正转
	/*else if(uin<0 )
	{	PWMDTY23 = uin;	PTM_PTM0 = 0; }//倒车
	else
	{	PWMDTY23 = 0;	 PTM_PTM0 = 1;	} */
}
void MotorControl(void) 
{
  int bangzhi;
  int P_Term;
  int I_Term;
  int D_Term;
  speedset=20;
  speedlost[2] = speedlost[1] ;
	speedlost[1] = speedlost[0] ;
	speedlost[0] = (speedset-speed) ;
	/*if(lastStrategy)
	  bangzhi=10;
	else
	  bangzhi=PIDtoBANGBANG;                   //棒棒与pid之间的阈值 */
/*	if(speedlost[0]>10) 
	{
	  uout=UOUTMAX;
	  speedlost[2]=0;
	  speedlost[1]=0;
	  lastStrategy=1;
	  last_PIDV=0;
	}
	else if(speedlost[0]<-10) 
	{
	  uout=UOUTMIN;
	  speedlost[2]=0;
	  speedlost[1]=0; 
	  lastStrategy=1;
	  last_PIDV=0;
	}//bangbang算法
	else 
	{ */
	  motorPcoe=200;
    motorIcoe=0;
    motorDcoe=0;    
	  /*uout=PIDcalc2(motorPcoe,motorIcoe,motorDcoe,speedset,speed,motorLastError,motorPrevError,motorPrevPrevError,motoriError_max);
	  uout=last_PIDV+uout;
	  uout=PWMMOTOT-uout;
	  lastStrategy=0;*/
	  P_Term = motorPcoe * (speedlost[0] - speedlost[1]);     //比例环节;
		D_Term = motorDcoe * (speedlost[0] - 2 * speedlost[1] + speedlost[2]);  //微分环节
  	I_Term = motorIcoe * speedlost[0];   //积分环节
		if(I_Term>I_Term_max) 	//积分分离(限幅)
			I_Term = I_Term_max;
		else if(I_Term<I_Term_min)	//	积分分离(限幅)
			I_Term = I_Term_min;
		PIDmotor = P_Term + I_Term + D_Term;	
		last_PIDV = PIDmotor;
		uout = umotor + P_Term + I_Term + D_Term;		//增量累加
		if(uout > UOUTMAX)		umotor = UOUTMAX;
		else if(uout < UOUTMIN)		umotor = UOUTMIN; 
		lastStrategy = 0;	  
//	}
  if(uout>UOUTMAX)		uout = UOUTMAX;
				else if(uout<UOUTMIN)		uout = UOUTMIN;
	umotor=uout;
  uout=PWMMOTOT-uout;
	ControlMotor(uout);
}



void Control(void)
	{ 
      PWMDTY23=speedwanted(); 	//确定电机控制  \
      
      //MotorControl();
      duojicontrol();
      //PWMDTY23=LINE_V;    		
	}