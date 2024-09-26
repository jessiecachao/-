#include <REG52.H>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#include <global.h>
#include <hd7279.h>
#include <ds18b20.h>
#include <motor.h>
#include <ee24c16.h>
#include <pid.h>
 
int menu_num=0,layer=1,motorNow = 0,run_num=0,con_num=0;
unsigned char keyvalue=0xff;
unsigned char LED[8]={0xa9,0xf1,0x01,0x00,0x44,0x00,0x00,0x00};
float Tset = 28.0,kp=4.0,ki=0.2,kd=0.0;
unsigned int pwm=50;
 
void main(void){
	unsigned char tP[8]={0xa9,0xf1,0x01,0x00,0x44,0x00,0x00,0x00};
  unsigned char run[8]={0x21,0x2c,0x25,0x01,0x79,0x00,0x00,0x00};
  unsigned char Con[8]={0xb8,0x2d,0x25,0x01,0x5d,0x00,0x00,0x00};
  unsigned char PA[8]={0xf1,0xf5,0x01,0x00,0xc5,0x00,0x00,0x00};
	unsigned char PID[8]={0xf1,0x44,0x6d,0x01,0x9d,0x00,0x00,0x00};
	unsigned char run_all[8]={0x00,0x00,0x00,0x00,0x21,0x01,0x00,0x00};
	unsigned char tempdparray[10]={0xfe,0x46,0x7b,0x5f,0xC7,0x9f,0xBf,0x56,0xFf,0xDf};//带小数点的数字
	unsigned char temparray[10]={0xFC,0x44,0x79,0x5D,0xC5,0x9D,0xBD,0x54,0xFD,0xDD}; //不带小数点
	unsigned char pass_in[3]={0,0,0},pass[3]={1,2,2},wrong=1;
	unsigned char dat1;
	unsigned char key,i;
	int run_menu_num=-1,pa_menu_num=-1;
	unsigned int pw_now;
	unsigned int t_down,t_up;
	
	init_hd7279();
	init_24c16();
	init_ds1820();
	motor = 0;
	save_all_data(); //只让它执行一次 存EEPROM数据 以后要注释掉
	
	while(1){
		display();		
		key = readKey();
		if(layer==2 && menu_num==0)   //第一个菜单的第二层
		{
			EA = 0;
			show_temperature();   
		}
		else if(layer==2 && menu_num==2) //第二个菜单的第二层
		{     //占空比
			EA = 1;
			motor = 1;
			temp_now = show_temperature();
			//PID控制
			if(temp_now>t_down && temp_now<t_up){   
				pw_now=(temp_now-t_down)/(t_up-t_down)*50+50;    //显示当前占空比
				LED[1] = 0x00;
				LED[2] = temparray[(pw_now%100)/10];
				LED[3] = temparray[(pw_now%100)%10];
			}
			else if(temp_now>=t_up){//100
				LED[1] = 0x44;
				LED[2] = 0xfc;
				LED[3] = 0xfc;
			}
			else{
				LED[1] = 0x00;
				LED[2] = 0x00;
				LED[3] = 0xfc;
			}
			LED[0] = 0x00;
		}
		else if(layer==2 && menu_num==4){     //第五个菜单的第二层
			  	EA = 1;
			motor = 1;
			temp_now = show_temperature();
			//PID控制
			if(temp_now>t_down && temp_now<t_up){   
				pw_now=(temp_now-t_down)/(t_up-t_down)*50+50;    //显示当前占空比
				LED[1] = 0x00;
				LED[2] = temparray[(pw_now%100)/10];
				LED[3] = temparray[(pw_now%100)%10];
			}
			else if(temp_now>=t_up){//100
				LED[1] = 0x44;
				LED[2] = 0xfc;
				LED[3] = 0xfc;
			}
			else{
				LED[1] = 0x00;
				LED[2] = 0x00;
				LED[3] = 0xfc;
			}
			LED[0] = 0x00;
		/*	EA=1;
			pwm = pid_getpwm(pwm,show_temperature(),(float)readbyte_24c16(12),kp,ki,kd);     //.....................................................................
			LED[0]=0x00;
			LED[1]=0x00;
			LED[2]=temparray[(pwm%100)/10]; //10
			LED[3]=temparray[(pwm%100)%10]; //1
			//motor=1;
			if(motorNow<(unsigned int)(pwm)){
				motor = 1;
				P2 = 0x00;//绿灯亮
			}
			else{
			  motor = 0;
				P2 = 0x02; */
		//	}
		}
		
		else if(layer==3 && menu_num==1){    //run    第二个菜单的第三层
			EA = 1;//开启中断 
			if(motorNow<(unsigned int)(dat1)){
				motor = 1;
				P2 = 0x00;//绿灯亮
			}
			else{
			  motor = 0;
				P2 = 0x02;
			}
		}
 
		if(key==0xff){
			keyvalue = 0xff;
		}
		else{
			if(keyvalue==0xff){
				keyvalue = key;
				if(layer==1){   //第一层
					menu_switch();
					switch(menu_num){
						case 0:for(i=0;i<8;i++)LED[i]=tP[i];break;
						case 1:for(i=0;i<8;i++)LED[i]=run[i];break;
						case 2:for(i=0;i<8;i++)LED[i]=Con[i];break;
						case 3:for(i=0;i<8;i++)LED[i]=PA[i];break;
						case 4:for(i=0;i<8;i++)LED[i]=PID[i];break;
					}
				}
				else if(layer == 2){     //第二层
					switch(menu_num){
						case 0:
						{
							if(keyvalue==0x39){
								layer = 1;
								keyvalue=0xff;
							}
							break;
						}
						case 1:
						{
							run_menu_num=run_menu_switch(run_menu_num);
							for(i=0;i<8;i++)LED[i]=run_all[i];
							switch(run_menu_num){
								case 0:{
									LED[7] = 0x44;
									dat1=readbyte_24c16(0);
									break;
								}
								case 1:{
									LED[7] = 0x79;
									dat1=readbyte_24c16(1);
									break;
								}
								case 2:{
									LED[7] = 0x5d;
									dat1=readbyte_24c16(2);
									break;
								}
								case 3:{
									LED[7] = 0xc5;
									dat1=readbyte_24c16(3);
									break;
								}
								case 4:{
									LED[7] = 0x9d;
									dat1=readbyte_24c16(4);
									break;
								}  
							}
							if((dat1/100)!=0)LED[1]=temparray[dat1/100];
							else LED[1]=0x00;
							LED[2]=temparray[(dat1%100)/10];
							LED[3]=temparray[(dat1%100)%10];
						break;
						}	
						case 2:
						{
						 // motor=1;               
							t_down = readbyte_24c16(10);
							t_up =readbyte_24c16(11);
							if(keyvalue==0x39){
								layer = 1;
								keyvalue=0xff;
							}
							break;
						}
						case 3://改密码的地方
						{
							if(wrong == 1){
							LED[0]=0x01;LED[1]=0x01;LED[2]=0x01;LED[3]=0x00;LED[4]=0xf1;LED[5]=0xf5;LED[6]=0x9d;LED[7]=0x00;
									switch(keyvalue)
									{	
										case 0x3B:
											pass_in[0]=(pass_in[0]+1)%3;break;
										case 0x3A:
											pass_in[1]=(pass_in[1]+1)%3;;break;	
										case 0x39:
											pass_in[2]=(pass_in[2]+1)%3;break;
										case 0x38:
										{
											if(pass_in[0]==pass[0]&&pass_in[1]==pass[1]&&pass_in[2]==pass[2])
												{
													wrong=0;//密码正确
													menu_num=3;layer = 2;
												}
										}		
											 
									}		
								LED[0]=temparray[(int)pass_in[0]];LED[1]=temparray[(int)pass_in[1]];LED[2]=temparray[(int)pass_in[2]];
									//break;	
								}
					 else if(wrong == 0){			                                                 																								                                                 																				
							pa_menu_num=pa_menu_switch(pa_menu_num);
							switch(pa_menu_num){
								case 0:LED[0]=0x00;LED[1]=0x21;LED[2]=0x2c;LED[3]=0x25;LED[4]=0xf1;LED[5]=0xf5;LED[6]=0x01;LED[7]=0x00;break;//run
								case 1:LED[0]=0x00;LED[1]=0xb8;LED[2]=0x2d;LED[3]=0x25;LED[4]=0xf1;LED[5]=0xf5;LED[6]=0x01;LED[7]=0x00;break;//con
								//case 2:LED[0]=0x00;LED[1]=0xf1;LED[2]=0x44;LED[3]=0x6d;LED[4]=0xf1;LED[5]=0xf5;LED[6]=0x01;LED[7]=0x00;break;//pid调节
  							}
						}					    
							break;
						}
						case 4:{
							init_motor();
							if(keyvalue==0x39){
								layer = 1;
								keyvalue=0xff;
							}
							break;
						};
					}
				}
				else if(layer==3){   //第三层
					switch(menu_num){
						case 0:
						{
							break;
						}
						case 1:
						{
							if(keyvalue==0x39){
								layer = 2;
								run_menu_num = -1;
								keyvalue=0xff;
								motor = 0;
								P2 = 0x02;
							}
							init_motor();
							for(i=0;i<8;i++)LED[i]=run_all[i];
							switch(run_menu_num){
								case 0:{
									dat1 = readbyte_24c16(0);
									break;
								}
								case 1:{
									dat1 = readbyte_24c16(1);
									break;
								}
								case 2:{
									dat1 = readbyte_24c16(2);
									break;
								}
								case 3:{
									dat1 = readbyte_24c16(3);
									break;
								}
								case 4:{
									dat1 = readbyte_24c16(4);
									break;
								}
							}
							if((dat1/100)!=0)LED[1]=temparray[dat1/100];
							else LED[1]=0x00;
							LED[2]=temparray[(dat1%100)/10];
							LED[3]=temparray[(dat1%100)%10];
							LED[4]=0x21;//r
							LED[5]=0x2c;//u
							LED[6]=0x25;//n
							break;
						}
						case 2:break;
						case 3:
						{	
							if(flag==0){
							//run
								run_menu_num=run_menu_switch(run_menu_num);
								if(LED[4]==0xf5){
									run_menu_num = run_num;
								}
								for(i=0;i<8;i++)LED[i]=run_all[i];
								switch(run_menu_num){
									case 0:{
										LED[7] = 0x44;
										dat1=readbyte_24c16(0);
										break;
									}
									case 1:{
										LED[7] = 0x79;
										dat1=readbyte_24c16(1);
										break;
									}
									case 2:{
										LED[7] = 0x5d;
										dat1=readbyte_24c16(2);
										break;
									}
									case 3:{
										LED[7] = 0xc5;
										dat1=readbyte_24c16(3);
										break;
									}
									case 4:{
										LED[7] = 0x9d;
										dat1=readbyte_24c16(4);
										break;
									}
								}
								if((dat1/100)!=0)LED[1]=temparray[dat1/100];
								else LED[1]=0x00;
								LED[2]=temparray[(dat1%100)/10];
								LED[3]=temparray[(dat1%100)%10];
								LED[4]=0xf1;								
							}

							else if (flag==1){   //con            
								pa_menu_num=pa_menu_switch(pa_menu_num);
								switch(pa_menu_num){
									case 0:dat1=readbyte_24c16(10);LED[0]=0x00;LED[1]=0x00;LED[2]=temparray[(readbyte_24c16(10)%100)/10];LED[3]=temparray[(readbyte_24c16(10)%100)%10];LED[4]=0xf1;LED[5]=0xf5;LED[6]=0x01;LED[7]=0xad;break;//b
									case 1:dat1=readbyte_24c16(11);LED[0]=0x00;LED[1]=0x00;LED[2]=temparray[(readbyte_24c16(11)%100)/10];LED[3]=temparray[(readbyte_24c16(11)%100)%10];LED[4]=0xf1;LED[5]=0xf5;LED[6]=0x01;LED[7]=0xb1;break;//f
								}
								//break; 
							}
							
//							else 
//							{
//								//pid
//							 	pa_menu_num=pa_menu_switch(pa_menu_num);
//						switch(pa_menu_num){
//							case 0:LED[0]=0x00;LED[1]=0x00;LED[2]=0x00;LED[3]=0xf1;LED[4]=0xf1;LED[5]=0x44;LED[6]=0x6d;LED[7]=0x01;break;//p
//							case 1:LED[0]=0x00;LED[1]=0x00;LED[2]=0x00;LED[3]=0x44;LED[4]=0xf1;LED[5]=0x44;LED[6]=0x6d;LED[7]=0x01;break;//i
//							case 2:LED[0]=0x00;LED[1]=0x00;LED[2]=0x00;LED[3]=0x6d;LED[4]=0xf1;LED[5]=0x44;LED[6]=0x6d;LED[7]=0x01;break;//d
//									}
//								   
//									if(keyvalue==0x3a){
//										Tset++;
//									}
//								else if(keyvalue==0x3b){
//										Tset--;
//								}
//								else if(keyvalue==0x39){
//									layer = 2;
//									keyvalue=0xff;
//									
//								} 
//								else{
//									  
//										delay_us(150);
//										writebyte_24c16((unsigned char)Tset,12);
//										delay_us(150);
//								}
//								LED[0]=0x00;LED[1]=temparray[(int)(Tset*10)/100];LED[2]=tempdparray[(int)((Tset*10)/10)%10];LED[3]=temparray[(int)(Tset*10)%10];LED[4]=0xf5;LED[5]=0x01;LED[6]=0x00;LED[7]=0xf1;//显示设置的温度
//								
//							}
							break;
						}  
						
						case 4:break;
						case 5:break;
					}	
				}

        else{                   //第四层
					if(flag==0){
						//run
						if(keyvalue==0x3a){
							if(dat1>=0 && dat1<100){
								//dat1++;
								dat1=keyContinue(dat1); 
								if((dat1/100)!=0)LED[1]=temparray[dat1/100];
								else LED[1]=0x00;
								LED[2]=temparray[(dat1%100)/10];
								LED[3]=temparray[(dat1%100)%10];
							}
						}
						else if(keyvalue==0x3b){
							if(dat1>0 && dat1<=100){
							//	dat1--;
								dat1=keyContinue(dat1); 
								if((dat1/100)!=0)LED[1]=temparray[dat1/100];
								else LED[1]=0x00;
								LED[2]=temparray[(dat1%100)/10];
								LED[3]=temparray[(dat1%100)%10];
							}
						}
						else if(keyvalue==0x39){
							layer = 3;
							keyvalue=0xff;
						}
						else{
							if(LED[4] == 0xf5){
								writebyte_24c16(dat1,run_num);
								layer = 3;
								keyvalue=0xff;
							}
						}
					}
					
					else if(flag==1){
						//con
						LED[1]=0x00;
						if(keyvalue==0x3a){
							if(dat1>=20 && dat1<40){
								if(dat1>=readbyte_24c16(11)&&LED[7]==0xad) //t_down小于t_up才能加  
									dat1=dat1;
								else
								//dat1++;	
								dat1=keyContinue(dat1); //键盘连按
							}
						}
						else if(keyvalue==0x3b){
							if(dat1>20 && dat1<=40){
								if(dat1<=readbyte_24c16(10)&&LED[7]==0xb1) //t_up大于t_down才能加      
									dat1=dat1;
								else
									dat1=keyContinue(dat1); 
									//dat1--;								
							}
						}
						else if(keyvalue==0x39){
							layer = 3;
							keyvalue=0xff;
						}
						else{
							if(LED[4] == 0xf5){
								if(con_num==0){
									writebyte_24c16(dat1,10);
								}
								else writebyte_24c16(dat1,11);
								layer = 3;
								keyvalue=0xff;
							}
						}

					}
		

				 
					LED[2]=temparray[(dat1%100)/10];
					LED[3]=temparray[(dat1%100)%10];
					LED[4] = 0xf5;
					LED[5] = 0x01;
					LED[6] = 0x00;
					//break;
					
					
				  
				  }
				 }
				 }
				 }
				 }
				 
				 
		
	


 
//定义中断函数，interrupt 1即为定时器中断
//中断函数没有返回值	
void timer0(void) interrupt 1{
	TH0 = 0xee;
	TL0 = 0x00;
	motorNow++;
	if(motorNow>=100){
		motorNow = 0;
	}
}