#include "../include/main.h"
#include "../MainCode/headers/autonomous/AutonSelector.h"
#include "../MainCode/headers/Sensors/Gyroscope.h"
#include "../MainCode/headers/Robot/DriveTrain.h"
#include "../MainCode/headers/Sensors/LineSensor.h"

void opcontrol()
{
		//displaySide();
		//Drivetrain train(pros::Motor(1), pros::Motor(2), pros::Motor(3), pros::Motor(4), Gyroscope(pros::ADIGyro(5), pros::ADIGyro(6)));
		//std::string x = "";
		//pros::Task task(train.driveTask, &x);
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
		}
}
