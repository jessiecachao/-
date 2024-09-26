#include <REG52.H>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#include <hd7279.h>
#include <global.h>
#include <ee24c16.h>
#include <ds18b20.h>


//初始化
void init_24c16(void){
	sda=1; //串行数据/地址
	scl=1; //串行时钟
}


void I2C_start(void){
	//生成一个下降沿
	sda=1;
	delay_us(5);
	scl=1;
	delay_us(5);
	sda=0;
	delay_us(5);
}

void I2C_stop(void){
	//生成一个上升沿
	sda=0;
	delay_us(5);
	scl=1;
	delay_us(5);
	sda=1;
	delay_us(5);
}

void I2C_ack(void){
	//产生应答信号
	unsigned char i;
	scl=1;
	delay_us(5);
	while((sda==1)&&(i<200))i++;
	scl=0;
	delay_us(5);
}

void I2C_noack(){
	sda = 1;
	delay_us(5);
	scl = 1;
	delay_us(5);
	scl = 0;
	delay_us(5);
}
void writebyte_I2C(unsigned char input){
	unsigned char i;
	scl = 0;
	for(i=0;i<8;i++){
		if(input&0x80) sda = 1; //从高位开始传输数据
		else sda = 0;
		input = input<<1; //左移便于继续传输数据
		delay_us(5);
		scl = 1;
		delay_us(5);
		scl = 0;
		delay_us(5);
	}
	sda = 1;
	delay_us(5);
}

unsigned char readbyte_I2C(void){
	unsigned char i,rbyte;
	scl = 0;
	delay_us(5);
	sda = 1;
	delay_us(5);
	for(i=0;i<8;i++){
		scl = 1;
		delay_us(5);
		rbyte = rbyte<<1;
		if(sda)rbyte++;
		scl = 0;
		delay_us(5);
	}
	return rbyte;
}

void writebyte_24c16(unsigned char dat,unsigned char addr){
	init_24c16();
	I2C_start();
	writebyte_I2C(WriteDeviceAddress);
	I2C_ack();
	writebyte_I2C(addr);
	I2C_ack();
	writebyte_I2C(dat);
	I2C_ack();
	I2C_stop();
}

unsigned char readbyte_24c16(unsigned char addr){
	unsigned char output;
	init_24c16();
	I2C_start();
	writebyte_I2C(WriteDeviceAddress);
	I2C_ack();
	writebyte_I2C(addr);
	I2C_ack();
	I2C_start();
	writebyte_I2C(ReadDeviceAddress);
	I2C_ack();
	output = readbyte_I2C();
	I2C_noack();
	I2C_stop();
	return output;
}

void save_all_data(void){
	//只调用一次
	delay_us(150);
	writebyte_24c16(10,0);
	delay_us(150);
	writebyte_24c16(20,1);
	delay_us(150);
	writebyte_24c16(30,2);
	delay_us(150);
	writebyte_24c16(40,3);
	delay_us(150);
	writebyte_24c16(50,4);
	delay_us(150);
	writebyte_24c16(60,5);
	delay_us(150);
	writebyte_24c16(70,6);
	delay_us(150);
	writebyte_24c16(80,7);
	delay_us(150);
	writebyte_24c16(90,8);
	delay_us(150);
	writebyte_24c16(100,9);
	delay_us(150);
	writebyte_24c16(23,10);//t_down
	delay_us(150);
	writebyte_24c16(32,11);//t_up
	delay_us(150);
}