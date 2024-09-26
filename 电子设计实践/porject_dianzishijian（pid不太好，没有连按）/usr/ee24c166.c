#include <REG52.H>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#include <hd7279.h>
#include <global.h>
#include <ee24c16.h>
#include <ds18b20.h>
 
void init_24c16(void){
	sda=1;
	scl=1;
}
 
 
void I2C_start(void){
	
	sda=1;
	delay_us(5);
	scl=1;
	delay_us(5);
	sda=0;
	delay_us(5);
}
 
void I2C_stop(void){
	sda=0;
	delay_us(5);
	scl=1;
	delay_us(5);
	sda=1;
	delay_us(5);
}
 
void I2C_ack(void){
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
		if(input&0x80)sda = 1;
		else sda = 0;
		input = input<<1;
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
	//??μ÷ó?ò?′?
	delay_us(150);
	writebyte_24c16(20,0);
	delay_us(150);
	writebyte_24c16(40,1);
	delay_us(150);
	writebyte_24c16(60,2);
	delay_us(150);
	writebyte_24c16(80,3);
	delay_us(150);
	writebyte_24c16(100,4);
	delay_us(150);
//	writebyte_24c16(60,5);
//	delay_us(150);
//	writebyte_24c16(70,6);
//	delay_us(150);
//	writebyte_24c16(80,7);
//	delay_us(150);
//	writebyte_24c16(90,8);
//	delay_us(150);
//	writebyte_24c16(100,9);
//	delay_us(150);
	writebyte_24c16(23,10);//t_down
	delay_us(150);
	writebyte_24c16(32,11);//t_up
	delay_us(150);
	writebyte_24c16((unsigned char)Tset,12); //设置的恒温参数
	delay_us(150);
}