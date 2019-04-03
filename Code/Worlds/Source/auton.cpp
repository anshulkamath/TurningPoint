#pragma once;

#include "Drivetrain.cpp"
#include "task.cpp"
#include "vars.cpp"
void setBrakeMode(vex::brakeType brake)
{
    FrontLeft.setStopping(brake);
    FrontRight.setStopping(brake);
    BackLeft.setStopping(brake);
    BackRight.setStopping(brake);
}

void turnRight(double degrees)
{
  drive.slipAdjust(true, false);
    setBrakeMode(vex::brakeType::hold);
    double trueDegs = degrees - getAngle();
    if(sgn(trueDegs) < 0) trueDegs += 30;
    else trueDegs -= 30;
    double rots = ((trueDegs)/360) * ((13*3.1415)/(4*3.1415)) * .703125*1.09756;
    rots /= 2.33333333;
    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 25, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 25, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 25, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 25, vex::velocityUnits::pct, true);
    double error = 100, lError = 100;
    double smallPowerConst = 20, smallPower;
    double kp = .65;
    drive.brake();
    while(true)
    {
      smallPower = smallPowerConst;
      error = degrees - getAngle();
      smallPower  = fabs(error * kp);
     if(fabs(error) <= .75 && fabs(lError) <= .75) break;
      if(smallPower < 4) smallPower = 2;
      if(error < 0) smallPower *= -1;

      FrontRight.spin(directionType::fwd, -smallPower, vex::velocityUnits::pct);
      FrontLeft.spin(directionType::fwd, smallPower, vex::velocityUnits::pct);
      BackLeft.spin(directionType::fwd, smallPower, vex::velocityUnits::pct);
          BackRight.spin(directionType::fwd, -smallPower, vex::velocityUnits::pct);
      lError = error;
      task::sleep(20);
    }
    FrontLeft.stop(hold);
    FrontRight.stop(hold);
    BackLeft.stop(hold);
    BackRight.stop(hold);
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
  drive.straightDrive(5.5, 40, getAngle());
  task::sleep(50);
  Scraper.rotateTo(0,  rotationUnits::deg, 100, velocityUnits::pct);
  drive.turnTo(135, 60, 4800);
  drive.slipAdjust(true, true);
  FrontRight.resetRotation();

  autonFireRotation = .622;
  autonFire = true;
//  drive.drivePID(20, 50, 50, 50, 1500, 125);
//  fire = true;
//  runIntake(0);
  //task::sleep(200);
  drive.drivePID(62, 100, 20, 20, 1500, 135);

}

void firstFrontAuton(Drivetrain drive)
{
  task cata(cataTask, 1);
    runIntake(1);
    drive.drivePID(-35, 80, 20, 20, 1500, 0);
    // drive.slipAdjust(true, true);
    drive.drivePID(35.5, 80, 20, 20, 1500, 0);
    runIntake(0);
    drive.slipAdjust(false, true, 30, 30);
    drive.turnTo(90, 30, 1500);
    //drive.turnTo(90, 100);
    task::sleep(100);
    drive.slipAdjust(true, true, 25, 100);
    task::sleep(100);
    runIntake(1);
    drive.straightDrive(10, 30, 90);
    runIntake(0);
    fire = true;
    task::sleep(20);
    drive.turnTo(90, 20, 1500);
    drive.slipAdjust(false, false);
    drive.straightDrive(-5, 30, 90);
    task::sleep(50);
    drive.slipAdjust(true, false, 40, 40);
    drive.turnTo(-43, 30, 1500);
    drive.straightDrive(-12.5, 40, -43);
    scrapeFunction(drive);
    return;
}

int drivey() {
  while(true)
  {
    Controller.rumble("-.-.-");
    drive.drivePID(-52, 100, 20, 20, 1500, 0);
    return 0;
  }
  return 1;
}

void frontDefense(Drivetrain drive)
{
//runIntake(1);
  FrontRight.resetRotation();
  //task driveT(drivey, 1);
  drivey();
  //while(FrontRight.rotation(rotationUnits::rev) < 36.0/(4*3.1415*2.33333));
  Scraper.rotateTo((scraperHeight+scraperReady)/2.0 + 30,  rotationUnits::deg, 100, velocityUnits::pct);
  runIntake(0);
  while (true);
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


void testTurn()
{
  drive.turnTo(90, 32);
  drive.turnTo(-45, 40);
  drive.turnTo(0, 30);
}
