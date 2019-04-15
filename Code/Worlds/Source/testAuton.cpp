#pragma once;
#include "vars.cpp"
#include "../cleanConfig.h"
#include "../Headers/Drivetrain.h"
#include "Drivetrain.cpp"
#include "auton.cpp"
#include "task.cpp"

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

int main()
{
    int gyroScale = 140;
    int invertGyroScale = 140;
    gyroscope.startCalibration(invertGyroScale);//133);
    invertedGyro.startCalibration(invertGyroScale);//136);
    task::sleep(6000);
    FrontRight.resetRotation();
    Scraper.resetRotation();
    int cataTorque = 0;
    task c(angleMonitor, 1);
  //  task d(wheelVelocity, 1);
    //task taskCatapult(cataTask, 1);
    /*
    task taskIntake(intakeTask, 1);
    task taskDrive(driveTask, 1);*/
    side = "RED";
    Brain.resetTimer();
  //  runIntake(-1, 100);
//    drive.drivePID(-38, 100, 100, 100, 1500, 0);
  //drive.setDrive(100, 0);
  //task::sleep(250);
  //drive.setDrive(0);
    //superMetaAuton(drive);
    superMetaAuton(drive);
    int time = Brain.timer(timeUnits::msec);
    Brain.Screen.clearScreen();
    while (true)
    {
       Brain.Screen.printAt(30, 30, "%d             ", gyroscope.value(analogUnits::range12bit));
       Brain.Screen.printAt(30, 60, "%d             ", invertedGyro.value(analogUnits::range12bit));
       Brain.Screen.printAt(30, 90, "%.2f           ", getAngle());
       Brain.Screen.printAt(30, 120, "%.2f            ", Scraper.rotation(rotationUnits::deg));
       Brain.Screen.printAt(30, 150,side.c_str());
       task::sleep(69);
    }
}
