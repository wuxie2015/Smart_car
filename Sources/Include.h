/*----------------------------  功能 包括其它所有头文件的文件 ------------------------------*/
/*************************************************************************************************/
#ifndef _H_INCLUDE_
	#define _H_INCLUDE_

  #include "Sensor.h"
 // #include "LCD.h"
  #include "Surelocation.h"
  #include "Control.h"
  #include "base.h"
  #include "HardWare.h"
  #include "Hardwaredriver.h"
  #include "MC9S12XS128.h"
  /*---------------------以下传感器整定参数------------------------------*/
  #define FRONTNUM 8//前排传感器数4
  #define TESTTIME 5//每次采集每个传感器读取的数值数目
  #define HISTORYNUM 10//历史数据保存量
  #define MIDYUZHI 3//中间两传感器迷失后的和
  #define ZHONGJIANYUZHI 35//线在两个传感器中间和某个传感器之下时的阈值
  #define ZHONGJIAN 210//用来检测车子是否在跑到中心(赛车中心距离赛道中心八厘米) 中间两传感器的两单峰函数叠加为单峰函数 峰值250
  #define ADYUZHI 10 //AD数据校验阈值  
  #define LOCYUZHI 68 //跨区阈值
  #define COE 1
  /*---------------------以下电机整定参数------------------------------*/
  #define PWMMOTOT  150     //电机PWM周期;//电机PWM周期 0.16ms ,电机周期随便 只关心占空比。//电机PWM周期 1.6ms
  #define UOUTMAX 130	//最大正向电压
  #define UOUTMIN 75	//最大反向电压
  /*---------------------以下舵机整定参数------------------------------*/
  #define PWMDT 30000//舵机PWM周期 20ms
  #define CENTRAL 2100//舵机PWM中心占空1.53ms,转动极限duojizhankong=750
  #define MOSTLEFT 2850//舵机最左边位置 
  #define MOSTRIGHT 1350//舵机最右边位置
  /*---------------------速度控制相关宏定义------------------------------*/
  #define PIDYushe (450+speedset*7) 
  #define PIDYushe1 (450+speedset*7) //(450+speedset*7) 
  #define PIDYushe2 (550+speedset*10) //(450+speedset*7)
  #define STOPCOUNT 120		//检测到速度异常（很小）停车所需的累加数目
  #define STOPSAFTMIN	30	//启动防撞需要达到的最小速度值
  #define SPEEDMAX 25//最高速度
  #define SPEEDMIN 12//最低速度 
  /*---------------------LCD相关宏定义------------------------------*/
 //#define RS PORTA_PA5
 // #define EN PORTA_PA3
#endif