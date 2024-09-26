#define HD7279_RESET 0xa4
 
sbit cs = P1^4;
sbit dat = P1^7;
sbit clk = P1^5;
sbit key = P1^6;
 
extern unsigned int flag;


void init_hd7279(void);
void send_byte(unsigned char);
unsigned char receive_byte(void);
unsigned char readKey(void);
void display(void);
void write_hd7279(unsigned char,unsigned char);
void menu_switch(void);
int showmathgame(void);
int run_menu_switch(int);
int pa_menu_switch(int num);
int keyContinue(int keynum);