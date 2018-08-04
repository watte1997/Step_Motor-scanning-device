#include "step_motor.h"

uch  fre=15;		  //������
uch  rest_fre=15;	   //ʣ��������
uint total_step=7300;  //�������ܲ���	Ȧ��=����/200
uint rest_step=7300;   //������ʣ�ಽ��
uch sw=1;	     //����״̬
uch state=0;       //����ǰ״̬  0��ֹͣ 1��ʼ
uch rate=30;
uint temp[6];		  //����״̬����
uch pp=0x02;    //�ϵ��־λ

void main()
{
	sz_init();
	lcd_init();
	Init_USART();
	key_init();
	lcd_distest();
	delay1s();
	PutString("��� �� ��̶��ѧ ��\n");
	lcd_init();
	dir=1;
		for(;rest_fre>0;rest_fre--)
		{	
			x_run();
			y_run();
			temp[1]=rest_fre-1;
			delay1s();
			dir=~dir;
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
		temp[4]=state;
		temp[5]=rate;
	}
}
static void delay_us(uint a)
{									 
	int i,j;
	for(i=0;i<a;i++)
		for (j=0;j<2;j++);
}					 

void run_begin()
{
    int i=100;		  // ������Ȧ
	dis_str("Accelarating",0,0);
	PutString("Accelarating...\n ");
	for(;i>0;i--)
	{
		if(i<temp[5])
		{

		while(temp[4]==0) ;
		pulse_x=~pulse_x;
		delay_us(temp[5]);
		pulse_x=~pulse_x;
		delay_us(temp[5]);
		temp[3]=rest_step--;
		}
		else
		{
		while(temp[4]==0);
		pulse_x=~pulse_x;
		delay_us(i);
		pulse_x=~pulse_x;
		delay_us(i);
		temp[3]=rest_step--;
		}								   //�����ٶȲ����������

	}
}

void run_mid()
{
	rest_step=64 delay1s(void)00;	 //ɨ�貽����ֵ��ռ��Ҫ�����ܴ�
	dis_str("Scanning...",1,0);
	PutString("Scanning..\n");
	while(rest_step>0)
	{
	while(temp[4]==0);
	pulse_x=1;
	delay_us(temp[5]);
	pulse_x=0;
	delay_us(temp[5]);
	temp[3]=rest_step--;
	}  
}
void run_end()
{
	uint i=temp[5];
	dis_str("Decelarating",2,0);
	PutString("Decelarating...\n ");
	for(i=0;i<300;i++)
	{
	if(i>temp[5])				 //�ǵõ���
		{
		while(temp[4]==0);
		pulse_x=~pulse_x;
		delay_us(i);
		pulse_x=~pulse_x;
		delay_us(i);
		}
	else
		{
		while(temp[4]==0);
		pulse_x=~pulse_x;
		delay_us(temp[5]);	  //�����������ٶȱ���һ��
		pulse_x=~pulse_x;
		delay_us(temp[5]);
		}	
		temp[3]=rest_step;
	}
}
void y_run()
{
	rest_step=790;
	dis_str("Changing...",3,0);
	PutString("Changing...\n");
	for(;rest_step>0;rest_step--)
	{
		while(temp[4]==0);
		pulse_y=1;
		delay_us(10);
		pulse_y=0;
		delay_us(10);
	   temp[3]=rest_step;
	}	
	lcd_init();	     //һ�ٲ�
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
	  IE0=0;//�������
      delay1s();//���������ӳ�100-300ms
      
       if( pp==0x02 )
       {
	   		stop();
       }
        else
              pp=0x02;
			  lcd_init();
			  dis_str("Moving...",1,1);
              //Ϊ�´ν������ģʽ��׼��	   	
}

void stop()
{
	lcd_init();
	dis_str("Stoping...",1,1);
    pp=0;//��־λ
    PCON=0x02;//�������ģʽ	
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