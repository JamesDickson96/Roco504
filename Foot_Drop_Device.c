/*
		Foot Assistance robot code
		By James Dickson

Notes:
	-Gyro reads values perfectly
	-LED System Correct

	-Servos cause issues
	Use servo below 23
*/

#include <wiringPiI2C.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <string.h>
#include <wiringPi.h>
#include <math.h>
#include <softPwm.h>
#include <errno.h>

#define Device_Address 0x68

#define Button 24

#define Front_LED 13
#define Back_LED 26
#define Left_LED 6
#define Right_LED 19

#define Front_Left 12
#define Back_Left 16
#define Front_Right 20
#define Back_Right 21

volatile int ButtonPress = 1;

int fd;

void PressedButton(void) {
			  ButtonPress = ButtonPress * (-1);
}

void MPU6050_Init(){
	wiringPiI2CWriteReg8 (fd, 0x19, 0x07);	//write to sample rate register
	wiringPiI2CWriteReg8 (fd, 0x6B, 0x01);	//write to power management register
	wiringPiI2CWriteReg8 (fd, 0x1A, 0);	//write to Configuration register
	wiringPiI2CWriteReg8 (fd, 0x1B, 24);	//write to Gyro configuration register
	wiringPiI2CWriteReg8 (fd, 0x38, 0x01);	//write to interrupt enable register
	}

short read_raw_data(int addr){
				short high_byte,low_byte,value;
				high_byte = wiringPiI2CReadReg8(fd, addr);
				low_byte = wiringPiI2CReadReg8(fd, addr+1);
				value = (high_byte << 8) | low_byte;
				return value;
			     }
