#include <REG52.H>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#include <global.h>
#include <ds18b20.h>
#include <pid.h>
 
float e_bebefore=0.0,e_before=2.0,e=0.0;
 
unsigned int pid_getpwm(int ui,float Tnow,float Tset,float kp,float ki,float kd){
	//ui = 50,Tset = 28.0,kp=4.0,ki=0.33,kd=4.0
	//ki=T/Ti,kd=Td/T
	//kp=10,ki=0.2,kd=0
	float A,B,C;
	int u_delta;
	A = kp*(1+ki+kd);
	B = -kp*(1+2*kd);
	C = kp*kd;
	e_bebefore = e_before;
	e_before = e;
	e = Tnow-Tset;
	//u_delta = 4*e-16*e_before;
	u_delta = A*e+B*e_before+C*e_bebefore;
	ui = ui+u_delta;
	
	if(ui>100) ui=100;
	return ui;
}