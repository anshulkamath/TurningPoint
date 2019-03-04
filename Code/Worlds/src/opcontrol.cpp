#include "../include/main.h"
#include "../MainCode/headers/autonomous/AutonSelector.h"
#include "../MainCode/headers/Sensors/Gyroscope.h"
#include "../MainCode/headers/Robot/DriveTrain.h"
#include "../MainCode/headers/Sensors/LineSensor.h"

void driveTask(Drivetrain& train)
{
  pros::Controller cont (E_CONTROLLER_MASTER);
  int leftSide = 0;
  int rightSide = 0;
  double turnLimiter = 1;
  while(true)
  {
    // Introducing deadzone
    if (E_CONTROLLER_ANALOG_LEFT_Y > 10)
      leftSide = cont.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
    if (E_CONTROLLER_ANALOG_RIGHT_Y > 10)
      rightSide = cont.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);
    else
    {
      leftSide = 0;
      rightSide = 0;
    }

    // Limiting turns
    if (abs(abs(rightSide) - abs(leftSide)) > 50)
    {
      rightSide *= turnLimiter;
      leftSide *= turnLimiter;
    }

    // Slow drive mode
    if (E_CONTROLLER_DIGITAL_R1)
      rightSide = leftSide = 60 * 127/100;
    else if (E_CONTROLLER_DIGITAL_R2)
      rightSide = leftSide = -60 * 127/100;


    // Brake mode
    if (isDriving())
      train.setBrakeMode(E_MOTOR_BRAKE_COAST);
    else
      train.setBrakeMode(E_MOTOR_BRAKE_BRAKE);

    train.setDrive(rightSide, leftSide);
    delay(10);
  }
}

void opcontrol()
{
		//displaySide();
		Drivetrain train(pros::Motor(1), pros::Motor(2), pros::Motor(3), pros::Motor(4), Gyroscope(pros::ADIGyro(5), pros::ADIGyro(6)));
		//std::string x = "";
		pros::Task taskDrive(driveTask, train);

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
