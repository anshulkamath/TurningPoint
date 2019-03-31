#pragma once;

#include "Drivetrain.cpp"
#include "task.cpp"
void setBrakeMode(vex::brakeType brake)
{
    FrontLeft.setStopping(brake);
    FrontRight.setStopping(brake);
    BackLeft.setStopping(brake);
    BackRight.setStopping(brake);
}

void turnRight(double degrees)
{
    setBrakeMode(vex::brakeType::hold);
    double trueDegs = degrees - getAngle();
    double rots = ((trueDegs - 10)/360) * ((13*3.1415)/(4*3.1415)) * .703125*1.09756;
    rots /= 2.33333333;
    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, true);
    /*double error = 100, lError = 100;
    double smallPowerConst = 2, smallPower;
    while(true)
    {
      smallPower = smallPowerConst;
      error = degrees - getAngle();
      if(abs(error) <= .75 && abs(lError) <= .75) break;
      if(error < 0) smallPower *= -1;

      FrontRight.spin(directionType::fwd, -smallPower, vex::velocityUnits::pct);
      FrontLeft.spin(directionType::fwd, smallPower, vex::velocityUnits::pct);
      BackLeft.spin(directionType::fwd, smallPower, vex::velocityUnits::pct);
          BackRight.spin(directionType::fwd, -smallPower, vex::velocityUnits::pct);
      lError = error;
      task::sleep(20);
    }
    FrontLeft.stop(brake);
    FrontRight.stop(brake);
    BackLeft.stop(brake);
    BackRight.stop(brake);
    */
    task::sleep(50);
}

void turnLeft(double degrees)
{
    setBrakeMode(vex::brakeType::hold);
    double trueDegs = degrees;
    double rots = ((trueDegs - 2)/360) * ((13*3.1415)/(4*3.1415)) * .703125*1.09756;
    rots /= 2.33333333;
    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, true);
    double error = 100, lError = 100;
    double smallPowerConst = 2, smallPower;

    while(true)
    {
      smallPower = smallPowerConst;
      error = degrees - getAngle();
      if(abs(error) <= .75 && abs(lError) <= .75) break;
      if(error < 0) smallPower *= -1;

      FrontRight.spin(directionType::fwd, -smallPower, vex::velocityUnits::pct);
      FrontLeft.spin(directionType::fwd, smallPower, vex::velocityUnits::pct);
      BackLeft.spin(directionType::fwd, smallPower, vex::velocityUnits::pct);
          BackRight.spin(directionType::fwd, -smallPower, vex::velocityUnits::pct);
      lError = error;
      task::sleep(20);
    }
    FrontLeft.stop(brake);
    FrontRight.stop(brake);
    BackLeft.stop(brake);
    BackRight.stop(brake);
}

// 1 is forward, 0 is stop, -1 is backward
void runIntake(int num)
{
    switch(num)
    {
        case -1:
            Intake.spin(directionType::rev, 100, velocityUnits::pct);
            break;
        case 0:
            Intake.stop(brakeType::coast);
            break;
        case 1:
            Intake.spin(directionType::fwd, 100, velocityUnits::pct);
            break;
        default:
            Intake.stop(brakeType::coast);
            break;
    }
}

int scraperTarget = 0;
int taskScraper()
{
    while(true)
    {
        Scraper.rotateTo(scraperTarget, rotationUnits::deg, 100, velocityUnits::pct);
    }
    return 0;
}

void scrapeFunction(Drivetrain drive)
{
  Scraper.rotateTo(scraperHeight,  rotationUnits::deg, 100, velocityUnits::pct);
  runIntake(1);
  drive.slipAdjust(true, true);
  drive.drivePID(6, 40, 20, 20, 1500);
  Scraper.rotateTo(0,  rotationUnits::deg, 100, velocityUnits::pct);
  drive.turnTo(135, 100);
  drive.slipAdjust(true, true);
  drive.drivePID(20, 60, 20, 20, 1500);
  fire = true;
  runIntake(0);
  task::sleep(200);
  drive.drivePID(48, 100, 20, 20, 1500);

}

void firstFrontAuton(Drivetrain drive)
{
  task cata(cataTask, 1);
  runIntake(1);
  drive.drivePID(-35, 80, 20, 20, 1500, 0);
  drive.slipAdjust(true, true);
  task::sleep(200);
  drive.drivePID(36, 80, 20, 20, 1500, 0);
  runIntake(0);
  drive.turnTo(90, 100);
  task::sleep(10);
  Controller.rumble("...");
  drive.slipAdjust(true, true, 25, 100);
  task::sleep(100);
  runIntake(1);
  drive.drivePID(12, 45, 50, 50, 1500, 90);
  runIntake(0);
  fire = true;
  task::sleep(200);
  drive.slipAdjust(false, false);
  drive.drivePID(-11, 45, 50, 50, 1500, 90);
  task::sleep(50);
  drive.turnTo(-45, 100);
  drive.drivePID(-13, 60);
  scrapeFunction(drive);

  //drive.turnTo(92, 100);//, 100);
  /*task::sleep(100);

   fire = true;
task::sleep(500);
   /*drive.drivePID(40, 80, 7, 7, 1500);
   drive.turnTo(92, 100);*/

  // drive.drivePID(-12, 30, 15, 7, 1500);
  // drive.turnTo(90, 100);
}
void firstBackAuton(Drivetrain drive)
{
  task cata(cataTask, 1);
  drive.drivePID(30, 80, 7, 7, 1500);
  drive.turnToSlow(108);
  fire = true;
  task::sleep(400);
  drive.turnToSlow(196);

  drive.drivePID(-10, 40, 7, 7, 1500);
  // scraper thing
  // flip cap
  drive.turnToSlow(270);
  runIntake(0);
  drive.drivePID(-20, 80, 7, 7, 1500);
  drive.turnToSlow(0);
  drive.drivePID(18, 80, 7, 7, 1500);
  drive.turnToSlow(130);
  fire = true;
  drive.drivePID(40, 100, 7, 7, 3000);
}
