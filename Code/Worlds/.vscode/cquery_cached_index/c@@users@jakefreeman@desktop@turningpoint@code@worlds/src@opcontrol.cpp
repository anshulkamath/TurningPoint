#include "main.h"
#include "../MainCode/headers/autonomous/AutonSelector.h"
#include "../MainCode/headers/Sensors/Gyroscope.h"
#include "../MainCode/headers/Robot/DriveTrain.h"
#include "../MainCode/headers/Sensors/LineSensor.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	/*pros::ADILineSensor line(1);
	line.calibrate();
	pros::lcd::print(1, "HHEE");
	pros::Task::delay(1000);
	while (true) {
		pros::lcd::clear_line(1);
		pros::lcd::print(1, "%d", line.get_value_calibrated());
		//printf("%d", line.get_value());
		pros::delay(50);
	}*/
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
			}else{

				if(cont.get_digital(E_CONTROLLER_DIGITAL_L2))
				{
					intake.move_velocity(-100);
				}else{
					intake.move_velocity(0);
				}
			}
			pros::delay(10);
		}
}
