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
int decToBcd(char b){ return (b/10*16) + (b%10); }
int i2cfile;
typedef struct {
        int sec;        
        int min;        
        int hour;       
        int wday;       
        int date;       
        int mon;        
        int year;       
    } Time_rtc;   // Structure defined to set alarm timr


typedef enum {
		        RS1Hz = 0,
		        RS4kHz = 1,
		        RS8kHz = 2,
		        RS32kHz = 3
		    } SqwRateSelect_t; // 

class Rpi2c{
protected:

	int rtcdata=0;
public:
	//Rpi2c(int);
	int connection();
	int address= 0x68;
	int rtc_w();
	int rtc_read();
	int rtc_alarm();

	bool swq_op(bool ena,SqwRateSelect_t rs);
	int current_time();
//	int rtc_write();

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
	 return 0;
 }
 int Rpi2c::rtc_w(){
	// unsigned char buffer[1];
	buffer[0]=value; // CH bit to "0"
	rtcdata= write(i2cfile, buffer, 1);

	if(rtcdata !=1){
		perror("I2c failed to write the device");
return 1;
	}
else{
	 //buffer[0]=value;
	 	 buffer[1]=0x05;
	 	 buffer[2]=0x09;
	 	 buffer[3]=0x03;
	 	 buffer[4]=0x02;
	 	 buffer[5]=0x18;
	 	 buffer[6]=0x03;
	 	 buffer[7]=0x19;

	 	rtcdata= write(i2cfile, buffer, 8);

	 	if(rtcdata !=8){
	 		perror("I2c failed to write the device");
	 return 1;
	 	}
	 	else{
	 		unsigned char seconds = buffer[1];
	 		unsigned char minutes = buffer[2];
	 		 				unsigned char hours = buffer[3];
	 		 				unsigned char dayOfWeek = buffer[4];
	 		 				unsigned char day = buffer[5];
	 		 				unsigned char month = buffer[6];
	 		 				unsigned char year = buffer[7];

	 		 				cout << "The Time written in RTC is:" << endl;
	 		 				 				cout << "Date Y/M/D:"<<  bcdToDec(year)<<"-"<<  bcdToDec(month)<< "-"<<  bcdToDec(day)<<endl;
	 		 				 				cout << "Day of this week"<<  bcdToDec(dayOfWeek)<<endl;
	 		 				cout << "Time H/M/S: "<<  bcdToDec(hours)<< "-"<<   bcdToDec(minutes)<< "-"<< bcdToDec(seconds)<<endl;
	 	}
//	return buffer;
 }
	return 0;
 }
 int Rpi2c::rtc_read(){
 	// unsigned char* data= new unsigned char [number];
 	// buffer[0]=value;
 	rtcdata= read(i2cfile, buffer,7);

 	if(rtcdata !=7){
 		perror("I2c failed to read data from the device");
 		return 1;
 	}
 	else           // return data
 			{

 				unsigned char seconds = buffer[0];
 				unsigned char minutes = buffer[1];
 				unsigned char hours = buffer[2];
 				unsigned char dayOfWeek = buffer[3];
 				unsigned char day = buffer[4];
 				unsigned char month = buffer[5];
 				unsigned char year = buffer[6];

 				cout << "The Time in RTC is:" << endl;
 				cout << "Date Y/M/D:"<<  bcdToDec(year)<<"-"<<  bcdToDec(month)<< "-"<<  bcdToDec(day)<<endl;
 				cout << "Day of this week"<<  bcdToDec(dayOfWeek)<<endl;
 				cout << "Time H/M/S: "<<  bcdToDec(hours)<< "-"<<   bcdToDec(minutes)<< "-"<<  bcdToDec(seconds)<<endl;

  }

 return 0;
 }
 int Rpi2c::current_time(){
  	// unsigned char* data= new unsigned char [number];
  	// buffer[0]=value;
  	rtcdata= read(i2cfile, buffer,7);

  	if(rtcdata !=7){
  		perror("I2c failed to read data from the device");
  		return 1;
  	}
  	else           // return data
  			{

  				unsigned char seconds = buffer[0];
  				unsigned char minutes = buffer[1];
  				unsigned char hours = buffer[2];
  				unsigned char dayOfWeek = buffer[3];
  				unsigned char day = buffer[4];
  				unsigned char month = buffer[5];
  				unsigned char year = buffer[6];

  				cout << "The Time in RTC is:" << endl;
  				cout << "Date Y/M/D:"<<  bcdToDec(year)<<"-"<<  bcdToDec(month)<< "-"<<  bcdToDec(day)<<endl;
  				cout << "Day of this week"<<  bcdToDec(dayOfWeek)<<endl;
  				cout << "Time H/M/S: "<<  bcdToDec(hours)<< "-"<<   bcdToDec(minutes)<< "-"<<  bcdToDec(seconds)<<endl;

   }

  return 0;
 }
 int Rpi2c::rtc_alarm(){
	// unsigned char buffer[1];
	buffer[8]={0x07}; // setting the Timekeepr registers
	rtcdata= write(i2cfile, buffer, 1);

	if(rtcdata !=1){
		perror("I2c failed to write the device");
return 1;
	}
else{
	 //buffer[0]=value;
	 	 buffer[9]=0x05;
	 	 buffer[10]=0x09;
	 	 buffer[11]=0x03;
	 	 buffer[12]=0x02;
	 	 buffer[13]=0x18;
	 	 buffer[14]=0x03;
	 	 buffer[15]=0x19;

	 	rtcdata= write(i2cfile, buffer, 8);

	 	if(rtcdata !=8){
	 		perror("I2c failed to write the device");
	 return 1;
	 	}
	 	else{
	 		unsigned char seconds = buffer[9];
	 		unsigned char minutes = buffer[10];
	 		 				unsigned char hours = buffer[11];
	 		 				unsigned char dayOfWeek = buffer[12];
	 		 				unsigned char day = buffer[13];
	 		 				unsigned char month = buffer[14];
	 		 				unsigned char year = buffer[15];

	 		 				cout << "The Time written in RTC for alarm: " << endl;
	 		 						cout << "Date Y/M/D:"<<  bcdToDec(year)<<"-"<<  bcdToDec(month)<< "-"<<  bcdToDec(day)<<endl;
	 		 				 				cout << "Day of this week"<<  bcdToDec(dayOfWeek)<<endl;
	 		 				cout << "Time H/M/S: "<<  bcdToDec(hours)<< "-"<<   bcdToDec(minutes)<< "-"<< bcdToDec(seconds)<<endl;
	 	}
//	return buffer;
 }
	return 0;
 }
 
