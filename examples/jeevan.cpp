

#include "../include/roboclaw.h"
#include "../include/conio.h"

#include <stdio.h>  //printf
#include <stdlib.h> //exit
#include <unistd.h> //sleep
#include <iostream>

using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

void usage(char **argv);
void informative_terminate(struct roboclaw  *rc);

int main(int argc, char **argv)
{
	struct roboclaw *rc;
	uint8_t address=0x82;
	int16_t voltage;
	float voltage_float;
	int baudrate, duty_cycle, motor1_speed, motor2_speed, motor_control;
	int c=0;

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

        switch((c=getch())) {
        case KEY_UP:
            cout << endl << "Up" << endl;//key up
			motor1_speed=30;
			motor2_speed=30;			
            // break;
        case KEY_DOWN:
            cout << endl << "Down" << endl;   // key down
			motor1_speed=-30;
			motor2_speed=-30;
            // break;
        case KEY_LEFT:
            cout << endl << "Left" << endl;  // key left
			motor1_speed=-30;
			motor2_speed=30;		
            // break;
        case KEY_RIGHT:
            cout << endl << "Right" << endl;  // key right
			motor1_speed=30;
			motor2_speed=-30;	
            // break;
        default:
            cout << endl << "null" << endl;  // not arrow
			motor1_speed=0;
			motor2_speed=0;		
            break;
        }

		// cin>>motor_control;
		// if(motor_control==8){
		// 	motor1_speed=30;
		// 	motor2_speed=30;	
		// }
		// else if(motor_control==2){
		// 	motor1_speed=-30;
		// 	motor2_speed=-30;				
		// }
		// else if(motor_control==4){
		// 	motor1_speed=-30;
		// 	motor2_speed=30;				
		// }
		// else if(motor_control==6){
		// 	motor1_speed=30;
		// 	motor2_speed=-30;				
		// }
		// else{
		// 	motor1_speed=0;
		// 	motor2_speed=0;				
		// }
		// 3500 is the max limit set for now 
		motor1_speed=(float)motor1_speed/100.0f*3500;
		motor2_speed=(float)motor2_speed/100.0f*3500;
		// Sending the speed to the motor
		int test=roboclaw_speed_m1m2(rc, address, motor1_speed, motor2_speed);

		if(test != ROBOCLAW_OK )
		{
			fprintf(stderr, "problem communicating with roboclaw, terminating\n");
			break;
		}			
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
