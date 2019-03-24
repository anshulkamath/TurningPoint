#pragma once

#include "vars.cpp"
#include "../cleanConfig.h"
#include "../Headers/Drivetrain.h"
#include "Drivetrain.cpp"

using namespace vex;

Drivetrain drive(FrontRight, FrontLeft, BackRight, BackLeft, gyroscope, invertedGyro);

int sideSelect()
{
  Brain.Screen.clearScreen();
  Brain.Screen.setFillColor(vex::color::red);
  Brain.Screen.drawRectangle(1,1,200,200);
  Brain.Screen.printAt(40, 40, false, "RED");
  Brain.Screen.setFillColor(vex::color::blue);
  Brain.Screen.drawRectangle(202,1,200,200);
  Brain.Screen.printAt(242, 40, false, "BLUE");
  while(!Brain.Screen.pressing());
  while(Brain.Screen.pressing());
    Brain.Screen.clearScreen();
  pressed = false;
  if(Brain.Screen.xPosition() < 200)
  {
    side = "RED";
  }else if(Brain.Screen.xPosition() > 200)
  {
    side = "BLUE";
  }
  Brain.Screen.clearScreen();
  Brain.Screen.setFillColor(vex::color::yellow);
  Brain.Screen.printAt(1,40,"Hello");
  Brain.Screen.drawRectangle(1,1,200,200);
  Brain.Screen.setFillColor(vex::color::white);
  Brain.Screen.drawRectangle(202,1,200,200);

  while(!Brain.Screen.pressing());
  while(Brain.Screen.pressing());
    Brain.Screen.clearScreen();
  if(Brain.Screen.xPosition() < 200)
  {
    autonNum = 1;
  }else if(Brain.Screen.xPosition() > 200)
  {
    autonNum = 2;
  }
  Brain.Screen.clearScreen();
  Brain.Screen.setPenColor(vex::color::red);
  Brain.Screen.setFillColor(vex::color::black);
  Brain.Screen.printAt(1,40, side.c_str());
  Brain.Screen.printAt(1, 80, "%d", autonNum);
  return 0;
}


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
  int thresh = 1;
  double kp = 0.25;

  do
  {
    error = CataPot.value(analogUnits::range12bit) - cataDown;
    cataPower = error * kp;

    CataL.spin(directionType::fwd, cataPower, velocityUnits::pct);
    CataR.spin(directionType::fwd, cataPower, velocityUnits::pct);

    task::sleep(20);
  }
  while (abs(error) < thresh);

  CataL.stop(brakeType::brake);
  CataR.stop(brakeType::brake);
}

int cataTask()
{
    CataL.setStopping(brakeType::brake);
    CataR.setStopping(brakeType::brake);

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

        task::sleep(20);
    }
    return 0;
}

int leftSide = 0, rightSide = 0;
int driveTask()
{
    const int accelCap = 10;
    int splitLeft = 0, splitRight = 0;
    int prevLeft = 0, prevRight = 0; // To maintain acceleration
    int dLeft = 0, dRight = 0;
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
          rightSide = leftSide = 50;
        else if (Controller.ButtonR2.pressing())
          rightSide = leftSide = -50;


        // Tank Drive Controls
        if (abs(Controller.Axis3.value()) > 10)
            leftSide = 127 * pow(abs((double)Controller.Axis3.value())/127.0, 9/7) * sgn(Controller.Axis3.value());
        if (abs(Controller.Axis2.value()) > 10)
            rightSide = 127 * pow(abs((double)Controller.Axis2.value())/127.0, 9/7) * sgn(Controller.Axis2.value());

        dLeft = leftSide - prevLeft;
        dRight = rightSide - prevRight;

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

        prevLeft = leftSide;
        prevRight = rightSide;
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

int main()
{
    task taskCatapult(cataTask, 1);
    task taskIntake(intakeTask, 1);
    task taskDrive(driveTask, 1);
    task taskScraper(scraperTask, 1);
    while (true)
    {
        Brain.Screen.printAt(0, 30, "Potentiometer: %d", CataPot.value(analogUnits::range12bit));

        task::sleep(20);
    }
}
