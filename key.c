#include "key.h"
#include "step_motor.h"
int x=10;
int i=0;
void key_init()
{
	IT0=0;
	IT1=0;
	EX0=1;
	EX1=1;
	EA=1;
}

