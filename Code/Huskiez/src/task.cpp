#pragma once

#include "../headers/Drivetrain.h"
#include "../include/cleanConfig.h"
#include "vars.cpp"

int intakeTask()
{
    Intake.setStopping(brakeType::coast);
    int intakePower = 0;
    while(true)
    {
        if (Controller.ButtonL1.pressing())
            intakePower = 100;
        else if (Controller.ButtonL2.pressing())
            intakePower = -100;
        else
            intakePower = 0;

        Intake.spin(directionType::fwd, intakePower, velocityUnits::pct);
        task::sleep(50);
    }
}

void resetCata()
{
  // Cata Variables
  int cataPower = 0;
  int error = 0;
  int lError = 0;
  int thresh = 4;
  double kp = 0.25;
  double kd = 0.1;

  do
  {
    error = CataPot.value(analogUnits::range12bit) - cataDown;
    cataPower = kp * error + kd * (error - lError);

    CataL.spin(directionType::fwd, cataPower, velocityUnits::pct);
    CataR.spin(directionType::fwd, cataPower, velocityUnits::pct);

    lError = error;
    task::sleep(20);
  }
  while (abs(error) > thresh);

  CataL.spin(directionType::fwd, 0, velocityUnits::pct);
  CataR.spin(directionType::fwd, 0, velocityUnits::pct);
}

int cataTask()
{
    CataL.setStopping(brakeType::hold);
    CataR.setStopping(brakeType::hold);

    resetCata();

    while(true)
    {
        if (Controller.ButtonX.pressing() || fire)
        {
            fire = false;
            CataL.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct, false);
            CataR.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct, true);

            resetCata();
        }

        // If catapult is not in position, set cataPower based on position
        if (abs(CataPot.value(analogUnits::range12bit) - cataDown) > 1)
            cataReady = false;
        else // If the catapult is in position, broadcast ready and stop
            cataReady = true;

        CataL.spin(directionType::fwd, 0, velocityUnits::pct);
        CataR.spin(directionType::fwd, 0, velocityUnits::pct);

        task::sleep(20);
    }
    return 0;
}

int leftSide = 0, rightSide = 0;
int driveTask()
{
    const int accelCap = 30;
    // int splitLeft = 0, splitRight = 0;
    int prevLeft = 0, prevRight = 0; // To maintain acceleration
    // int dLeft = 0, dRight = 0;
    while(true)
    {
        leftSide = 0;
        rightSide = 0;

        /*
        // Split Drive Controls
        leftSide = Controller.Axis2.value() + Controller.Axis4.value();
        rightSide = Controller.Axis2.value() - Controller.Axis4.value();
        */

        if (Controller.ButtonR1.pressing())
        {
          rightSide = leftSide = 70;
          if (abs(prevLeft - leftSide) > accelCap)
            rightSide = leftSide = (prevLeft + accelCap);
        }
        else if (Controller.ButtonR2.pressing())
        {
          rightSide = leftSide = -70;
          if (abs(prevLeft - leftSide) > accelCap)
            rightSide = leftSide = (prevLeft - accelCap);
        }

        // Tank Drive Controls
        if (abs(Controller.Axis3.value()) > 10)
        {
            leftSide = 127 * pow(fabs((double)Controller.Axis3.value())/127.0, 9/7) * sgn(Controller.Axis3.value());
            if(leftSide - prevLeft > accelCap) leftSide = prevLeft + accelCap;

            if(leftSide - prevLeft < -accelCap) leftSide = prevLeft - accelCap;
        }
        if (abs(Controller.Axis2.value()) > 10)
        {
          if(rightSide - prevLeft > accelCap) rightSide = prevRight + accelCap;

          if(rightSide - prevLeft < -accelCap) rightSide = prevRight - accelCap;
          
            rightSide = 127 * pow(fabs((double)Controller.Axis2.value())/127.0, 9/7) * sgn(Controller.Axis2.value());
        }
        /*
        // Acceleration control
        if (leftSide != 0 && abs(dLeft) > accelCap)
          leftSide = prevLeft + sgn(dLeft) * accelCap;
        else if (rightSide != 0 && abs(dRight) > accelCap)
          rightSide = prevRight + sgn(dRight) * accelCap;
          */


        FrontLeft.spin(directionType::fwd, (int)leftSide, velocityUnits::pct);
        BackLeft.spin(directionType::fwd, (int)leftSide, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, (int)rightSide, velocityUnits::pct);
        BackRight.spin(directionType::fwd, (int)rightSide, velocityUnits::pct);

        prevLeft = BackLeft.velocity(velocityUnits::pct);
        prevRight = BackRight.velocity(velocityUnits::pct);
        task::sleep(20);
    }
    return 0;
}

int scraperTask()
{
  Scraper.setStopping(brakeType::hold);
  int scraperPower = 0;
  while (true)
  {
    if (Controller.ButtonUp.pressing())
      scraperPower = 100;
    else if (Controller.ButtonDown.pressing())
      scraperPower = -100;
    else
      scraperPower = 0;

    Scraper.spin(directionType::fwd, scraperPower, velocityUnits::pct);
    task::sleep(50);
  }
  return 0;
}
