/*************************************************************************************************/
/*--------------------------------  文件名 ： HardWare.c  -----------------------------------*/
/*----------------------------------  功能 硬件初始化 ----------------------------------------*/
/*************************************************************************************************/
#include "Include.h"
int PWMMOTOTA;              //电机占空比
extern int LINE_V;
/*----------------------------------  系统时钟初始化 ----------------------------------------*/
void CLK_init(void) 
{
  CLKSEL=0x00; //初始时还没有PLL
  PLLCTL=0xe1; //时钟监控使能，PLL允许，VCOCLK调频2%，禁止快速苏醒，禁止自给时钟，伪停止下禁止实时中断和看门狗
  SYNR=2;
  REFDV=1;           //PLLCLK=2*OSCCLK*(SYNR+1/((REFDV+1))=48M;
  asm NOP;asm NOP;asm NOP;//等待锁相环稳定
  while(!(CRGFLG&0x08));//锁相环稳定标志位
  CLKSEL=0x80;  //时钟选择寄存器，Bus Clock=PLLCLK/2,停止模式下振荡器、系统时钟、锁相环、内核时钟、看门狗继续工作
    
}
/*----------------------------------  ECT初始化 ----------------------------------------*/
void ECT_init(void) 
{
  TIOS=0x0f;   //通道0~4输出比较，5~7输入捕捉
  TSCR2=0x07;  //TOF被置位时禁止中断，通道7输出后不复位，f=24/128=0.1875MHz
  TCTL3=0x55;  //通道IC7~4捕捉上升沿；（TCTL2=0x00,OC与引脚断开)
  PACTL=0x40;  //脉冲累加器A使能，当PT7检测到上升沿后计数器加1，用分频因子定义时钟，禁止累加器溢出中断
  TIE=0x01;    //OC通道各位引起中断，自由计时器溢出不引起中断
  PACNT=0x00;
  TCNT=0x00;
  TSCR1=0x80;  //定时器允许，WAIT模式继续工作，FREEZE模式继续工作，定时器标志不快速清除
}
/*----------------------------------  IO口初始化 ----------------------------------------*/
void IO_init(void)
{
  DDRA=0xff;
  PORTA=0xff;
  DDRA=0x00;      //PORTA做输入口
  //PUCR_PUPAE=1;   //A口上拉电阻
  DDRB=0xff;      //PORTB做LED灯
  PORTB=0x00; 
  //DDRT=0xff;    
  //PTT=0x00;       //初始拉低
  DDRT=0xff;      //T口输出
  PERT=0xff;      //T口加pull device 防抖
  PPST=0xff;      //T口下拉电阻
  PTT_PTT0=0;
  DDRP=0xff;      //PORTP做PWM输出
  PTP=0xff;
  DDRM_DDRM0=1;   //PTM0做输出口，1=正开 0=倒车
  PTM_PTM0=0;
  DDRJ=0xff;
  PTJ=0xff;     //PTJ0初始值为高
  PERJ=0xff;
  DDRJ=0x00;      //PTJ0做输入口，接受干簧管中断
  PPSJ=0x00;      //PTJ7捕捉下降沿中断
  PIEJ=0X80;      //中断使能
  //RDR0AD0=0xff;//AD口即使传感器不工作时也有值 认为是电磁干扰 加下拉电阻
  RDRP=0xff;
  
  //PUCR=0x40;    //BKGD E K A B都是用上拉电阻
  //PUCR 对于输入 拉高 RDRIV对于输出 拉低
}
/*----------------------------------  PWM初始化 ----------------------------------------*/
void PWM_init(void)
{
  PWME=0x00;        //初始禁止PWM
  PWMPRCLK=0x44;    //预分频BUSCLOK/16=1.5MHz
  PWMCLK=0x00;     //0、1使用CLOCKA  2、3使用CLOCKB
  PWMCTL=0x30;      //通道01  23级联
  PWMPOL=0xff;      //初始低电平
  PWMCAE=0x00;      //波形左对齐
  PWMPER01=PWMDT;   //舵机周期
  PWMDTY01=CENTRAL; //舵机中心占空比
  PWMPER23=PWMMOTOT;//电机周期
  PWMDTY23=70;//电机初始中心占空比
  PWME=0xff;         //延时之后启动电机和舵机
}
/*----------------------------------  AD模块初始化 ----------------------------------------*/
void AD_init(void)
{
   ATD0CTL0=0x07;     //AD转换序列长度为8
   ATD0CTL1=0x00;     //允许指定AD口做外部触发输入口，12位结果，采样前采样电容不放电 1
   ATD0CTL2=0x10;     //标志寄存器不快速清零，停止模式不运行，禁止外部触发，禁止转换完成中断和比较成功中断
   ATD0CTL3=0xc0;     //结果右对齐，非FIFO模式，冻结模式继续转换 转换长度4
   ATD0CTL4=0x44;     //第二阶段采用8个AD周期（第一阶段2个AD周期），f=fbus/2*(4+1)，4.16ms转换一次
   ATD0CTL5=0x30;     //连续多通道转换，从通道0开始
   ATD0DIEN=0x00;     //禁用数字输入缓冲?
}     

/*----------------------------------  获取AD转换初始数据 ----------------------------------------*/
unsigned char GetATD0(unsigned char ch)
{
  int ad=0;
  //while(!ATD0STAT0_SCF);
  switch(ch) 
  { 
    case 0:
    ad=((ATD0DR0H*256)+ATD0DR0L);
    break;
    case 1:
    ad=((ATD0DR1H*256)+ATD0DR1L);
    break;
    case 2:
    ad=((ATD0DR2H*256)+ATD0DR2L);
    break;
    case 3:
    ad=((ATD0DR3H*256)+ATD0DR3L);
    break;
    case 4:
    ad=((ATD0DR4H*256)+ATD0DR4L);
    break;
    case 5:
    ad=((ATD0DR5H*256)+ATD0DR5L);
    break;
    case 6:
    ad=((ATD0DR6H*256)+ATD0DR6L);
    break;
    case 7:
    ad=((ATD0DR7H*256)+ATD0DR7L);
    break;       
  }
  return ad;
}
/*----------------------------------  总初始化--------------------------------------------*/
void init(void)
{
    CLK_init();
    
    ECT_init();
    IO_init();       
    PWM_init();
    AD_init();    
}