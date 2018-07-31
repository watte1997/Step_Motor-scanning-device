#include "step_motor.h"

uch  fre=10;		  //周期数
uch  rest_fre=10;	   //剩余周期数
uint total_step=20000;  //单周期总步数	圈数=步数/200
uint rest_step=400;   //单周期剩余步数
uch sw=1;	     //开关状态
uch state=0;       //程序当前状态  0表停止 1表加速  2表扫描 3表减速 4表转向
uint temp[5];		  //程序状态数组
uch pp=0x02;    //断电标志位

void main()
{
	sz_init();
	lcd_init();
	Init_USART();
	key_init();
	lcd_distest();
	delay1s();
	PutString("你好 ， 湘潭大学 ！");
	lcd_init();
//	ET0 = 1;
//    /* 定时器0 */
//    TMOD = 0x01; //使用定时器0,方式1
//    TH0 = (65536 - 10) / 256; //12MHz晶振下,定时器为10ms触发中断,更方便观察转动情况
//    TL0 = (65536 - 10) % 256;
//    TR0 = 1;
		for(;rest_fre>0;rest_fre--)
		{	
			x_run();
			y_run();
			temp[1]=rest_fre-1;
		}
	while(1);
}

void sz_init()
{
	uch i;
	for(i=0;i<4;i++)
	{
		temp[0]=fre;
		temp[1]=rest_fre;
		temp[2]=total_step;
		temp[3]=rest_step;
		temp[3]=state;
	}
}
static void delay_us(uint a)
{									 
	int i,j;
	for(i=0;i<a;i++)
		for (j=0;j<50;j++);
}					 

void run_begin()
{
    int i;
	state=1;
	i=rest_step/10;   //两圈
	dis_str("Accelarating",0,0);
	PutString(" Accelarating...\n ");
	for(;i>0;i--)
	{
	en_x=1;
	if(i>10)				 //记得调节
		{
		pulse_x=~pulse_x;
		delay_us(10*i);
		pulse_x=~pulse_x;
		delay_us(10*i);
		}
	else
		{
		pulse_x=~pulse_x;
		delay_us(10*i);	  //与正常运行速度保持一致
		pulse_x=~pulse_x;
		delay_us(10*i);
		}	
	temp[3]=rest_step--;
	}
}
void run_mid()
{
    state=2;
	rest_step=total_step*4/5;	 //扫描步数赋值，占比要尽可能大
	dis_str("Scanning...",1,0);
	PutString("Scanning... \n");
	while(rest_step>0)
	{
	pulse_x=1;
	delay_us(10);
	pulse_x=0;
	delay_us(10);
	temp[3]=rest_step--;
	}  
}
uint run_end()
{
	uint i;
	i=rest_step;
    state=3;   
	dis_str("Decelarating",0,0);
	PutString("Deccelarating...\n ");
	for(;rest_step>0;rest_step--)
	{
	if(rest_step<10)				 //记得调节
		{
		pulse_x=~pulse_x;
		delay_us(15);
		pulse_x=~pulse_x;
		delay_us(15);
		}
	else
		{
		pulse_x=~pulse_x;
		delay_us(i-rest_step);	  //与正常运行速度保持一致
		pulse_x=~pulse_x;
		delay_us(i-rest_step);
		}	
		temp[3]=rest_step;
	}
		en_x=0;
}
void y_run()
{
    state=4;
	rest_step=400;
	dis_str("Changing...",0,0);
	PutString("Changing....\n");
	en_y=1;
	for(;rest_step>0;rest_step--)
	{
		pulse_y=1;
		delay_us(20);
		pulse_y=0;
		delay_us(20);
	   temp[3]=rest_step;
	}	
	lcd_init();	     //一百步
}
void x_run()
{
	run_begin();
	run_mid();
	run_end();
	delay1s();
	lcd_init();
}

void delay1s(void)

{

unsigned char h,i,j,k;

for(h=5;h>0;h--)

for(i=4;i>0;i--)

for(j=116;j>0;j--)

for(k=214;k>0;k--);

}

void INTERR(void) interrupt 0
{
	  IE0=0;//软件清零
      delay1s();//防抖动，延迟100-300ms
      
       if( 0x02==pp )
       {
	   		stop();
       }
        else
              pp=0x02;
			  lcd_init();
			  dis_str("Moving...",1,1);
              //为下次进入掉电模式做准备	   	
}

void stop()
{
	lcd_init();
	dis_str("Stoping...",1,1);
    pp=0;//标志位
    PCON=0x02;//进入掉电模式	
}
//void timer()interrupt 1
//{
//	TH0 = (65536 - 10) / 256; 
//    TL0 = (65536 - 10) % 256;
//		
//	
//}
//void INTERREPT(void) interrupt 2
//{
//
//	 else
//	 {
//	 	dis_str("Stoping",0,0);
//		while(1);
//	 }
//}