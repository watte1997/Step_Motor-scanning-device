#ifndef __LCD12864_H
#define __LCD12864_H

#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int

void beep();
void  dataconv();
void lcd_pos(uchar X,uchar Y);  //确定显示位置
void delay(int ms);
bit lcd_busy();
void lcd_wcmd(uchar cmd);
void lcd_wdat(uchar dat);
void lcd_init();
void lcd_pos(uchar X,uchar Y);
void lcd_distest();
void dis_str(uchar *buff,uint x,uint y);

#endif