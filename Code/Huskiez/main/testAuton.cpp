#pragma once
#include "../src/vars.cpp"
#include "../include/cleanConfig.h"
#include "../headers/Drivetrain.h"
#include "../src/task.cpp"
#include "../src/auton.cpp"

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
  while(!Brain.Screen.pressing()) {}
  while(Brain.Screen.pressing()) {}
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

  while(!Brain.Screen.pressing()) {}
  while(Brain.Screen.pressing()) {}
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
    gyroscope.startCalibration(150);
    invertedGyro.startCalibration(150);
    gyroTest.startCalibration();
    task::sleep(6000);
    FrontRight.resetRotation();
    Scraper.resetRotation();
    int cataTorque = 0;
    /*task taskCatapult(cataTask, 1);
    task taskIntake(intakeTask, 1);
    task taskDrive(driveTask, 1);*/
    Brain.resetTimer();
    int time = Brain.timer(timeUnits::msec);
  //drive.drivePID(-35, 80, 7, 7, 1500, 0);
     //drive.turnToSlow(90);
    firstFrontAuton(drive);
    //drive.turnTo(90, 100);
    Brain.Screen.clearScreen();
    while (true)
    {
       Brain.Screen.printAt(30, 30, "%.2f", getAngle());
        Brain.Screen.printAt(30, 60, "Time: %d", time);
        /*if (CataL.torque(torqueUnits::Nm) > cataTorque)
            //cataTorque = CataL.torque(torqueUnits::Nm);
        Brain.Screen.printAt(0, 30, "Temperature: %.2f", CataL.temperature());
        Brain.Screen.printAt(0, 60, "Torque: %.2f", cataTorque);

        Brain.Screen.printAt(0, 90, "Potentiometer: %d", CataPot.value(analogUnits::range12bit));*/

        task::sleep(10);
    }
}
