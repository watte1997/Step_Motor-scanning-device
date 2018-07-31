#include<reg51.h>
#define uchar unsigned char    //宏定义
#define uint unsigned int   
sbit led=P1^7;               
void Delay(uint del)    //10毫秒带参量延时函数   
{
        uint i,j;
        for(i=0; i<del; i++)
        for(j=0; j<1827; j++);
}

void interrupt_0()interrupt 0   //外部中断0的序号是0
{
  led=~led;                 //将led的状态取反                 
  Delay(200);                //延时10X10=100ms
}
//现在你试试，按一下P3^2的按键，看看led有没有闪一下，注意噢，这是用外部中断来实现led灯亮灭的噢，虽然也是用按键，但是开头我没有对任何一个按键KEY进行定义噢。
void main()
{
  EA=1;                   //打开总中断
  EX0=1;                 //打开外部中断0
  IT0=0;                  //让外部中断0用电平触发方式
while(1);               //让程序就停在这儿
}
