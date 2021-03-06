#pragma once;
#include "vars.cpp"
#include "../cleanConfig.h"
#include "../Headers/Drivetrain.h"
#include "Drivetrain.cpp"
#include "auton.cpp"

using namespace vex;

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


int driveTask()
{
    int leftSide = 0, rightSide = 0;
    while(true)
    {
        leftSide = 0;
        rightSide = 0;

        if (Controller.ButtonR1.pressing())
            rightSide = leftSide = 50;
        else if (Controller.ButtonR2.pressing())
            rightSide = leftSide = -50;

        if (abs(Controller.Axis3.value()) > 10)
            leftSide = Controller.Axis3.value();
        if (abs(Controller.Axis2.value()) > 10)
            rightSide = Controller.Axis2.value();

        FrontLeft.spin(directionType::fwd, leftSide, velocityUnits::pct);
        BackLeft.spin(directionType::fwd, leftSide, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, rightSide, velocityUnits::pct);
        BackRight.spin(directionType::fwd, rightSide, velocityUnits::pct);

        task::sleep(20);
    }
    return 0;
}



int main()
{
    gyroscope.startCalibration(137);//133);
    invertedGyro.startCalibration(137);//136);
    task::sleep(6000);
    FrontRight.resetRotation();
    Scraper.resetRotation();
    int cataTorque = 0;
    /*task taskCatapult(cataTask, 1);
    task taskIntake(intakeTask, 1);
    task taskDrive(driveTask, 1);*/
    Brain.resetTimer();
    //drive.drivePID(-35, 80, 7, 7, 1500, 0);
     //drive.turnToSlow(90);
    //firstFrontAuton(drive);
    drive.slipAdjust(false, true);
    turnRight(45);
    //drive.turnTo(90, 100);
    int time = Brain.timer(timeUnits::msec);
    Brain.Screen.clearScreen();
    while (true)
    {
       Brain.Screen.printAt(30, 30, "%d             ", gyroscope.value(analogUnits::range12bit));
       Brain.Screen.printAt(30, 60, "%d             ", invertedGyro.value(analogUnits::range12bit));
       Brain.Screen.printAt(30, 90, "%d           ", time);
      task::sleep(69);
    }
}
