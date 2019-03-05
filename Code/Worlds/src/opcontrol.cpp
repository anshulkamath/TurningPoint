#include "../include/main.h"
#include "../MainCode/headers/autonomous/AutonSelector.h"
#include "../MainCode/headers/Sensors/Gyroscope.h"
#include "../MainCode/headers/Robot/DriveTrain.h"
#include "../MainCode/headers/Sensors/LineSensor.h"
#include "../MainCode/source/Robot/DriveTrain.cpp"
#include "../MainCode/headers/Robot/Intake.h"
#include "../MainCode/headers/Robot/Puncher.h"
#include "../MainCode/headers/Robot/Angler.h"

#define FRONTLEFT 2
#define FRONTRIGHT 3
#define BACKLEFT 7
#define BACKRIGHT 6
#define INTAKEMOT 1
#define ANGLER 5
#define PUNCHER 10
#define potent = 1

void driveTask1(void * t)
{
	Drivetrain x = *(Drivetrain*)(t);
	x.driveTask();

}

void puncherTask(void * t)
{
	Puncher x = *(Puncher*)(t);
	x.puncherDriveTask();
}


void opcontrol()
{
		//displaySide();
		std::vector<std::pair<double, double>> angles;

		Drivetrain train(pros::Motor(FRONTRIGHT), pros::Motor(FRONTLEFT), pros::Motor(BACKRIGHT), pros::Motor(BACKLEFT), Gyroscope(pros::ADIGyro(5), pros::ADIGyro(6)));
LineSensor d(ADILineSensor(6));
LineSensor e(ADILineSensor(7));
		Intake inta(pros::Motor(INTAKEMOT), d, e);
		Puncher pun(pros::Motor(PUNCHER), Angler(pros::Motor(ANGLER), ADIAnalogIn(potent)), inta,d, ADIDigitalIn(3) );
		//std::string x = "";
//driveTask1(&train);
		
		pros::Task taskDrive(driveTask1, &train);
		pros::Task taskP(puncherTask, &pun);
		while(true);

		/*
		pros::Motor FrontLeft(1), BackLeft(2), FrontRight(3), BackRight(4);
		pros::Motor intake(5);

		pros::Controller cont (E_CONTROLLER_MASTER);
		while(true)
		{
			int leftSide = cont.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
			int rightSide = cont.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);
			FrontLeft.move(leftSide);
			FrontRight.move(rightSide);
			BackRight.move(rightSide);
			BackLeft.move(leftSide);
			delay(10);

			if(cont.get_digital( E_CONTROLLER_DIGITAL_L1))
			{
				intake.move_velocity(100);
			}
			else
			{
				if(cont.get_digital(E_CONTROLLER_DIGITAL_L2))
					intake.move_velocity(-100);
				else
					intake.move_velocity(0);
			}
			pros::delay(10);
		}*/
}