int main(){
		float Gyro_x,Gyro_y,Gyro_z;
		float Gx=0,Gy=0,Gz=0;
		float AbsGx=0,AbsGy=0,AbsGz=0;
		int Position_BL=13,Position_BR=13,Position_FL=13,Position_FR=13;
		int TrainingCounter = 1;
		int ForwardNumber = 0,BackNumber = 0,LeftNumber = 0,RightNumber = 0,CentreNumber1 = 0,CentreNumber2 = 0;

		fd = wiringPiI2CSetup(Device_Address); //starts I2c with device address
		MPU6050_Init();	//starts MPU6050

		wiringPiSetupGpio();
		pinMode(Front_LED,OUTPUT);
		pinMode(Back_LED,OUTPUT);
		pinMode(Left_LED,OUTPUT);
		pinMode(Right_LED,OUTPUT);
		pinMode(Button,INPUT);
		pullUpDnControl (Button,PUD_UP);

		softPwmCreate(Front_Left,13,100);//set up front left tendon
		softPwmCreate(Front_Right,13,100);//set up front right tendon
		softPwmCreate(Back_Left,13,100);//set up back left tendon
		softPwmCreate(Back_Right,13,100);//Set up back right tendon

//		wiringPiISR (Button, INT_EDGE_FALLING, &PressedButton);


	while(1){
		//read raw accelerometer and gyroscope from MPU6050
		if(digitalRead(Button) == LOW){
				  PressedButton();
				  printf("Button Pressed\n");
				}

		Gyro_x = read_raw_data(0x43);
		Gyro_y = read_raw_data(0x45);
		Gyro_z = read_raw_data(0x47);

		Gx = Gyro_x/131;
		Gy = Gyro_y/131;
		Gz = Gyro_z/131;

		printf("\n Gx=%.3f Degrees/s\tGy=%.3f Degrees/s\tGz=%.3f Degrees/s\n ",Gx,Gy,Gz);
		printf("\n Motor Positions: FL=%d FR=%d BL=%d BR=%d",Position_FL,Position_FR,Position_BL,Position_BR);

		AbsGx  = fabs(Gx);
		AbsGy  = fabs(Gy);
		AbsGz  = fabs(Gz);

		//printf("\n AbsGx=%.3f \tAbsGy=%.3f \tAbsGz=%.3f \n",AbsGx,AbsGy,AbsGz);

		if (ButtonPress < 0) {	//If the button is pressed the sytem starts training mode
					if (TrainingCounter == 1){	//Go Forwards
									digitalWrite(Front_LED,HIGH);
									if (ForwardNumber > 4){	RightNumber = 0;
												CentreNumber1 = 0;
                                                                                                digitalWrite(Back_LED,HIGH);
                                                                                                digitalWrite(Left_LED,HIGH);
                                                                                                digitalWrite(Right_LED,HIGH);

												if(CentreNumber2 > 4) { digitalWrite(Front_LED,LOW);
															digitalWrite(Left_LED,LOW);
															digitalWrite(Back_LED,LOW);
															digitalWrite(Right_LED,LOW);
															TrainingCounter = TrainingCounter + 1;}
									}
					}else if (TrainingCounter == 2){//Go Left
									digitalWrite(Left_LED,HIGH);
									if (LeftNumber > 4){    digitalWrite(Back_LED,HIGH);
												digitalWrite(Front_LED,HIGH);
												digitalWrite(Right_LED,HIGH);
                                                                                                BackNumber = 0;
												CentreNumber2 = 0;

												if(CentreNumber1 > 4) { digitalWrite(Front_LED,LOW);
                                                                                                                        digitalWrite(Left_LED,LOW);
                                                                                                                        digitalWrite(Back_LED,LOW);
                                                                                                                        digitalWrite(Right_LED,LOW);
                                                                                                                        TrainingCounter = TrainingCounter + 1;}
									}
					}else if (TrainingCounter == 3){//Go Right
									digitalWrite(Right_LED,HIGH);
									if (RightNumber > 4){   digitalWrite(Left_LED,HIGH);
												digitalWrite(Front_LED,HIGH);
												digitalWrite(Back_LED,HIGH);
                                                                                                CentreNumber1 = 0;
                                                                                                ForwardNumber = 0;

												if(CentreNumber2 > 4) { digitalWrite(Front_LED,LOW);
                                                                                                                        digitalWrite(Left_LED,LOW);
                                                                                                                        digitalWrite(Back_LED,LOW);
                                                                                                                        digitalWrite(Right_LED,LOW);
                                                                                                                        TrainingCounter = TrainingCounter + 1;}
                                                                        }

					}else if (TrainingCounter ==4){ //Go Back
									digitalWrite(Back_LED,HIGH);
									if (BackNumber > 4){    digitalWrite(Left_LED,HIGH);
												digitalWrite(Right_LED,HIGH);
												digitalWrite(Front_LED,HIGH);
												LeftNumber = 0;
												CentreNumber2 = 0;

												if(CentreNumber1 > 4) { digitalWrite(Front_LED,LOW);
                                                                                                                        digitalWrite(Left_LED,LOW);
                                                                                                                        digitalWrite(Back_LED,LOW);
                                                                                                                        digitalWrite(Right_LED,LOW);
                                                                                                                        TrainingCounter = TrainingCounter - 3;}

                                                                        }

					}
		}

	 	if (AbsGy > AbsGx && AbsGy > AbsGz){
							    if(AbsGy > 1.7){ //Check forward and backwards movement
										if(Gy > 0){ //forwards
											if (Position_FL < 23){
											   			Position_FL = Position_FL + 2;}
											if (Position_BL < 23){
											   			Position_BL = Position_BL + 2;}
											if (Position_FR > 2){
											   			Position_FR = Position_FL - 2;}
											if (Position_BR  > 2){
											   			Position_BR = Position_BL - 2;}
											if (ButtonPress > 0){
											   digitalWrite(Front_LED,HIGH);}
											if (ButtonPress < 0){
												if(TrainingCounter == 1){
												ForwardNumber = ForwardNumber + 1;}
												if(TrainingCounter == 4){
												CentreNumber1 = CentreNumber1 + 1;}

											 //Moving Foot Forward
											   softPwmWrite(Front_Left,Position_FL);
											   softPwmWrite(Back_Left,Position_BL);
											   softPwmWrite(Front_Right,Position_FR);
											   softPwmWrite(Back_Right,Position_BR);
											   delay(250);
											  }
										}else if (Gy < 0) { //Backwards
												if (Position_FL > 2 ){
												  		Position_FL = Position_FL - 2;}
												if (Position_BL > 2){
												  		Position_BL = Position_BL - 2;}
												if (Position_FR < 23){
												  		Position_FR = Position_FR + 2;}
												if (Position_BR < 23){
												  		Position_BR = Position_BR + 2;}
												if (ButtonPress > 0){
												  digitalWrite(Back_LED,HIGH);}

	                                                                                        if (ButtonPress < 0){
													if(TrainingCounter == 4){
                                                                                                				BackNumber = BackNumber + 1;}
													if(TrainingCounter == 1){
													CentreNumber2 = CentreNumber2 + 1;}

												}
												 //Moving Foot Backwards
												  softPwmWrite(Front_Left,Position_FL);
												  softPwmWrite(Back_Left,Position_BL);
												  softPwmWrite(Front_Right,Position_FR);
												  softPwmWrite(Back_Right,Position_BR);
												  delay(250);

										}
								}
						   }
		else if (AbsGz > AbsGx && AbsGz > AbsGy){
							    if(AbsGz > 1.7){//Check Left and Right Movement
										if(Gz < 0){
											if(Position_FL > 2){
											   		Position_FL = Position_FL - 2;}
											if(Position_BL < 23){
											   		Position_BL = Position_BL + 2;}
											if(Position_FR < 23){
											   		Position_FR = Position_FR + 2;}
											if(Position_BR > 2){
											   		Position_BR = Position_BR - 2;}
											if (ButtonPress > 0){
											   digitalWrite(Left_LED,HIGH);}

                                                                                        if (ButtonPress < 0){
												if(TrainingCounter == 2){
                                                                                            		LeftNumber = LeftNumber + 1;}
												if(TrainingCounter == 3){
												CentreNumber2 = CentreNumber2 + 1;}

											 //Moving Foot Left
											   softPwmWrite(Front_Left,Position_FL);
											   softPwmWrite(Back_Left,Position_BL);
											   softPwmWrite(Front_Right,Position_FR);
											   softPwmWrite(Back_Right,Position_BR);
											   delay(250);
											  }
										}else if (Gy > 0) {
												if(Position_FL > 2){
												  Position_FL = Position_FL - 2;}
												if(Position_BL < 23){
												  Position_BL = Position_BL + 2;}
												if(Position_FR < 23){
												  Position_FR = Position_FR + 2;}
												if(Position_BR > 2){
												  Position_BR = Position_BR - 2;}

												if(ButtonPress > 0){
												  digitalWrite(Right_LED,HIGH);}

	                                                                                        if (ButtonPress < 0){
													if(TrainingCounter == 3){
        	                                                                                        RightNumber = RightNumber + 1;}
													if(TrainingCounter == 2){
													CentreNumber1 = CentreNumber1 + 1;}
												}

												//Moving Foot Right
												  softPwmWrite(Front_Left,Position_FL);
												  softPwmWrite(Back_Left,Position_BL);
												  softPwmWrite(Front_Right,Position_FR);
												  softPwmWrite(Back_Right,Position_BR);
												  delay(250);
												 }
							}
				}
		delay(250);
	if(ButtonPress > 0){
		digitalWrite(Front_LED,LOW);
		digitalWrite(Back_LED,LOW);
		digitalWrite(Left_LED,LOW);
		digitalWrite(Right_LED,LOW);}
		}
	return 0;
}
