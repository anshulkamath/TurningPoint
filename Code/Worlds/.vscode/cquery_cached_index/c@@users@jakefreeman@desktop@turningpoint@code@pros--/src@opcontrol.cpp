#include "main.h"
#include "../MainCode/headers/autonomous/AutonSelector.h"
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
		displaySide();
}
