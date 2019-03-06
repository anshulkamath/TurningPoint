#pragma once
#include "../../../include/api.h"
#include "../Sensors/Linesensor.h"

class Intake
{
  pros::Motor intake;
  LineSensor line, puncherSensor;
  bool inUse = false;
public:
  int intakeMode = 0;

  Intake(pros::Motor inty, LineSensor liney, LineSensor puncherSensor1)
  : intake(inty), line(liney), puncherSensor(puncherSensor1) {}

  void intakeTask(void * params);

  void runIntake(int state);

};
