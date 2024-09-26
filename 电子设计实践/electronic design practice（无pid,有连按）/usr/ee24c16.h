sbit scl = P1^1;
sbit sda = P1^0;
 
#define WriteDeviceAddress 0xa0
#define ReadDeviceAddress 0xa1
 
unsigned char readbyte_24c16(unsigned char);
void writebyte_24c16(unsigned char,unsigned char);
unsigned char readbyte_I2C(void);
void writebyte_I2C(unsigned char);
void I2C_noack(void);
void I2C_ack(void);
void I2C_stop(void);
void I2C_start(void);
void init_24c16(void);
void save_all_data(void);