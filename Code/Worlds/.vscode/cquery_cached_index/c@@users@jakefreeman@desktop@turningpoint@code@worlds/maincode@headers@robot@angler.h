#pragma once

#include "../../../include/api.h"
#include "../Sensors/lineSensor.h"

class Angler
{
  private:
    pros::Motor angler;
    ADIAnalogIn pot;

  public:
    Angler(pros::Motor angle, pros::ADIAnalogIn poten) : angler(angle), pot(poten) {}

    bool angle(double angle)
    {
      double P = 0, kp = .82;
      double error = 100, lError = 100;
      double anglePower = 0;
      angler.set_brake_mode(E_MOTOR_BRAKE_HOLD);
      while(fabs(error) > 10 && fabs(lError) > 10)
      {
        error = angle - pot.get_value();
        P = error * kp;

        angler.move_velocity(P);
        delay(50);
      }
      angler.move(0);
      return true;
    }
};