/* if ((buffer[1] && buffer[9] || buffer[2] && buffer[10] ||buffer[3] && buffer[11] ||buffer[4] && buffer[12] ||buffer[5] && buffer[13] || buffer[6] && buffer[14] ||buffer[7] && buffer[15])=1){
	 
	 cout<< " the RTc current timr and set alarm time are same -> trigger " << endl;
	 
 }*/
/*
bool Rpi2c::swq_op(bool ena, SqwRateSelect_t rs){
	unsigned char buffer[8]= {0x07}; // firtst trying to read buffer values.
	cout << "firtst trying to read buffer values" << endl;
		if (!read(7,buffer, 1)){
			perror("Failed to read buffer values  .");
return 0;
}
		cout << "[buffer:0x07] = %02x\n" << buffer[8] <<endl;

		//  preserve the OUT control bit while writing the frequency and enable bits
		   buffer[8] = (buffer[8] & 0x80) | (ena ? 0x10 : 0) | ((char)rs & 0x03);
			cout << "Writing back registers value" << endl;
			cout << "[buffer:0x07] = %02x\n" << buffer[8] <<endl;

			if (!write(7,buffer, 1)){
						perror("Failed to write buffer values  .");
			return 0;
			}
			cout << "Successfuly changes the squarewave output" << endl;
			return 1;
}

*/


 int main (){

	 Rpi2c x;
	 x.connection();
	 x.current_time();
	 x.rtc_w();
	 x.rtc_read();
	 x.rtc_alarm();
	// x.swq_op(true,RS4kHz);

	 close (i2cfile);
	 return 0;
 }
