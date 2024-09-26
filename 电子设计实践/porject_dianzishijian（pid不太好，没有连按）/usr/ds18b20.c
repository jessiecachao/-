#include <REG52.H>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#include <global.h>
#include <hd7279.h>
#include <ds18b20.h>
#include <ee24c16.h>
 
float temp_now=0.0;
 
void init_ds1820(void){
	reset_ds1820();
	writeData_ds1820(0xCC);
	writeData_ds1820(0x4E);
	writeData_ds1820(0x20);
	writeData_ds1820(0x00);
	writeData_ds1820(0x7F);
	reset_ds1820();
}
 
void reset_ds1820(void){
	unsigned char i;
	ds1820_dq = 0;
	for(i=240;i>0;i--);
	ds1820_dq = 1;
	for(i=240;i>0;i--);
}
 
void writeData_ds1820(unsigned char dat){
	unsigned char i,j;
	for(i=8;i>0;i--)
	{
		ds1820_dq = 0;  //拉低总线
		for(j=2;j>0;j--);
		ds1820_dq = dat&0x01;   //发送1位
		for(j=30;j>0;j--);
		ds1820_dq = 1;    //释放总线
		dat>>=1;   //准备传输下一位数据
	}
}
 
unsigned char readData_ds1820(void){
	unsigned char i,j,s;
	for(i=8;i>0;i--)
	{
		s>>=1;
		ds1820_dq = 0;
		for(j=2;j>0;j--);
		ds1820_dq = 1;
		for(j=4;j>0;j--);
		if(ds1820_dq==1) s|=0x80;
		for(j=30;j>0;j--);
		ds1820_dq = 1;
	}
	return s;
}
 
float show_temperature(void){
	unsigned char temp_data,temp_data2,i;
	unsigned short TempDec;
	unsigned char temperature[2];
	unsigned char temp[4];
	float T=0.0;
	reset_ds1820();
	writeData_ds1820(0xcc);   //跳过ROM指令
	writeData_ds1820(0x44);   //温度转换指令
	reset_ds1820();
	writeData_ds1820(0xcc);   //跳过ROM指令
	writeData_ds1820(0xbe);   //读DS1820温度暂存器命令
	for(i=0;i<2;i++){
		temperature[i]=readData_ds1820();  //采集温度，先读低字节，再读高字节
	}
	reset_ds1820();
	
	temp_data=temperature[1];
	temp_data&=0xf0;    //取高 4 位
	if(temp_data==0xf0)//判断是正温度还是负温度读数
	{   //负温度读数求补,取反加 1,判断低 8 位是否有进位
		if(temperature[0]==0)
		{//有进位,高 8 位取反加 1
			temperature[0]=~temperature[0]+1;
			temperature[1]=~temperature[1]+1;
		}
		else
		{//没进位,高 8 位不加 1
			temperature[0]=~temperature[0]+1;
			temperature[1]=~temperature[1];
		}
	}
	temp_data = temperature[1]<<4;
	temp_data2 = temperature[0]>>4;
	temp_data = temp_data|temp_data2;
	temp[0]=(temp_data%100)/10;//10
	temp[1]=(temp_data%100)%10;//1
	temperature[0]&=0x0f;
	TempDec = temperature[0]*625; //625=0.0625*10000，表示小数部分,扩大 1 万倍 ，方便显示
	temp[2] = TempDec/1000;//1
	temp[3] = (TempDec%1000)/100;//10
	T = 10*(float)temp[0]+(float)temp[1]+0.1*(float)temp[2]+0.01*(float)temp[3];
	if(T==85.0)return 0;
	if(temp_now==0){
		temp_now = T;
	}
	if(abs(T-temp_now)<1){
		LED[0] = 0xd1;
		LED[1] = 0xb8;
		LED[2] = 0x00;
		LED[3] = 0x00;
		LED[4] = temparray[temp[0]];
		LED[5] = tempdparray[temp[1]];
		LED[6] = temparray[temp[2]];
		LED[7] = temparray[temp[3]];
		temp_now = T;
	}
	
	//if((T<40.0) || (abs(T-temp_now)<1)){}
	return temp_now;
}