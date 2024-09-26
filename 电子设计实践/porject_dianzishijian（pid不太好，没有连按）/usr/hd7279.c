#include <REG52.H>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#include <hd7279.h>
#include <global.h>
#include <ee24c16.h>
 
unsigned int flag = 0;
float Tset = 28.0;
 
void init_hd7279(void){
	send_byte(HD7279_RESET);  //初始化hd7279芯片
}
 
void send_byte(unsigned char byte){
	unsigned char i;
	cs = 0;
	long_delay();
	for(i=0;i<8;i++){
		if(byte&0x80){
			dat = 1;
		}
		else{
			dat = 0;
		}
		clk = 1;
		short_delay();
		clk = 0;
		short_delay();
		byte = byte*2;
	}
	dat = 0;
}
 
unsigned char receive_byte(void){
	unsigned char i,byte;
	dat = 1;
	long_delay();
	for(i=0;i<8;i++){
		clk = 1;
		short_delay();
		byte = byte*2;
		if(dat){
			byte = byte|0x01;
		}
		clk = 0;
		short_delay();
	}
	dat = 0;
	return(byte);
}
 
unsigned char readKey(void){
	unsigned char key;
	//cs = 0;
	send_byte(0x15);
	key = receive_byte();
	//cs = 1;
	return(key);
}
 
void display(){
	unsigned char i;
	//cs = 0;
	for(i=0;i<8;i++){
		write_hd7279(0x90+i,LED[i]);
	}
	//cs = 1;
}
 
void write_hd7279(unsigned char position,unsigned char dat){
	send_byte(position);
	send_byte(dat);
}
 
//菜单界面控制
void menu_switch(void){
	switch(keyvalue){
		case 0x3a:       //右上，加   
		{
			menu_num++;
			menu_num = menu_num%5;
			break;
		}
		case 0x3b:   //左上，减
		{
			menu_num=(menu_num+4)%5;
			break;
		}
		case 0x39:   //左下，返回
		{
			layer = 1;
			break;
		}
		case 0x38:   //右下，确认
		{
			layer = 2;
			keyvalue = 0xff;
			break;
		}
	}
}
 
//电机测试界面
int run_menu_switch(int num){
	switch(keyvalue){
		case 0x3a:
		{
			num++;num = num%5;
			//num = num%10;
			break;
		}
		case 0x3b:
		{
			num=(num+4)%5;
			//num=(num+9)%10;
			break;
		}
		case 0x39:
		{
			if(num!=-1){
				layer--;
				keyvalue = 0xff;
				run_num = num;
				num = -1;
			}
			else num = 0;
			break;
		}
		case 0x38:
		{
			if(num!=-1){
				layer++;
				keyvalue = 0xff;
				run_num = num;
				num = -1;
			}
			else num = 0;
			break;
		}
	}
	return num;
}
 
//参数界面控制                                           
int pa_menu_switch(int num){
	switch(keyvalue){
		case 0x3a:
		{
			num++;
			if(layer==3)
				num=num%2;
			else if(layer==2)
			  num = num%3;
			break;
		}
		case 0x3b:
		{
			if(layer==3)
				num=(num+1)%2;
			else if(layer==2)
			  num=(num+1)%3;			
			break;
		}
		case 0x39:
		{
			if(num!=-1){
				layer--;
				keyvalue = 0xff;
				con_num = num;
				num = -1;
			}
			else num = 0;
			break;
		}
		case 0x38:
		{
			if(num!=-1){
				if(layer==2){
					if(num==0)flag=0;
					else if(num==1) flag = 1;                       //................................................................
					else flag=2;
				}
				layer++;
				keyvalue = 0xff;
				con_num = num;
				num = -1;
			}
			else num = 0;
			break;
		}
	}
	return num;
}

