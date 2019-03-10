#include <iostream>
#include<cstdio>
#include<linux/i2c-dev.h>
#include<linux/i2c.h>
#include<string.h>
#include <fstream>
#include<fcntl.h>  // THis is used for O_RDWR
#include<unistd.h>  // This used for open()
#include<sys/ioctl.h>
using namespace std;
#define BUFFER_SIZE 19      //0x00 to 0x12
#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)
int bcdToDec(char b) { return (b/16)*10 + (b%16); }
int i2cfile;
class Rpi2c{
protected:

	int rtcdata=0;
public:
	//Rpi2c(int);
	int connection();
	int address= 0x68;
	int rtc_w();
	int rtc_r();

	int number=7;
	int data=0;
	unsigned char value=0x00;
	unsigned char buffer[BUFFER_SIZE];
	//virtual ~Rpi2c();
};

 int Rpi2c::connection()
 {
		char filename[11] = "/dev/i2c-1";

	 i2cfile= open(filename, O_RDWR);

	 if(i2cfile <0){
		 cout << "Failed to open the bus" << endl;
	 }

	 if(ioctl(i2cfile, I2C_SLAVE, address) <0){
		 perror("failed to connect to the sensor\n");

		 cout << "For slave I2C address can't be set" << endl;

	 }
 }
 int Rpi2c::rtc_w(){
	// unsigned char buffer[1];
	 buffer[0]=value;
	rtcdata= write(i2cfile, buffer, 1);

	if(rtcdata !=1){
		perror("I2c failed to write the device");
return 1;
	}
	return 0;
 }