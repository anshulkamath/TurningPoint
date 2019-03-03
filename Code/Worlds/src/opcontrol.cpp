#include "main.h"
#include "../MainCode/headers/autonomous/AutonSelector.h"
#include "../MainCode/headers/Sensors/Gyroscope.h"
#include "../MainCode/headers/Robot/DriveTrain.h"

using namespace pros;

void opcontrol()
{
	/*ADILineSensor line(1);
	line.calibrate();
	lcd::print(1, "HHEE");
	Task::delay(1000);
	while (true) {
		lcd::clear_line(1);
		lcd::print(1, "%d", line.get_value_calibrated());
		//printf("%d", line.get_value());
		delay(50);
	}*/
		//displaySide();

		Drivetrain train(Motor(1), Motor(2), Motor(3), Motor(4), Gyroscope(ADIGyro(5), ADIGyro(6)));
		std::string x = "Argument";
		task_t driveTask = c::task_create(train.driveTask(), NULL, TASK_PRIORITY_MAX, TASK_STACK_DEPTH_DEFAULT, "Drive Task");

}
