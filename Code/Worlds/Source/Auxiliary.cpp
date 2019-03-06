#pragma once
#include "../Headers/Auxiliary.h"

int Auxiliary::intakeTask()
{
    while (true)
    {
        // Controls intake
        if (Controller.ButtonL1.pressing())
            intake.spin(directionType::fwd, 100, velocityUnits::pct);
        else if(Controller.ButtonL2.pressing())
            intake.spin(directionType::rev, 100, velocityUnits::pct);
        else
            intake.stop(brakeType::hold);

        task::sleep(50);
    }
    return 0;
}

int Auxiliary::puncherTask()
{
  while(true)
  {
    if (Controller.ButtonX.pressing())
        puncher.spin(directionType::fwd, 100, velocityUnits::pct);
    else if(Controller.ButtonB.pressing())
        doubleShot(2450, 2260); // Temporary, replace with variables later
    else
        puncher.stop(brakeType::coast);

    if (Controller.ButtonY.pressing())
      angler.spin(directionType::fwd, 50, velocityUnits::pct);
    else if (Controller.ButtonA.pressing())
      angler.spin(directionType::fwd, -50, velocityUnits::pct);
    else
      angler.stop(brakeType::hold);
  }
  return 1;
}
int Auxiliary::setAngle(int angle)
{
  Angler.setStopping(brakeType::hold);
  double P = 0, kp = 0.1;
  double I = 0, ki = 0;
  double D = 0, kd = 0;
  double error = 100, lError = 100;
  double anglePower = 0;

  while(fabs(error) > 10 && fabs(lError) > 10)
  {
      error = -potenVal + poten.value(analogUnits::range12bit);
      P = error * kp;

      Angler.spin(directionType::fwd, P, velocityUnits::pct);
      task::sleep(50);
  }

  Angler.stop();
}
