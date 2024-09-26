sbit ds1820_dq = P1^3;
 
extern float temp_now;
 
void init_ds1820(void);
void reset_ds1820(void);
void writeData_ds1820(unsigned char);
float show_temperature(void);
unsigned char readData_ds1820(void);