#ifndef _STEP_MOTOR_H_
#define	_STEP_MOTOR_H_


#include <stdio.h>
#include <reg52.h>
#include <intrins.h>
#include "lcd12864.h"
#include "uart.h"
#include "key.h"

#define uch   unsigned char
#define uint  unsigned int
#define ulong  unsigned  long

sbit pulse_x=P1^0;
sbit pulse_y=P1^1;
sbit dir=P1^2;
sbit en_x=P1^3;
sbit en_y=P1^3;




uint run_begin();
static void delay_us(uint a);
uint run_mid();
uint run_end();
uint y_run();
void x_run();
void delay1s(void);
void sz_init();
void stop();

#endif