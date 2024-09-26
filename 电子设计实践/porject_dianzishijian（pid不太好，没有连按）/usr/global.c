#include <REG52.H>
#include <stdio.h>
#include <math.h>
#include <intrins.h>
#include <global.h>

unsigned char temparray[10]={0xFC,0x44,0x79,0x5D,0xC5,0x9D,0xBD,0x54,0xFD,0xDD};
unsigned char tempdparray[10]={0xfe,0x46,0x7b,0x5f,0xC7,0x9f,0xBf,0x56,0xFf,0xDf};
 
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
    //Œ¢√Îº∂—” ± —” ±zŒ¢√Î
	unsigned int x;
	for(x = 0;x<z;x++){
	  _nop_();
	}
}
 
 