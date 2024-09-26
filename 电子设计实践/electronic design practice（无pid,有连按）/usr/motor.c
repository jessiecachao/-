#include <REG52.H>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#include <hd7279.h>
#include <global.h>
#include <motor.h>
#include <uart.h>
 
void init_motor(void){
	TMOD = 0x01;
	TH0 = 0xee;
	TL0 = 0x00;
	//EA = 1;
	ET0 = 1; 
	TR0 = 1;
	motor = 0;
	P2 = 0x02;
}