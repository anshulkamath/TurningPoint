#pragma once;

#include "Drivetrain.cpp"

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

    double rots = ((degrees - 10)/360) * ((13*3.1415)/(4*3.1415)) * .703125*1.09756;
    rots /= 2.33333333;
    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, true);
    double error = 100, lError = 100;
    double smallPowerConst = 3, smallPower;
    while(true)
    {
      smallPower = smallPowerConst;
      error = degrees - getAngle();
      if(abs(error) <= .75 && abs(lError) <= .75) break;
      if(error < 0) smallPower *= -1;

      Brain.Screen.printAt(30, 30, "%d             ", gyroscope.value(analogUnits::range12bit));
      Brain.Screen.printAt(30, 60, "%d             ", invertedGyro.value(analogUnits::range12bit));
      Brain.Screen.printAt(30, 90, "%.2f           ", getAngle());
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

void firstFrontAuton(Drivetrain drive)
{
  task cata(cataTask, 1);
  runIntake(1);
  drive.drivePID(-35, 80, 20, 20, 1500, 0);
  drive.slipAdjust(true, true);
  task::sleep(200);
  drive.drivePID(38, 80, 20, 20, 1500, 0);
  runIntake(0);
  drive.slipAdjust(false, true);
  turnRight(90);
  task::sleep(10);
  drive.slipAdjust(true, true, 0, 100);
  task::sleep(100);
  runIntake(1);
  drive.drivePID(15, 45, 50, 50, 1500);
  runIntake(0);  
  fire = true;
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
