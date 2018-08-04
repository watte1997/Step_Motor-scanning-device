#include "uart.h"

//------------------����ͨ��Э��-----------------//
/*
    �ͻ������ݰ���ʽ����(���Ⱥ�Ϊ15):
    ����:A01_fmq_01Off___#
    A--------���ݰ��Ŀ�ʼ���(����ΪA��Z,��ζ�����ݰ�������26��)
    01-----�豸����
    fmq_01Off___--------ָ��(���Ⱥ�Ϊ10)��ָ���ǰ4�����ַ���ָ��ͷ����ָ��ĺ�6���ַ���ָ��β��
    #---------���ݰ��Ľ������

    �����������ݰ���ʽ����(���Ⱥ�Ϊ15):
    ����:A02_SenT010250#
    A--------���ݰ��Ŀ�ʼ���(����ΪA��Z,��ζ�����ݰ�������26��)
    02-----�豸����
    SenT010250--------ָ��(���Ⱥ�Ϊ10)��ָ���ǰ4���ַ���ָ��ͷ����ָ��ĺ�6���ַ���ָ��β��
    #---------���ݰ��Ľ������
*/
char buf_string[16];  //�������ݰ�����Ϊ15���ַ�
#define deviceID_1Bit '0'                //���ڴ���ͨ��ʱ�����屾���豸ID�ĵ�1λ
#define deviceID_2Bit '2'                //���ڴ���ͨ��ʱ�����屾���豸ID�ĵ�2λ
#define datapackage_headflag 'A'        //���ڴ���ͨ��ʱ���������ݰ�ͷ������֤���

//----------------------------------------------//
/*******************************
            ����ͨ��
    MCU:89C52RC        11.0592MHz

//11.0592MHz 0xd0 1200bps
//12MHz 0xcc 1200bps
//11.0592MHz 0xfa 9600bps
//0xf4 11.0592MHz  0xf3 12MHz 4800bps
//����SMOD=1������£������ʱ���ģʽ��
*******************************/
//���ڷ��ͺ���

void PutString(unsigned char *TXStr)  
{                
    ES=0;     
     while(*TXStr!=0) 
    {                      
        SBUF=*TXStr;
        while(TI==0);
        TI=0;    
        TXStr++;
    }
    ES=1; 
}                                                     
//���ڽ��պ���
bit ReceiveString()    
{
    char * RecStr=buf_string;
    char num=0;
    unsigned char count=0;
    loop:    
    *RecStr=SBUF;
    count=0;
    RI=0;    
    if(num<13)  //���ݰ�����Ϊ15���ַ�,������������15���ַ�
    {
        num++;
        RecStr++;    
        while(!RI)
        {
            count++;
            if(count>130)return 0;    //�������ݵȴ��ӳ٣��ȴ�ʱ��̫�ûᵼ��CPU�������ã�̫�̻����"���ݰ����ָ�",Ĭ��count=130
        }
        goto loop;
    }
    return 1;
}
//��ʱ��1���������ʷ�����
void Init_USART()  
{
    SCON = 0x50;        //���ڷ�ʽ1
 
    TMOD = 0x20;        // ��ʱ��ʹ�÷�ʽ2�Զ�����
    TH1 = 0xFD;    //9600�����ʶ�Ӧ��Ԥ��������ʱ����ʽ2�£�TH1=TL1
    TL1 = 0xFD;
 
    TR1 = 1;//������ʱ������ʼ����������
	EA=1;
}
//�Ƚ�ָ��ͷ��
bit CompareCMD_head(char CMD_head[])    
{
    unsigned char CharNum;
    for(CharNum=0;CharNum<4;CharNum++)  //ָ���Ϊ10���ַ�
    {
        if(!(buf_string[CharNum+4]==CMD_head[CharNum]))
        {
            return 0;  //ָ��ͷ��ƥ��ʧ��
        }
    }
    return 1;        //ָ��ͷ��ƥ��ɹ�
}
//�Ƚ�ָ��β��(start:�����￪ʼ�Ƚϣ�quality:�Ƚ϶��ٸ��ַ���CMD_tail[]��Ҫ�Ƚϵ��ַ���)
bit CompareCMD_tail(unsigned char start,unsigned char quality,char CMD_tail[]) 
{
    unsigned char CharNum;
    for(CharNum=0;CharNum<quality;CharNum++)
    {
        if(!(buf_string[start+CharNum]==CMD_tail[CharNum]))
        {
            return 0; 
        }
    }
    return 1;
}
bit Deal_UART_RecData()   //�����ڽ������ݰ��������ɹ��������ݰ��򷵻�1�����򷵻�0��
{
     int flag;
    //PutString(buf_string);
    if(buf_string[0]==datapackage_headflag&&buf_string[13]=='#')  //�������ݰ�ͷβ�����֤		   //����ĸΪA  ��βΪ#
    {   
		PutString("ָ��ͷβ��֤�ɹ���\n");     
        if(CompareCMD_head("sudu"))    //�ж�ָ��ͷ���Ƿ�Ϊ"sudu"   A___sudu
        {
			PutString("��⵽�ٶȵ���ָ��\n");
            //������ָ��β������
            if(CompareCMD_tail(9,3,"001"))   //	 A___sudu_001_#
                {
					temp[5]=100;
                    return 1;
                }
            else if(CompareCMD_tail(9,3,"002"))	 //	  A___sudu_002_#
                {
                    temp[5]=75;
                    return 1;
                }
			else if(CompareCMD_tail(9,3,"003"))	 //	  A___sudu_003_#
                {
                    temp[5]=50;
                   return 1;
                }  
			else if(CompareCMD_tail(9,3,"004"))	 // A___sudu_004_#
                {
                   temp[5]=25;
                   return 1;  
				} 
			else
			PutString("�ٶ�ָ���������\n");
				return 0;      
        }
        else if(CompareCMD_head("stop"))  	 //ֹͣ		   A___stop_____#
        {
			PutString("��ͣ�ɹ���\n");
			 temp[4]=0;
        }
       else if(CompareCMD_head("go__"))  	  //��ʼ	   A___go_______#
        {
			PutString("��ʼ���У�\n");	
          	 temp[4]=1;
        }
        else if(CompareCMD_head("dive"))  		//�ı䷽��	A___dive_____#
        {
            
            dir=~dir;
        }
		else
		{
			PutString("ָ�����"); 
        	return 0;
		}
    }
	else
	{
	PutString("ָ��ͷβ��֤ʧ��\n");
    return 0;
	}
}
/************************
        �жϺ���
************************/
//�����жϷ�����-----------
void USART() interrupt 4   //��־λTI��RI��Ҫ�ֶ���λ��TI��RI��λ����һ���ж����
{
    if(ReceiveString())    
    {
        //���ݰ�������ȷ��ִ�����´���
		PutString("ָ�����֤�ɹ���\n"); 
        Deal_UART_RecData();   
    }
    else
    {
       PutString("ָ��ȴ���\n");               
    }
    RI=0;  //���ղ�����һ�����ݺ�ѽ����жϱ�־���һ�£��ܾ���Ӧ���жϽ���æ��ʱ����������
}



/***************************
        ������
***************************/
//void main()
//{
//    EA=1;
//    Init_USART();
//    while(1)
//    {
//        //PutString(buf_string);//�ո�20H,�س�0DH
//        
//    }
//}