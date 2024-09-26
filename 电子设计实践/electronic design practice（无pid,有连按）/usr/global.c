#include <REG52.H>
#include <stdio.h>
#include <math.h>
#include <intrins.h>
#include <global.h>
 //延时子函数
void long_delay(void){
  unsigned char d;
	for(d=0;d<0x30;d++);
}
 
void short_delay(void){
	unsigned char d;
	for(d=0;d<8;d++);
}
 
void delay(unsigned int z){
	//ms
 	unsigned int x,y;
 	for(x=z;x>0;x--) for(y=110;y>0;y--);
}
 
void delay_us(unsigned int z){
    //微秒级延时 延时z微秒
	unsigned int x;
	for(x = 0;x<z;x++){
	  _nop_();
	}
}
 
 