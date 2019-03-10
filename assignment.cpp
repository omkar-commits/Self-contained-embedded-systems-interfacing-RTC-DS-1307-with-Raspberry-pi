//============================================================================
// Name        : assignment1ee513.cpp
// Author      : Omkar
// Version     :
// Copyright   : Your copyright notice
// Description : AssignmentEE513, Ansi-style
//============================================================================
#include <iostream>
#include<cstdio>
#include<linux/i2c-dev.h>
#include<linux/i2c.h>
#include<string.h>
#include<fcntl.h>  // THis is used for O_RDWR
#include<unistd.h>  // This used for open()
#include<sys/ioctl.h>
using namespace std;
#define BUFFER_SIZE 19      //0x00 to 0x12
#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)
int bcdToDec(char b) { return (b/16)*10 + (b%16); }

class Rpi2c{
protected:
	int i2cfile;
public:
	Rpi2c(int);
	//int file;
	int address= 0x68;
	int I2C_SLAVE;
	int rtc_w();
	int rtc_r();
	//int rtcdata;
	int number=7;
	int data=0;
	unsigned char value;
	unsigned char buffer[BUFFER_SIZE];
	//virtual void close();
	virtual ~Rpi2c();
};

 Rpi2c::Rpi2c(int address)
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

 class rtc_w: public Rpi2c{

 private:
	 int rtcdata;
public:

	 int data;
	// unsigned char value;

 };
 int Rpi2c::rtc_w(int rtcdata){
	 unsigned char buffer[1];
	 buffer[0]=value;
	rtcdata= write(i2cfile, buffer, 1);

	if(rtcdata !=1){
		perror("I2c failed to write the device");
		return 1;
	}
	return 0;
 }

 class rtc_r: public Rpi2c{

 private:
	 int rtcdata;
public:

	 int data;
	// unsigned char value;

 };
 int Rpi2c::rtc_r(int rtcdata){
 	// unsigned char* data= new unsigned char [number];
 	 buffer[0]=value;
 	rtcdata= read(i2cfile, buffer, (int) number);

 	if(rtcdata !=(int)number){
 		perror("I2c failed to read data from the device");
 		return NULL;
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
 				cout << "Time H/M/S: "<<  bcdToDec(hours)<< "-"<<   bcdToDec(minutes)<< "-"<<  bcdToDec(seconds)<<endl;
  }

 return 0;
 }

 void Rpi2c::close(){
	 close (i2cfile);
	 i2cfile=-1;
 }
 Rpi2c::~Rpi2c(){
	 if(i2cfile != -1)
		 close (i2cfile);

 }

 int main (){

	 Rpi2c date;
	 date.rtc_w;
	 date.rtc_r;
	 return 0;
 }

/*
#include <iostream>
#include<linux/i2c-dev.h>
#include<linux/i2c.h>
#include<string.h>
#include<fcntl.h>  // THis is used for O_RDWR
#include<unistd.h>  // This used for open()
#include<sys/ioctl.h>

using namespace std;
int bcdToDec(char b) { return (b/16)*10 + (b%16); }

class Rpi2c{
private:
	int i2cfile;
public:
	Rpi2c(int);
	//int file;
	int address= 0x68;
	//int I2C_SLAVE;
};

 Rpi2c::Rpi2c(int address)
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

 Rpi2c::write(unsigned char value){

    unsigned char buffer[1];

    buffer[0]=value;

    if (::write(i2cfile, buffer, 1)!=1){

       perror("I2C: Failed to write to the device.");

       return 1;

    }

    return 0;

 }
  Rpi2c::readRegister(int registeraddress){

    write(registeraddress);

    unsigned char buffer[2];

    if(::read(i2cfile, buffer, 1)!=1){

       perror("I2C: Failed to read in the value.");

       return 1;

    }

    return buffer[0];

 }
 
int main (){
     cout << "Finally the code runs till here" << endl;

}

#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<unistd.h>
#include<stdint.h>

#define BUFFER_SIZE 19      //0x00 to 0x12

// the time is in the registers in encoded decimal form
int bcdToDec(char b) { return (b/16)*10 + (b%16); }

int main(){
   int file;
   printf("Starting the DS1307 test application\n");
   if((file=open("/dev/i2c-1", O_RDWR)) < 0){
      perror("failed to open the bus\n");
      return 1;
   }
   if(ioctl(file, I2C_SLAVE, 0x68) < 0){
      perror("Failed to connect to the sensor\n");
      return 1;
   }
   char writeBuffer[1] = {0x00};
   if(write(file, writeBuffer, 1)!=1){
      perror("Failed to reset the read address\n");
      return 1;
   }
   char buf[BUFFER_SIZE];
   if(read(file, buf, BUFFER_SIZE)!=BUFFER_SIZE){
      perror("Failed to read in the buffer\n");
      return 1;
   }
   printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buf[2]),
         bcdToDec(buf[1]), bcdToDec(buf[0]));
   close(file);
   return 0;
}
*/
