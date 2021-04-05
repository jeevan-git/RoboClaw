/*
 * roboclaw-test example for roboclaw library
 *
 * Copyright 2018 (C) Bartosz Meglicki <meglickib@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

/*
 *  This example:
 *  -initializes communication with roboclaw unit
 *  -reads & prints battery voltage
 *  -in a loop
 *    - reads duty cycle from user [-100, 100]%
 *    - sets this duty cycle to motors
 *    - breaks the loop on non numeric input
 *  -stops the motors
 *  -cleans after library
 *
 *  Program expects terminal device, baudrate and roboclaw address (from 0x80 to 0x87, ignored in USB mode) e.g.
 *
 *  roboclaw-test /dev/ttyACM0 38400 0x80
 *
 */

#include "../include/roboclaw.h"

#include <stdio.h>  //printf
#include <stdlib.h> //exit
#include <unistd.h> //sleep
#include <iostream>

using namespace std;
void usage(char **argv);
void informative_terminate(struct roboclaw  *rc);

int main(int argc, char **argv)
{
	struct roboclaw *rc;
	uint8_t address=0x82;
	int16_t voltage;
	float voltage_float;
	int baudrate, duty_cycle, motor_speed;
	cout<<"The value of the argc is: "<<argc<<endl;
	if(argc != 4)
	{
		usage(argv);
		return 0;
	}

	baudrate = (int)strtol(argv[2], NULL, 10);
	address = (uint8_t)strtol(argv[3], NULL, 0);

	//initialize at supplied terminal at specified baudrate
	rc=roboclaw_init(argv[1], baudrate);
	cout<<"The value of argc :"<<argv[1]<<endl;

	if( rc == NULL )
	{
		perror("unable to initialize roboclaw");
		exit(1);
	}

	cout<<"initialized communication with roboclaw\n"<<endl;

	//read the battery voltage
	if(roboclaw_main_battery_voltage(rc, address, &voltage) != ROBOCLAW_OK)
		informative_terminate(rc);

	voltage_float = (float)voltage/10.0f;
	cout<<"battery voltage is : "<<voltage_float<<"V"<<endl;

	cout<<"WARNING - make sure it is safe for the motors to be moved\n"<<endl;

	while(1)
	{
		cout<<"Enter motor speed [-100, 100] or 'q'  to quit"<<endl;

		// if( scanf("%d", &duty_cycle) < 1 )
		// 	break;
		// if( duty_cycle > 100 )
		// 	duty_cycle = 100;
		// if( duty_cycle < -100 )
		// 	duty_cycle = -100;

		// //32767 is max duty cycle setpoint that roboclaw accepts
		// duty_cycle = (float)duty_cycle/100.0f * 32767;
		// // Sending the pwm signals to the motor
		// int test = roboclaw_duty_m1m2(rc, address, duty_cycle, duty_cycle);

		// if(test != ROBOCLAW_OK )
		// {
		// 	fprintf(stderr, "problem communicating with roboclaw, terminating\n");
		// 	break;
		// }

			if(scanf("%d", &motor_speed)<1)
				break;
			if(motor_speed>100)
				motor_speed=100;
			if(motor_speed<-100)
				motor_speed=-100;

			// 3500 is the max limit set for now 
			motor_speed=(float)motor_speed/100.0f*3500;
			// Sending the speed to the motor
			roboclaw_speed_m1m2(rc, address, motor_speed, motor_speed);
	}

	//make sure the motors are stopped before leaving
	cout<<"stopping the motors..\n";
	roboclaw_duty_m1m2(rc, address, 0, 0);

	if(roboclaw_close(rc) != ROBOCLAW_OK)
		perror("unable to shutdown roboclaw cleanly");

	cout<<"bye...\n";
	return 0;
}

void usage(char **argv)
{
	cout<<"Usage:\n";
	cout<<argv[0]<< " terminal_device baudrate address\n\n";
	cout <<"examples:\n";
	cout<<argv[0]<< " /dev/ttyACM0 38400 0x80\n";
	cout<<argv[0]<< " /dev/ttyAMA0 460800 0x81\n";
	cout<<argv[0]<< " /dev/tty_int1 11520 0x82\n";

}

void informative_terminate(struct roboclaw *rc)
{
	fprintf(stderr, "problem communicating with roboclaw\n");
	fprintf(stderr, "make sure you are:\n");
	fprintf(stderr, "-using correct tty device\n");
	fprintf(stderr, "-using correct address\n");
	fprintf(stderr, "-set correct roboclaw baudrate\n");
	fprintf(stderr, "-wired things correctly\n");
	fprintf(stderr, "terminating...\n");
	roboclaw_close(rc);
	exit(1);
}
