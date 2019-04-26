#pragma once

#include "vars.cpp"
#include "../cleanConfig.h"
#include "../Headers/Drivetrain.h"
#include "Drivetrain.cpp"
#include "task.cpp"
#include "auton.cpp"

using namespace vex;

int sideSelect()
{
    // Pre-auton
    int gyroScale = 140;
    int invertGyroScale = 140;
    gyroscope.startCalibration(invertGyroScale);//133);
    invertedGyro.startCalibration(invertGyroScale);//136);
    Brain.Screen.printAt(0, 30, "GYRO CALIBRATING...");
    task::sleep(6000);
    Brain.Screen.clearScreen();
    FrontRight.resetRotation();

    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(vex::color::red);
    Brain.Screen.drawRectangle(1,1,200,200);
    Brain.Screen.printAt(40, 40, false, "RED");
    Brain.Screen.setFillColor(vex::color::blue);
    Brain.Screen.drawRectangle(202,1,200,200);
    Brain.Screen.printAt(242, 40, false, "BLUE");
    while(!Brain.Screen.pressing()) task::sleep(100);
    while(Brain.Screen.pressing()) task::sleep(100);
    Brain.Screen.clearScreen();
    pressed = false;
    if(Brain.Screen.xPosition() < 200)
        side = "RED";
    else if(Brain.Screen.xPosition() > 200)
        side = "BLUE";
    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(vex::color::white);
    Brain.Screen.drawRectangle(1,1,200,200);
    Brain.Screen.printAt(40, 40, false, "Front");
    Brain.Screen.setFillColor(vex::color::orange);
    Brain.Screen.drawRectangle(202,1,200,200);
    Brain.Screen.printAt(242, 40, false, "Back");

    while(!Brain.Screen.pressing());
    while(Brain.Screen.pressing());
    Brain.Screen.clearScreen();
    if(Brain.Screen.xPosition() < 200)
        autonNum = 1;
    else if(Brain.Screen.xPosition() > 200)
        autonNum = 2;

    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(vex::color::red);
    Brain.Screen.drawRectangle(1,1,200,200);
    Brain.Screen.printAt(40, 40, false, "Park");
    Brain.Screen.setFillColor(vex::color::blue);
    Brain.Screen.drawRectangle(202,1,200,200);
    Brain.Screen.printAt(242, 40, false, "No Park");

    while(!Brain.Screen.pressing()) task::sleep(100);
    while(Brain.Screen.pressing()) task::sleep(100);
    Brain.Screen.clearScreen();
    if(Brain.Screen.xPosition() < 200)
        parking = true;
    else if(Brain.Screen.xPosition() > 200)
        parking = false;
  Brain.Screen.clearScreen();
  Brain.Screen.setFillColor(vex::color::blue);
  Brain.Screen.drawRectangle(1,1,200,200);
  if(autonNum == 2)
    Brain.Screen.printAt(40, 40, false, "Defensive");
  else
    Brain.Screen.printAt(40, 40, false, "Regular");
  Brain.Screen.setFillColor(vex::color::yellow);
  Brain.Screen.drawRectangle(202,1,200,200);
  if(autonNum == 2)
    Brain.Screen.printAt(242, 40, false, "Non Defensive");
  else
    Brain.Screen.printAt(242, 40, false, "Super Meta");
  while(!Brain.Screen.pressing()) task::sleep(100);
  while(Brain.Screen.pressing()) task::sleep(100);
  Brain.Screen.clearScreen();
  if(Brain.Screen.xPosition() < 200)
      isDefensive = true;
  else if(Brain.Screen.xPosition() > 200)
      isDefensive = false;
  Brain.Screen.clearScreen();
  Brain.Screen.setPenColor(vex::color::red);
  Brain.Screen.setFillColor(vex::color::black);
  Brain.Screen.printAt(1,40, side.c_str());
  Brain.Screen.printAt(1, 80, "%.2f", getAngle());
  return 0;
}

void pre_auton( void )
{
  task autonSelect(sideSelect, 1);
  Brain.Screen.clearScreen();
  Brain.Screen.printAt(0, 30, ("Side:" + side).c_str());

  Brain.Screen.printAt(0, 60, "AutonNum: %d", autonNum);
  Brain.Screen.printAt(0, 90, "Parking: %d", parking);
  Brain.Screen.printAt(0, 120, "Defensive: %d", isDefensive);


}

int autoNum = 0;
void autonomous( void )
{
    task c(angleMonitor, 1);
    if(autonNum == 1)
    {
      if(isDefensive)
      {
        thirdFrontAuton(drive);
      }else
      {
        superMetaAuton(drive);
      }


    }else
    {
      if(isDefensive)
      {
        backDefense(drive);
      }else
      {
        thirdBackAuton(drive);
      }
    }
  //  superMetaAuton(drive);
  // drive.straightDrive(24, 100, 90);
  //drive.turnTo(90, 30, 1500);
  //backDefense(drive);
}

void usercontrol( void )
{
  //pre_auton();
  FrontRight.stop(brakeType::coast);
  FrontLeft.stop(brakeType::coast);
  BackRight.stop(brakeType::coast);
  BackLeft.stop(brakeType::coast);
  task taskCatapult(cataTask, 1);
  task taskIntake(intakeTask, 1);
  task taskDrive(driveTask, 1);
  task taskScraper(scraperTask, 1);
  task taskGyro(angleMonitor, 1);
  int minPotenVal = 2000;
  while (true)
  {
    if (CataPot.value(analogUnits::range12bit) < minPotenVal)
      minPotenVal = CataPot.value(analogUnits::range12bit);
      Brain.Screen.printAt(30, 30, "%d             ", gyroscope.value(analogUnits::range12bit));
      Brain.Screen.printAt(30, 60, "%d             ", invertedGyro.value(analogUnits::range12bit));
      Brain.Screen.printAt(1, 80, "%.2f", getAngle());
      //Brain.Screen.printAt(30, 120, "Pot: %d           ", minPotenVal);
      task::sleep(100);
  }
}

int main() {

    //Run the pre-autonomous function.


    //Set up callbacks for autonomous and driver control periods.
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );
    pre_auton();
    //Prevent main from exiting with an infinite loop.
    while(1) {
      vex::task::sleep(100);//Sleep the task for a short amount of time to prevent wasted resources.
    }

}
