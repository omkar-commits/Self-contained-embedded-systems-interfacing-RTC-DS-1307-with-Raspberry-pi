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
int decToBcd (char b) { return (b/10*16) + (b%10); }
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
	unsigned char minutes=0x01;
	unsigned char hours=0x02;
	unsigned char dayOfweek=0x03;
	unsigned char day=0x04;
	unsigned char month=0x05;
	unsigned char year=0x06;
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
	 buffer[1]=0x01;
	 buffer[2]=0x08;
	 buffer[3]=0x03;
	 buffer[4]=0x04;
	 buffer[5]=0x05;
	 buffer[6]=0x06;
	/* buffer[7]=0x07;
	 buffer[8]=0x08;
	 buffer[9]=0x09;
	 buffer[10]=0x10;
	 buffer[11]=0x11;
	 buffer[12]=0x12;*/
	rtcdata= write(i2cfile, buffer, 7);

	if(rtcdata !=7){
		perror("I2c failed to write the device");
return 1;
	}
	return 0;
 }


 int Rpi2c::rtc_r(){
 	// unsigned char* data= new unsigned char [number];
 	 buffer[0]=value;
 	 buffer[1]=0x01;
 		 buffer[2]=0x02;
 		 buffer[3]=0x03;
 		 buffer[4]=0x04;
 		 buffer[5]=0x05;
 		 buffer[6]=0x06;
 	rtcdata= read(i2cfile, buffer,7);

 	if(rtcdata !=7){
 		perror("I2c failed to read data from the device");
 		return 1;
 	}
 	else           // return data
 			{

 				char seconds = buffer[0];
 				char minutes = buffer[1];
 				char hours = buffer[2];
 				char dayOfWeek = buffer[3];
 				char day = buffer[4];
 				char month = buffer[5];
 				char year = buffer[6];

 				cout << "The Time in RTC is:" << endl;
 				cout << "Date Y/M/D:"<<  bcdToDec(year)<<"-"<<  bcdToDec(month)<< "-"<<  bcdToDec(day)<<endl;
 				cout << "day is " << bcdToDec(dayOfWeek) << endl;
 				cout << "Time H/M/S: "<<  bcdToDec(hours)<< "-"<<   bcdToDec(minutes)<< "-"<<  bcdToDec(seconds)<<endl;
  }

 return 0;
 }



 int main (){

	 Rpi2c x;
	 x.connection();
	 x.rtc_w();
	 x.rtc_r();
	 close (i2cfile);
	 return 0;
 }


