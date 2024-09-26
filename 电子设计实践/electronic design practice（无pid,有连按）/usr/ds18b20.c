#include <REG52.H>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#include <global.h>
#include <hd7279.h>
#include <ds18b20.h>
#include <uart.h>
#include <ee24c16.h>

/*DS18B20温度读取，指令内容以及数据的写入和读取详见解读文件*/

float temp_now=0.0; //温度值带小数，使用float型

//初始化
void init_ds1820(void){
	reset_ds1820();
	//见DS28B20指令表
	writeData_ds1820(0xCC); //跳跃ROM指令
	writeData_ds1820(0x4E); //向RAM中写数据
	writeData_ds1820(0x20); 
	writeData_ds1820(0x00);
	writeData_ds1820(0x7F);
	reset_ds1820();
}

//1820复位
void reset_ds1820(void){
	unsigned char i;
	ds1820_dq = 0;      //ds1820_dq为定义的引脚P1.3引脚为VCC引脚
	for(i=240;i>0;i--); //延时，即给芯片发送一个低电平信号
	ds1820_dq = 1;
	for(i=240;i>0;i--);
}

//1820数据写入
void writeData_ds1820(unsigned char dat){
	unsigned char i,j;
	for(i=8;i>0;i--){
		ds1820_dq = 0;        //写数据前，被控制器拉低电平
		for(j=2;j>0;j--);     //拉低电平延时
		ds1820_dq = dat&0x01; //开始写入数据，每次传入最低位数据
		for(j=30;j>0;j--);    //延时
		ds1820_dq = 1;
		dat>>=1;              //数据右移，便于下一次传输
	}
}

//1820数据读取
unsigned char readData_ds1820(void){
	unsigned char i,j,s;
	for(i=8;i>0;i--){
		s>>=1;             //每次读一位（最高位），读完后进行右移
		ds1820_dq = 0; 
		for(j=2;j>0;j--);  //读数据前，先拉低电平并延时，表示读时间的开始
		ds1820_dq = 1;
		
		//开始读入数据，此时总线为高电平，则读1，低电平则读0
		for(j=4;j>0;j--);
		if(ds1820_dq==1) s|=0x80; //0x80即10000000，最高位置‘1’，其它位不变
		
		for(j=30;j>0;j--);
		ds1820_dq = 1;
	}
	return s;
}

float show_temperature(void){
	unsigned char temp_data,temp_data2,i;
	unsigned short TempDec;
	unsigned char temperature[2]; //存放读取到的温度数据
	unsigned char temp[4];        //存放温度数值
	unsigned char temparray[10]={0xFC,0x44,0x79,0x5D,0xC5,0x9D,0xBD,0x54,0xFD,0xDD}; //用于取用的LED数据显示值
	unsigned char tempdparray[10]={0xfe,0x46,0x7b,0x5f,0xC7,0x9f,0xBf,0x56,0xFf,0xDf}; //用于LED的第二位，即带小数点的数据显示值
	float T=0.0;
	reset_ds1820();
	writeData_ds1820(0xcc); //芯片指令：跳跃ROM
	writeData_ds1820(0x44); //芯片指令：温度转换
	reset_ds1820();
	writeData_ds1820(0xcc);
	writeData_ds1820(0xbe); //芯片指令：从RAM读取数据
	for(i=0;i<2;i++){       //从1820读取数据
		temperature[i]=readData_ds1820(); 
	}
	reset_ds1820();
	
	temp_data=temperature[1];
	temp_data&=0xf0; //和1111 0000做或运算，即取高四位
	
	if(temp_data==0xf0){
		if(temperature[0]==0){
			temperature[0]=~temperature[0]+1;
			temperature[1]=~temperature[1]+1;
		}
		else{
			temperature[0]=~temperature[0]+1;
			temperature[1]=~temperature[1];
		}
	}
	temp_data = temperature[1]<<4; //取第二次读取的数据作为高四位
	temp_data2 = temperature[0]>>4; //取第一次读取的数据作为低四位
	temp_data = temp_data|temp_data2; //结合成为最终的温度值
	
	//温度值转换
	temp[0]=(temp_data%100)/10;   //10
	temp[1]=(temp_data%100)%10;   //1 取出LED的第二位，即个位
	temperature[0]&=0x0f;         //取出温度值的后四位 
	//DS18B20的精度为0.0625度，*0.0625之后转换所得的温度即为实际温度
	TempDec = temperature[0]*625;
	temp[2] = TempDec/1000;       //1 取出小数点后一位
	temp[3] = (TempDec%1000)/100; //10 取出小数点后第二位
	
	T = 10*(float)temp[0]+(float)temp[1]+0.1*(float)temp[2]+0.01*(float)temp[3]; //整合温度值
	if(T==85.0)return 0;
	if(temp_now==0){
		temp_now = T;
	}
	
	if(abs(T-temp_now)<1){
		LED[0] = 0xd1; //0xd1即1101 0001 
		LED[1] = 0xb8; //即1011 1000 结合起来，即在LED中显示℃的
		LED[2] = 0x00; //LED不亮
		LED[3] = 0x00;
		LED[4] = temparray[temp[0]]; //显示温度值
		LED[5] = tempdparray[temp[1]];
		LED[6] = temparray[temp[2]];
		LED[7] = temparray[temp[3]];
		temp_now = T;
	}
	
	//if((T<40.0) || (abs(T-temp_now)<1)){}
	return temp_now;
}