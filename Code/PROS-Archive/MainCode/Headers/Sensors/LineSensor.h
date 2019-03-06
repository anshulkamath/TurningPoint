#pragma once
#include "../../../include/api.h"

using namespace pros;

class LineSensor
{
  int threshHold = 220;
  pros::ADILineSensor Line;
 public:
  LineSensor(pros::ADILineSensor lineArg) : Line(lineArg) {}

  void calibrateLine();
  bool isBall();
};
