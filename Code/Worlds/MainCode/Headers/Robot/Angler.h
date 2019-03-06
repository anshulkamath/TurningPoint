#pragma once

#include "../../../include/api.h"
#include "../Sensors/lineSensor.h"

class Angler
{
  private:
    Motor angler;
    ADIAnalogIn pot;

  public:
    Angler(pros::Motor angle, ADIAnalogIn poten) : angler(angle), pot(poten) {}

    bool setAngle(double angle);
};
