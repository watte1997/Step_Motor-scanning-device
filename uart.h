#ifndef __UART_H
#define __UART_H

#include <reg52.h>
#include <intrins.h>
#include "step_motor.h"

typedef   unsigned char  uint8;
typedef   unsigned int   uint16;
 
void PutString(unsigned char *TXStr);
bit ReceiveString();
void Init_USART();
bit CompareCMD_head(char CMD_head[]);
bit CompareCMD_tail(unsigned char start,unsigned char quality,char CMD_tail[]); 
bit Deal_UART_RecData() ;  //处理串口接收数据包函数（成功处理数据包则返回1，否则返回0）
#endif