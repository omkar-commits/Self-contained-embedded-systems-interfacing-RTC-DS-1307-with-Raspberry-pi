//============================================================================
// Name        : assignment1ee513.cpp
// Author      : Omkar
// Version     :
// Copyright   : Your copyright notice
// Description : AssignmentEE513, Ansi-style
//============================================================================

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
	int I2C_SLAVE;
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

/*#include<stdio.h>
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
