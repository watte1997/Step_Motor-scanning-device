#include<reg51.h>
#define uchar unsigned char    //�궨��
#define uint unsigned int   
sbit led=P1^7;               
void Delay(uint del)    //10�����������ʱ����   
{
        uint i,j;
        for(i=0; i<del; i++)
        for(j=0; j<1827; j++);
}

void interrupt_0()interrupt 0   //�ⲿ�ж�0�������0
{
  led=~led;                 //��led��״̬ȡ��                 
  Delay(200);                //��ʱ10X10=100ms
}
//���������ԣ���һ��P3^2�İ���������led��û����һ�£�ע���ޣ��������ⲿ�ж���ʵ��led��������ޣ���ȻҲ���ð��������ǿ�ͷ��û�ж��κ�һ������KEY���ж����ޡ�
void main()
{
  EA=1;                   //�����ж�
  EX0=1;                 //���ⲿ�ж�0
  IT0=0;                  //���ⲿ�ж�0�õ�ƽ������ʽ
while(1);               //�ó����ͣ�����
}
