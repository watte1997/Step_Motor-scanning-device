#include "step_motor.h"

uch  fre=10;		  //������
uch  rest_fre=10;	   //ʣ��������
uint total_step=20000;  //�������ܲ���	Ȧ��=����/200
uint rest_step=400;   //������ʣ�ಽ��
uch sw=1;	     //����״̬
uch state=0;       //����ǰ״̬  0��ֹͣ 1�����  2��ɨ�� 3����� 4��ת��
uint temp[5];		  //����״̬����
uch pp=0x02;    //�ϵ��־λ

void main()
{
	sz_init();
	lcd_init();
	Init_USART();
	key_init();
	lcd_distest();
	delay1s();
	PutString("��� �� ��̶��ѧ ��");
	lcd_init();
//	ET0 = 1;
//    /* ��ʱ��0 */
//    TMOD = 0x01; //ʹ�ö�ʱ��0,��ʽ1
//    TH0 = (65536 - 10) / 256; //12MHz������,��ʱ��Ϊ10ms�����ж�,������۲�ת�����
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
	i=rest_step/10;   //��Ȧ
	dis_str("Accelarating",0,0);
	PutString(" Accelarating...\n ");
	for(;i>0;i--)
	{
	en_x=1;
	if(i>10)				 //�ǵõ���
		{
		pulse_x=~pulse_x;
		delay_us(10*i);
		pulse_x=~pulse_x;
		delay_us(10*i);
		}
	else
		{
		pulse_x=~pulse_x;
		delay_us(10*i);	  //�����������ٶȱ���һ��
		pulse_x=~pulse_x;
		delay_us(10*i);
		}	
	temp[3]=rest_step--;
	}
}
void run_mid()
{
    state=2;
	rest_step=total_step*4/5;	 //ɨ�貽����ֵ��ռ��Ҫ�����ܴ�
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
	if(rest_step<10)				 //�ǵõ���
		{
		pulse_x=~pulse_x;
		delay_us(15);
		pulse_x=~pulse_x;
		delay_us(15);
		}
	else
		{
		pulse_x=~pulse_x;
		delay_us(i-rest_step);	  //�����������ٶȱ���һ��
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
      
       if( 0x02==pp )
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