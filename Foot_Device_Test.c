/*
	This is the code fot my foot assistive device
	Author: James Dickson
	Date: 12/10/18

	Servo:
	 below 23
	fr is reverse of fl
	fl full taught at 1 (full loose at 24)
	fr full loose at 1 (full taught at 24)

	bl full loose at 1 (full taught at 24)
	br full taught at 1(full loose at 24) 
*/


#include <math.h>
#include <stdint.h>
#include <float.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <softPwm.h>

#define Front_Left 12
#define Back_Left 16
#define Front_Right 20
#define Back_Right 21

//#define BUTTON1 
//#define BUTTON2 

#define Front_LED 13
#define Back_LED 26
#define Left_LED 6
#define Right_LED 19

int main()
{
	printf("Code is running\n");
	wiringPiSetupGpio();

		softPwmCreate(Front_Left,1,100);	//Setup Front Left
		softPwmCreate(Back_Right,1,100);	//Setup Back Right
		softPwmCreate(Front_Right,1,100);	//Setup Front Right
		softPwmCreate(Back_Left,1,100);	//Setup Back Left

		pinMode(Front_LED, OUTPUT);
		pinMode(Back_LED,OUTPUT);
		pinMode(Left_LED,OUTPUT);
		pinMode(Right_LED,OUTPUT);

	//	int i = 0;
	//pinMode (BUTTON1,INPUT);
	//pinMode (BUTTON2,INPUT);
	for (;;) //Walking loop
	{
		//if (i == 1){
	digitalWrite (Front_LED,LOW);	//Send all forwards
	delay (250);
//	softPwmWrite(Back_Left,50);
//	softPwmWrite(Front_Left,35);
//	softPwmWrite(Back_Right,35);
//	softPwmWrite(Front_Right,35);
	printf("Forwards\n");
	//i = i - 1;
	delay(1000); //}
	//if (i ==  0){
	digitalWrite (Front_LED, HIGH);
//		softPwmWrite(Back_Left,23);
//	        softPwmWrite(Front_Left,65);
//        	softPwmWrite(Back_Right,65);
//       	softPwmWrite(Front_Right,65);
	//	i = i + 1;
		printf("Back\n");
		delay (1000);
	//}

	/* Individual
	digitalWrite (Front_LED,LOW);
	softPwmWrite(Back_Left,90);
	printf("Back Left\n");
	delay(1000);
	digitalWrite (Back_LED,HIGH);
	softPwmWrite(Front_Right,90);
	printf("Front Right\n");
	delay(1000);
	digitalWrite(Back_LED,LOW);
	softPwmWrite(Back_Right,90);
	printf("Back Right\n");
	delay(1000); //Wait for a second
	digitalWrite (Left_LED,HIGH);
	softPwmWrite(Front_Left,10);
	delay(1000);
	digitalWrite (Left_LED,LOW);
	softPwmWrite(Back_Left,10);
	delay(1000);
	digitalWrite(Right_LED,HIGH);
	softPwmWrite(Front_Right,90);
	delay(1000);
	digitalWrite(Right_LED,LOW);
	softPwmWrite(Back_Right,90);
	delay(1000);
	*/
	}

	return 0;
}
