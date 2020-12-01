# EE513_Assignment
#https://gitlab.com/pabsheo2/ee513_assignment1.git
#Self-contained embedded systems interfacing RTC DS-1307 with Raspberry pi

Code written for self-contained embedded systems interfacing RTC DS-1307 with Raspberry pi. The report has section 1 which shows the physical connection of RTC module Ds-1307 with I2C bus. Section 2 represents the testing carried out by I2C tools, Section 3 represents the results of C++ code which gives the results based on the functionalities described by RTC DS1307 datasheet. Section 3 includes the output of method used to read and display date and time from DS 1307 followed by method to set date and time in RTC. And final section shows the integration of Linux OS with RTC using pre written RTC LKM . 

Section 1: Physical Connection  
The connection if RTC to RPI using I2C bus are carried out using the reference from assignment material where using Pin 6 to GND, Pin 4 power supply of 5v as per datasheet, Pin 2 for SDA and Pin 3 for SCL. The hardware  bus for ds1307 i2c is I2C1. 
 
 Section 2: Testing carried out using I2C tools 
1. Sudo I2cdetect -y 1 the address of RTC is at 0x68 
2. To gain access to various device registers I2C Dump command is used. It is executed twice to observe change in values at register and by referring datasheet it can be understood. 

Section 3: C++ class to wrap I2c communication with Ds1307 using RPI: 

Code Approach : 
 The code has main class which describes the values required for communication. The class “Rpi2c” has functions such as connection ( To check connection), Rtc_w( write function), Rtc_r(read function), Current_time(to read and show current time before write function is called to set time. Sqw_op to set and control square wave.   
 
1.  Read and display of current time and date of RTC module 
2. Setting the date and time in RTC by write function code and verify it by calling read function.
3. Verifying the clock running by observing using I2c dump 
4. Integration of RTC with Linux OS using LKM 
 
 
