extern int menu_num,layer,run_num,con_num;
extern unsigned char keyvalue;
extern unsigned char LED[8];
extern float Tset,kp,ki,kd;
extern unsigned int pwm;
extern unsigned char temparray[10];
extern unsigned char tempdparray[10];
 
void long_delay(void);
void short_delay(void);
void delay(unsigned int);
void delay_us(unsigned int);