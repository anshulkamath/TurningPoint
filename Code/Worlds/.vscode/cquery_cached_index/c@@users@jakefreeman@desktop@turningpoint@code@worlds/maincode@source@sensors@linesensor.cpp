#include "../../Headers/Sensors/LineSensor.h"

void LineSensor::calibrateLine()
{
	Line.calibrate();
	pros::lcd::print(1, "HHEE");
	pros::Task::delay(1000);
	while (true) {
		pros::lcd::clear_line(1);
		//pros::lcd::print(1, "%d", line.get_value_calibrated());
		//printf("%d", line.get_value());
		pros::delay(50);
	}
}

  bool LineSensor::isBall()
  {
    return (threshHold > Line.get_value());
  }
