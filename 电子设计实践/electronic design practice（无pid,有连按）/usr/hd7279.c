#include <REG52.H>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#include <hd7279.h>
#include <global.h>
extern unsigned char temparray[10]={0xFC,0x44,0x79,0x5D,0xC5,0x9D,0xBD,0x54,0xFD,0xDD}; //����С����
//����LED�ͼ���
unsigned int flag = 0;

//7279��ʼ��
void init_hd7279(void){
	send_byte(HD7279_RESET);
}

//�����ֽ�
void send_byte(unsigned char byte){
	unsigned char i;
	cs = 0;
	long_delay();
	for(i=0;i<8;i++){
		if(byte&0x80){ //ÿ��ȡ���λ���ͣ�������λΪ1������1
			dat = 1;
		}
		else{ //������0
			dat = 0;
		}
		clk = 1;
		short_delay(); //����һ���½���
		clk = 0;
		short_delay();
		byte = byte*2; //�ֽ�����
	}
	dat = 0;
}

//�����ֽ�
unsigned char receive_byte(void){
	unsigned char i,byte;
	dat = 1;
	long_delay();
	for(i=0;i<8;i++){
		clk = 1;
		short_delay();
		//����һλ���ճ����һλ��Ž�������λdat
		byte = byte*2; //�����Ƴ�2��������
		if(dat){       //�������ݿ���dat��ֵ����byte
			byte = byte|0x01;
		}
		clk = 0;
		short_delay();
	}
	dat = 0;
	return(byte);
}


//����ֵ
unsigned char readKey(void){
	unsigned char key;
	//cs = 0;
	send_byte(0x15); //����ֵ����0x15
	key = receive_byte();
	//cs = 1;
	return(key);
}

//led��ʾ
void display(){
	unsigned char i;
	//cs = 0;
	for(i=0;i<8;i++){
		write_hd7279(0x90+i,LED[i]); //0x90���Ƶ�i��led��LED[i]�����䷢��������
	}
	//cs = 1;
}

//7279д��
void write_hd7279(unsigned char position,unsigned char dat){
	send_byte(position);
	send_byte(dat);
}

//�˵����ĸ�����
void menu_switch(void){
	switch(keyvalue){ //����ֵ
		case 0x3b:
		{
			menu_num++;              //����ѡ�����ϼ�
			menu_num = menu_num%5;
			break;
		}
		case 0x3a:
		{	
		   
			menu_num=(menu_num+4)%5; //����ѡ�����ϼ�
			break;
		}
		case 0x39:
		{
			layer = 1;               //������һ�㣬�����¼�
			break;
		}
		case 0x38:                 //����˵��ĵڶ��㣬�����¼�
		{
			layer = 2;
			keyvalue = 0xff;
			break;
		}
	}
}
//����run���� �������
int run_menu_switch(int num){
	switch(keyvalue){
		case 0x3b: //���ټ�
		{
			num++;
			num = num%5;
			break;
		}
		case 0x3a: //���ټ�
		{
			num=(num+4)%5;
			break;
		}
		case 0x39: //������һ��
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
		case 0x38: //������һ�㣬�������
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

//����pa���ܣ��������ã���ΪP(��λ)��A(�ٶ�)����
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
		case 0x39: //������һ��
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

  
//��������
int keyContinue(int keynum){
	unsigned char tempkv;
	unsigned int kc1=0,kc2=0;
	unsigned int pastep=0;   //���䲽��

	delay(10); //��������
	tempkv=readKey();    //����ǰ��ֵ
	if(tempkv==0xff){    //��ǰû�м�������
		keyvalue=0xff;
		kc1=0;
		kc2=0;
	}
	else{                 //˵����ǰ����tempkv��������
		while((keyvalue==0x3a|keyvalue==0x3b)&&tempkv!=0xff){ //ͬʱkeyvalue��ֵҲ�У�˵����ʱ��������
			delay(200);
	
			if(keyvalue==0x3a){           //������µļ�Ϊ�Ӽ�
				kc1++;                      //kc1��ʼ��ʼ��
				if(kc1==5){ kc1=0; kc2++;}  //����������ʱ��ֵ�����俪ʼ����
				keynum=keynum+kc1+kc2*pastep;
		  }
			
			else if(keyvalue==0x3b){      //������µļ�Ϊ����
				kc1++;                      //kc1��ʼ��ʼ��
				if(kc1==5){ kc1=0; kc2++;}  //����������ʱ��ֵ�����俪ʼ����
				keynum=keynum-kc1-kc2*pastep;
			}
			
			if((keynum/100)!=0)LED[1]=temparray[keynum/100]; //��ʾ
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

		