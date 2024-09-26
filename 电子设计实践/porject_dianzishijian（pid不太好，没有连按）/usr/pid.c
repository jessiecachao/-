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

//unsigned int pidControl(float Tnow, float Tset, float kp, float ki, float kd) 
//{
//    float iTerm=0;
//	  float dTerm=0;
//	  float pTerm=0;
//	  float output=0;
//	
//	// 定义PID参数
//    static float integral = 0;
//    static float prevError = 0;
//	  static float error=0;
//    error = Tset - Tnow;

//    // PID计算
//    pTerm = kp * error;
//    integral =integral + ki * error;
//    iTerm = integral;
//    dTerm = kd * (error - prevError);

//    // 更新上一次误差
//    prevError = error;

//    // 计算PID输出
//    output = pTerm + iTerm + dTerm;

//    // 限制PID输出范围在 0 到 100 之间
//    if (output > 100) 
//		{
//        output = 100;
//    }
//		else if (output < 0)
//		{
//        output = 0;
//    }

//    return (int)output;
//}


