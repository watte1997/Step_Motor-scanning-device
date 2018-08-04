#include "uart.h"

//------------------串口通信协议-----------------//
/*
    客户端数据包格式解释(长度恒为15):
    例如:A01_fmq_01Off___#
    A--------数据包的开始标记(可以为A到Z,意味着数据包可以有26种)
    01-----设备代号
    fmq_01Off___--------指令(长度恒为10)，指令的前4个人字符是指令头部，指令的后6个字符是指令尾部
    #---------数据包的结束标记

    服务器端数据包格式解释(长度恒为15):
    例如:A02_SenT010250#
    A--------数据包的开始标记(可以为A到Z,意味着数据包可以有26种)
    02-----设备代号
    SenT010250--------指令(长度恒为10)，指令的前4个字符是指令头部，指令的后6个字符是指令尾部
    #---------数据包的结束标记
*/
char buf_string[16];  //定义数据包长度为15个字符
#define deviceID_1Bit '0'                //用于串口通信时，定义本地设备ID的第1位
#define deviceID_2Bit '2'                //用于串口通信时，定义本地设备ID的第2位
#define datapackage_headflag 'A'        //用于串口通信时，定义数据包头部的验证标记

//----------------------------------------------//
/*******************************
            串口通信
    MCU:89C52RC        11.0592MHz

//11.0592MHz 0xd0 1200bps
//12MHz 0xcc 1200bps
//11.0592MHz 0xfa 9600bps
//0xf4 11.0592MHz  0xf3 12MHz 4800bps
//均在SMOD=1的情况下（波特率倍增模式）
*******************************/
//串口发送函数

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
//串口接收函数
bit ReceiveString()    
{
    char * RecStr=buf_string;
    char num=0;
    unsigned char count=0;
    loop:    
    *RecStr=SBUF;
    count=0;
    RI=0;    
    if(num<13)  //数据包长度为15个字符,尝试连续接收15个字符
    {
        num++;
        RecStr++;    
        while(!RI)
        {
            count++;
            if(count>130)return 0;    //接收数据等待延迟，等待时间太久会导致CPU运算闲置，太短会出现"数据包被分割",默认count=130
        }
        goto loop;
    }
    return 1;
}
//定时器1用作波特率发生器
void Init_USART()  
{
    SCON = 0x50;        //串口方式1
 
    TMOD = 0x20;        // 定时器使用方式2自动重载
    TH1 = 0xFD;    //9600波特率对应的预设数，定时器方式2下，TH1=TL1
    TL1 = 0xFD;
 
    TR1 = 1;//开启定时器，开始产生波特率
	EA=1;
}
//比较指令头部
bit CompareCMD_head(char CMD_head[])    
{
    unsigned char CharNum;
    for(CharNum=0;CharNum<4;CharNum++)  //指令长度为10个字符
    {
        if(!(buf_string[CharNum+4]==CMD_head[CharNum]))
        {
            return 0;  //指令头部匹配失败
        }
    }
    return 1;        //指令头部匹配成功
}
//比较指令尾部(start:从哪里开始比较，quality:比较多少个字符，CMD_tail[]：要比较的字符串)
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
bit Deal_UART_RecData()   //处理串口接收数据包函数（成功处理数据包则返回1，否则返回0）
{
     int flag;
    //PutString(buf_string);
    if(buf_string[0]==datapackage_headflag&&buf_string[13]=='#')  //进行数据包头尾标记验证		   //首字母为A  结尾为#
    {   
		PutString("指令头尾验证成功！\n");     
        if(CompareCMD_head("sudu"))    //判断指令头部是否为"sudu"   A___sudu
        {
			PutString("检测到速度调整指令\n");
            //下面是指令尾部分析
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
			PutString("速度指令参数错误\n");
				return 0;      
        }
        else if(CompareCMD_head("stop"))  	 //停止		   A___stop_____#
        {
			PutString("暂停成功！\n");
			 temp[4]=0;
        }
       else if(CompareCMD_head("go__"))  	  //开始	   A___go_______#
        {
			PutString("开始运行！\n");	
          	 temp[4]=1;
        }
        else if(CompareCMD_head("dive"))  		//改变方向	A___dive_____#
        {
            
            dir=~dir;
        }
		else
		{
			PutString("指令错误！"); 
        	return 0;
		}
    }
	else
	{
	PutString("指令头尾验证失败\n");
    return 0;
	}
}
/************************
        中断函数
************************/
//串口中断服务函数-----------
void USART() interrupt 4   //标志位TI和RI需要手动复位，TI和RI置位共用一个中断入口
{
    if(ReceiveString())    
    {
        //数据包长度正确则执行以下代码
		PutString("指令长度验证成功！\n"); 
        Deal_UART_RecData();   
    }
    else
    {
       PutString("指令长度错误！\n");               
    }
    RI=0;  //接收并处理一次数据后把接收中断标志清除一下，拒绝响应在中断接收忙的时候发来的请求
}



/***************************
        主函数
***************************/
//void main()
//{
//    EA=1;
//    Init_USART();
//    while(1)
//    {
//        //PutString(buf_string);//空格20H,回车0DH
//        
//    }
//}