#include <REG52.H>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#include <hd7279.h>
#include <global.h>
extern unsigned char temparray[10]={0xFC,0x44,0x79,0x5D,0xC5,0x9D,0xBD,0x54,0xFD,0xDD}; //不带小数点
//驱动LED和键盘
unsigned int flag = 0;

//7279初始化
void init_hd7279(void){
	send_byte(HD7279_RESET);
}

//发送字节
void send_byte(unsigned char byte){
	unsigned char i;
	cs = 0;
	long_delay();
	for(i=0;i<8;i++){
		if(byte&0x80){ //每次取最高位发送，如果最高位为1，则发送1
			dat = 1;
		}
		else{ //否则发送0
			dat = 0;
		}
		clk = 1;
		short_delay(); //生成一个下降沿
		clk = 0;
		short_delay();
		byte = byte*2; //字节左移
	}
	dat = 0;
}

//接收字节
unsigned char receive_byte(void){
	unsigned char i,byte;
	dat = 1;
	long_delay();
	for(i=0;i<8;i++){
		clk = 1;
		short_delay();
		//左移一位，空出最后一位存放进新来的位dat
		byte = byte*2; //二进制乘2，即左移
		if(dat){       //根据数据口中dat的值生成byte
			byte = byte|0x01;
		}
		clk = 0;
		short_delay();
	}
	dat = 0;
	return(byte);
}


//读键值
unsigned char readKey(void){
	unsigned char key;
	//cs = 0;
	send_byte(0x15); //读键值命令0x15
	key = receive_byte();
	//cs = 1;
	return(key);
}

//led显示
void display(){
	unsigned char i;
	//cs = 0;
	for(i=0;i<8;i++){
		write_hd7279(0x90+i,LED[i]); //0x90控制第i个led，LED[i]控制其发亮的内容
	}
	//cs = 1;
}

//7279写入
void write_hd7279(unsigned char position,unsigned char dat){
	send_byte(position);
	send_byte(dat);
}

//菜单，四个按键
void menu_switch(void){
	switch(keyvalue){ //检测键值
		case 0x3b:
		{
			menu_num++;              //功能选择，左上键
			menu_num = menu_num%5;
			break;
		}
		case 0x3a:
		{	
		   
			menu_num=(menu_num+4)%5; //功能选择，右上键
			break;
		}
		case 0x39:
		{
			layer = 1;               //返回上一层，即左下键
			break;
		}
		case 0x38:                 //进入菜单的第二层，即右下键
		{
			layer = 2;
			keyvalue = 0xff;
			break;
		}
	}
}
//进入run功能 电机测试
int run_menu_switch(int num){
	switch(keyvalue){
		case 0x3b: //调速加
		{
			num++;
			num = num%5;
			break;
		}
		case 0x3a: //调速减
		{
			num=(num+4)%5;
			break;
		}
		case 0x39: //返回上一层
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
		case 0x38: //进入下一层，电机运行
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

//进入pa功能，参数设置，分为P(档位)和A(速度)设置
int pa_menu_switch(int num){
	switch(keyvalue){
		case 0x3b:
		{
			num++;
			num = num%3;
			break;
		}
		case 0x3a:
		{
			num=(num+1)%3;
			break;
		}
		case 0x39: //返回上一层
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
					else flag = 1;
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

  
//键盘连按
int keyContinue(int keynum){
	unsigned char tempkv;
	unsigned int kc1=0,kc2=0;
	unsigned int pastep=0;   //跳变步长

	delay(10); //按键消抖
	tempkv=readKey();    //读当前键值
	if(tempkv==0xff){    //当前没有键被按下
		keyvalue=0xff;
		kc1=0;
		kc2=0;
	}
	else{                 //说明当前键（tempkv）被按下
		while((keyvalue==0x3a|keyvalue==0x3b)&&tempkv!=0xff){ //同时keyvalue的值也有，说明此时连按存在
			delay(200);
	
			if(keyvalue==0x3a){           //如果按下的键为加键
				kc1++;                      //kc1开始初始化
				if(kc1==5){ kc1=0; kc2++;}  //当连续按下时，值的跳变开始增大
				keynum=keynum+kc1+kc2*pastep;
		  }
			
			else if(keyvalue==0x3b){      //如果按下的键为减键
				kc1++;                      //kc1开始初始化
				if(kc1==5){ kc1=0; kc2++;}  //当连续按下时，值的跳变开始增大
				keynum=keynum-kc1-kc2*pastep;
			}
			
			if((keynum/100)!=0)LED[1]=temparray[keynum/100]; //显示
			else LED[1]=0x00;
			LED[2]=temparray[(keynum%100)/10];
			LED[3]=temparray[(keynum%100)%10];
			display();
			tempkv=readKey();			
		}
		keyvalue=tempkv;
	}
	return keynum;
}

		