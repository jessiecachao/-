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

/*DS18B20�¶ȶ�ȡ��ָ�������Լ����ݵ�д��Ͷ�ȡ�������ļ�*/

float temp_now=0.0; //�¶�ֵ��С����ʹ��float��

//��ʼ��
void init_ds1820(void){
	reset_ds1820();
	//��DS28B20ָ���
	writeData_ds1820(0xCC); //��ԾROMָ��
	writeData_ds1820(0x4E); //��RAM��д����
	writeData_ds1820(0x20); 
	writeData_ds1820(0x00);
	writeData_ds1820(0x7F);
	reset_ds1820();
}

//1820��λ
void reset_ds1820(void){
	unsigned char i;
	ds1820_dq = 0;      //ds1820_dqΪ���������P1.3����ΪVCC����
	for(i=240;i>0;i--); //��ʱ������оƬ����һ���͵�ƽ�ź�
	ds1820_dq = 1;
	for(i=240;i>0;i--);
}

//1820����д��
void writeData_ds1820(unsigned char dat){
	unsigned char i,j;
	for(i=8;i>0;i--){
		ds1820_dq = 0;        //д����ǰ�������������͵�ƽ
		for(j=2;j>0;j--);     //���͵�ƽ��ʱ
		ds1820_dq = dat&0x01; //��ʼд�����ݣ�ÿ�δ������λ����
		for(j=30;j>0;j--);    //��ʱ
		ds1820_dq = 1;
		dat>>=1;              //�������ƣ�������һ�δ���
	}
}

//1820���ݶ�ȡ
unsigned char readData_ds1820(void){
	unsigned char i,j,s;
	for(i=8;i>0;i--){
		s>>=1;             //ÿ�ζ�һλ�����λ����������������
		ds1820_dq = 0; 
		for(j=2;j>0;j--);  //������ǰ�������͵�ƽ����ʱ����ʾ��ʱ��Ŀ�ʼ
		ds1820_dq = 1;
		
		//��ʼ�������ݣ���ʱ����Ϊ�ߵ�ƽ�����1���͵�ƽ���0
		for(j=4;j>0;j--);
		if(ds1820_dq==1) s|=0x80; //0x80��10000000�����λ�á�1��������λ����
		
		for(j=30;j>0;j--);
		ds1820_dq = 1;
	}
	return s;
}

float show_temperature(void){
	unsigned char temp_data,temp_data2,i;
	unsigned short TempDec;
	unsigned char temperature[2]; //��Ŷ�ȡ�����¶�����
	unsigned char temp[4];        //����¶���ֵ
	unsigned char temparray[10]={0xFC,0x44,0x79,0x5D,0xC5,0x9D,0xBD,0x54,0xFD,0xDD}; //����ȡ�õ�LED������ʾֵ
	unsigned char tempdparray[10]={0xfe,0x46,0x7b,0x5f,0xC7,0x9f,0xBf,0x56,0xFf,0xDf}; //����LED�ĵڶ�λ������С�����������ʾֵ
	float T=0.0;
	reset_ds1820();
	writeData_ds1820(0xcc); //оƬָ���ԾROM
	writeData_ds1820(0x44); //оƬָ��¶�ת��
	reset_ds1820();
	writeData_ds1820(0xcc);
	writeData_ds1820(0xbe); //оƬָ���RAM��ȡ����
	for(i=0;i<2;i++){       //��1820��ȡ����
		temperature[i]=readData_ds1820(); 
	}
	reset_ds1820();
	
	temp_data=temperature[1];
	temp_data&=0xf0; //��1111 0000�������㣬��ȡ����λ
	
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
	temp_data = temperature[1]<<4; //ȡ�ڶ��ζ�ȡ��������Ϊ����λ
	temp_data2 = temperature[0]>>4; //ȡ��һ�ζ�ȡ��������Ϊ����λ
	temp_data = temp_data|temp_data2; //��ϳ�Ϊ���յ��¶�ֵ
	
	//�¶�ֵת��
	temp[0]=(temp_data%100)/10;   //10
	temp[1]=(temp_data%100)%10;   //1 ȡ��LED�ĵڶ�λ������λ
	temperature[0]&=0x0f;         //ȡ���¶�ֵ�ĺ���λ 
	//DS18B20�ľ���Ϊ0.0625�ȣ�*0.0625֮��ת�����õ��¶ȼ�Ϊʵ���¶�
	TempDec = temperature[0]*625;
	temp[2] = TempDec/1000;       //1 ȡ��С�����һλ
	temp[3] = (TempDec%1000)/100; //10 ȡ��С�����ڶ�λ
	
	T = 10*(float)temp[0]+(float)temp[1]+0.1*(float)temp[2]+0.01*(float)temp[3]; //�����¶�ֵ
	if(T==85.0)return 0;
	if(temp_now==0){
		temp_now = T;
	}
	
	if(abs(T-temp_now)<1){
		LED[0] = 0xd1; //0xd1��1101 0001 
		LED[1] = 0xb8; //��1011 1000 �������������LED����ʾ���
		LED[2] = 0x00; //LED����
		LED[3] = 0x00;
		LED[4] = temparray[temp[0]]; //��ʾ�¶�ֵ
		LED[5] = tempdparray[temp[1]];
		LED[6] = temparray[temp[2]];
		LED[7] = temparray[temp[3]];
		temp_now = T;
	}
	
	//if((T<40.0) || (abs(T-temp_now)<1)){}
	return temp_now;
}