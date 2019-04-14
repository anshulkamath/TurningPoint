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
void runIntake(int num, double speed = 70)
{
    switch(num)
    {
        case -1:
            Intake.spin(directionType::rev, speed, velocityUnits::pct);
            break;
        case 0:
            Intake.stop(brakeType::coast);
            break;
        case 1:
            Intake.spin(directionType::fwd, speed, velocityUnits::pct);
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
  drive.straightDrive(4, 20, -39);
  //Scraper.rotateTo(scraperHeight,  rotationUnits::deg, 100, velocityUnits::pct);

  task::sleep(50);
  Scraper.rotateTo(0,  rotationUnits::deg, 100, velocityUnits::pct);
  drive.turnTo(138, 100, 1600);
  drive.slipAdjust(true, true);
  FrontRight.resetRotation();

  autonFireRotation = .482;
  autonFire = true;
//  drive.drivePID(20, 50, 50, 50, 1500, 125);
//  fire = true;
//  runIntake(0);
  //task::sleep(200);
  drive.drivePID(76, 100, 100, 100, 1500, 135);

}

void firstFrontAuton(Drivetrain drive)
{
  task cata(cataTask, 1);
    runIntake(1);
    drive.drivePID(-35, 70, 101, 101, 1500, 0);
    // drive.slipAdjust(true, true);
    drive.drivePID(35.5, 70, 101, 101, 1500, 0);
    runIntake(0);
    drive.slipAdjust(false, true, 30, 30);
    drive.turnTo(92, 32, 1500);
    //drive.turnTo(90, 100);
    task::sleep(100);
    drive.slipAdjust(true, true, 25, 90);
    task::sleep(100);
    runIntake(1);
    drive.straightDrive(10, 30, 92);
    runIntake(0);
    fire = true;
    task::sleep(20);
    drive.turnTo(90, 20, 1500);
    drive.slipAdjust(false, false);
    drive.straightDrive(-5, 30, 90);
    task::sleep(50);
    drive.slipAdjust(true, false, 40, 40);
    drive.turnTo(-42, 25, 2300);
    drive.straightDrive(-11, 40, -42);
    scrapeFunction(drive);
    runIntake(0);
    return;
}

void scrapeFunctionBlue(Drivetrain drive)
{
  Scraper.rotateTo(scraperHeight,  rotationUnits::deg, 100, velocityUnits::pct);
  runIntake(1);
  drive.slipAdjust(true, true);
  drive.straightDrive(4.5, 40, 39);
  //Scraper.rotateTo(scraperHeight,  rotationUnits::deg, 100, velocityUnits::pct);

  task::sleep(50);
  Scraper.rotateTo(0,  rotationUnits::deg, 100, velocityUnits::pct);
  drive.turnTo(-135, 35, 2000);
  drive.slipAdjust(true, true);
  FrontRight.resetRotation();

  autonFireRotation = .482;
  autonFire = true;
//  drive.drivePID(20, 50, 50, 50, 1500, 125);
//  fire = true;
//  runIntake(0);
  //task::sleep(200);
  drive.drivePID(72, 100, 100, 100, 1500, -135);

}

void firstFrontAutonBlue(Drivetrain drive)
{
  task cata(cataTask, 1);
    runIntake(1);
    drive.drivePID(-35, 70, 101, 101, 1500, 0);
    // drive.slipAdjust(true, true);
    drive.drivePID(35.5, 70, 101, 101, 1500, 0);
    runIntake(0);
    //drive.slipAdjust(false, true, 30, 30);
    drive.turnTo(-92, 25, 1500);
    //drive.turnTo(90, 100);
    runIntake(1);
    drive.straightDrive(10, 30, -92);
    runIntake(0);
    fire = true;
    task::sleep(20);
    drive.turnTo(-90, 20, 1500);
    drive.slipAdjust(false, false);
    drive.straightDrive(-5, 30, -90);
    task::sleep(50);
  //  drive.slipAdjust(true, false, 40, 40);
    drive.turnTo(42, 25, 2300);
    drive.straightDrive(-10.25, 40, 42);
    scrapeFunctionBlue(drive);
    runIntake(0);
    return;
}
double degTarget = 0;
int taskScraper1()
{
  while(FrontRight.rotation(rotationUnits::rev) < degTarget);
    Scraper.rotateTo(0,  rotationUnits::deg, 75, velocityUnits::pct);
  return 0;
}

void secondFrontAuton(Drivetrain drive)
{
  // task cata(cataTask, 1);
  //   runIntake(1);
  //   drive.drivePID(-35, 100, 100, 100, 1500, 0);
  //   // drive.slipAdjust(true, true);
  //   drive.drivePID(36, 100, 100, 100, 2100, 0);
  //   runIntake(0);
  //   drive.turnTo(92, 25, 2000);
  //   //drive.turnTo(90, 100);
  //   runIntake(0);
  //   FrontRight.resetRotation();
  //   autonFireRotation = 12.0/(4*3.1415*2.333);
  //   autonFire = true;
  //
  //   task::sleep(20);
  //   drive.drivePID(53, 100, 100, 100, 1500, 90);
  //   drive.slipAdjust(false, false);
  //   //  drive.turnTo(90, 25, 1800);
  //   return;
  task cata(cataTask, 1);
  runIntake(1);
  drive.drivePID(-35, 70, 101, 101, 1500, 0);
  drive.drivePID(side == "RED" ? 36.5 : 40, 70, 101, 101, 1500, 0);
  runIntake(0);
  drive.turnTo(93, 25, 1200);
  task::sleep(100);
  drive.slipAdjust(true, true, 25, 90);
  runIntake(1);
  drive.straightDrive(10, 30, 92);
  runIntake(0);
  fire = true;
  drive.turnTo(88, 25, 1000);
  drive.drivePID(43, 100, 100, 100, 1500, 90);
  Scraper.rotateTo(-850,  rotationUnits::deg, 80, velocityUnits::pct, false);
  drive.drivePID(-12, 100, 100, 100, 1500, 90);
  drive.turnTo(37, 30, 1500);
  drive.slipAdjust(false, false);
  FrontRight.resetRotation();
  Scraper.rotateTo(0,  rotationUnits::deg, 40, velocityUnits::pct, false);
  drive.drivePID(side == "RED" ? -90 : -93, 100, 100, 100, 1000, getAngle());
  drive.turnTo(-90, 40, 1200);

  drive.drivePID(66, 100, 100, 100, 3000, -90);
  return;
}

void secondBackAuton(Drivetrain drive)
{
  task cata(cataTask, 1);
    runIntake(1);
    drive.drivePID(-35, 100, 100, 100, 1500, 0);
    drive.slipAdjust(true, true);
    drive.drivePID(12, 50, 100, 100, 800, 0);
    drive.turnTo(102, 30, 3000);
    drive.slipAdjust(false, false);
    drive.drivePID(-27, 70, 100, 100, 1000, 102);
    task::sleep(400);
    fire = true;
    runIntake(0);
    return;
    task::sleep(500);
    drive.straightDrive(-3, 50, 102);
    drive.turnTo(0, 30);
    drive.slipAdjust(false, false);
    drive.straightDrive(-6.5, 30, 15);
    Scraper.rotateTo(scraperHeight,  rotationUnits::deg, 100, velocityUnits::pct);
    runIntake(1);
    drive.slipAdjust(true, true);
    drive.straightDrive(5.5, 40, 15);
    Scraper.rotateTo(0,  rotationUnits::deg, 100, velocityUnits::pct, true);

    drive.turnTo(110, 20);
    drive.slipAdjust(true, true);
    drive.drivePID(50, 100, 100, 100, 3000, 110);
    return;
  //  drive.turnTo(90, 30, 500);
    //drive.drivePID(40, 100, 100, 100, 1500, 90);
}

void thirdBackAuton(Drivetrain drive)
{
  task cata(cataTask, 1);
    runIntake(1);
    drive.drivePID(-35, 100, 100, 100, 1500, 0);
    drive.slipAdjust(true, true);
    drive.turnTo(-45, 40, 2000);
    Scraper.rotateTo(-730, rotationUnits::deg, 100, velocityUnits::pct, false);
    drive.drivePID(18, 100, 100, 100, 1500, 0);
    drive.turnTo(27, 25, 2000);
    runIntake(0);
    drive.drivePID(-34, 75, 100, 100, 1000, getAngle());
    task::sleep(500);
    Scraper.rotateTo(0, rotationUnits::rev, 100, velocityUnits::pct);
    drive.drivePID(15, 100, 100, 100, 1000, getAngle());
    drive.turnTo(90, 30, 3000);
    drive.drivePID(60, 100, 100, 100, 2000, 90);
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

void backDefense(Drivetrain drive)
{
  task cata(cataTask, 1);
  runIntake(1);
  //FrontRight.resetRotation();
  //task driv(drivey, 1);
  Scraper.rotateTo(-730, rotationUnits::deg, 1000, velocityUnits::pct, false);
  drive.drivePID(-51, 100, 101, 101, 1600, 0);
  runIntake(0);
  Scraper.rotateTo(-100, rotationUnits::deg, 1000, velocityUnits::pct, true);
  Scraper.rotateTo(-730, rotationUnits::deg, 1000, velocityUnits::pct, true);
  drive.setDrive(0, 0);
  task::sleep(3000);
  //while(FrontRight.rotation(rotationUnits::deg) < 36.0/(4.0*3.14*2.33333));
  Scraper.rotateTo(-100, rotationUnits::deg, 1000, velocityUnits::pct, true);
  drive.turnTo(100, 40);
  drive.slipAdjust(true, true);
  drive.straightDrive(3, 20, 100);
  Scraper.rotateTo(-870, rotationUnits::deg, 1000, velocityUnits::pct, true);
  drive.slipAdjust(false, false);
  drive.straightDrive(-8, 20, 100);
  task::sleep(500);
  Scraper.rotateTo(-100, rotationUnits::deg, 1000, velocityUnits::pct, true);

  drive.turnTo(35, 40);
  drive.straightDrive(18, 60, 60);
  drive.drivePID(30, 100, 100, 100, 1600, 90);

}



void frontDefense(Drivetrain drive)
{
  task cata(cataTask, 1);
  runIntake(1);
  Scraper.rotateTo(-720, rotationUnits::deg, 97, velocityUnits::pct, false);
  drive.drivePID(-50.5, 100, 101, 101, 1600, 0);

  Scraper.rotateTo(-100, rotationUnits::deg, 1000, velocityUnits::pct, true);
  drive.setDrive(0, 0);
  runIntake(0);
    //while(FrontRight.rotation(rotationUnits::deg) < 36.0/(4.0*3.14*2.33333));
  Scraper.rotateTo(-100, rotationUnits::deg, 1000, velocityUnits::pct, true);
  drive.turnTo(90, 30, 1500);
}

void firstBackAuton(Drivetrain drive)
{
  task cata(cataTask, 1);
  drive.drivePID(-30, 80, 7, 7, 1500);
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

void defenseFunc()
{
  runIntake(1);
  Scraper.rotateTo(-565, rotationUnits::deg, 1000, velocityUnits::pct, false);
  drive.drivePID(-52.5, 100, 101, 101, 1600, 0);
  drive.setDrive(0, 0);
  Scraper.rotateTo(0, rotationUnits::deg, 100, velocityUnits::pct);
  task::sleep(300);
  runIntake(0);
}

void thirdFrontAuton(Drivetrain drive)
{
  task cata(cataTask, 1);
  defenseFunc();
  drive.straightDrive(6, 40, 0);
  drive.turnTo(97, 40);
  drive.straightDrive(18, 40, 97);
  fire = true;
  drive.straightDrive(6, 30, 90);
  drive.turnTo(180, 40);
}

void scrapAndShoot()
{
    task cata(cataTask, 1);
task::sleep(500);

  runIntake(1);

  Scraper.rotateTo(-650, rotationUnits::deg, 70, velocityUnits::pct, false);
  FrontRight.resetRotation();

  //autonFireRotation = .9 * 38 /(4*3.1415*2.333);
//  autonFire = true;
  drive.drivePID(-18, 80, 100, 100, 1000, 0);
  fire = true;
  drive.drivePID(-5, 40, 100, 100, 400, 0);
  task::sleep(100);
  drive.straightDrive(8, 20, 0);
  runIntake(1);
  Scraper.rotateTo(0, rotationUnits::deg, 100, velocityUnits::pct);
//  drive.straightDrive(24, 20, 135);
  runIntake(1);
  drive.drivePID(-48, 80, 100, 100, 1000, 0);
  runIntake(0);
}

void superMetaAuton(Drivetrain drive)
{
    task cata(cataTask, 1);
      runIntake(1);
    drive.drivePID(-36, 100, 101, 101, 2500, 0);
          runIntake(0);
    drive.straightDrive(-5, 30, 0);//9
  //  drive.setDrive(-5, 5);
    //task::sleep(50);
      runIntake(1);
    drive.turnTo(90, 25, 1900);

    //task::sleep(200);
    drive.drivePID(52, 100, 100, 100, 1600, 90);
    runIntake(0);
    drive.straightDrive(-4.5, 20, 90);
    runIntake(0);
    if (side == "RED") drive.setDrive(0, 40);
    else drive.setDrive(40, 0);
    while(getAngle() < 110) {task::sleep(10);}
    drive.turnTo(135, 30, 1000);
    runIntake(1);

    if (side == "RED") Scraper.rotateTo(-700, rotationUnits::deg, 55, velocityUnits::pct, false);
    else Scraper.rotateTo(-700, rotationUnits::deg, 47, velocityUnits::pct, false);

    FrontRight.resetRotation();

    //autonFireRotation = .9 * 38 /(4*3.1415*2.333);
  //  autonFire = true;
    drive.drivePID(-24, 80, 100, 100, 1000, 135);
    fire = true;
    drive.straightDrive(-5, 40, 135);
    task::sleep(100);
    drive.straightDrive(8, 20, 135);
    task::sleep(600);
    runIntake(1, 100);
    Scraper.rotateTo(0, rotationUnits::deg, 100, velocityUnits::pct);
  //  drive.straightDrive(24, 20, 135);
    runIntake(1, 100);

    drive.drivePID(-54.5, 80, 100, 100, 1000, 135);
    task::sleep(150);
    drive.turnTo(81, 30, 2000);
    fire = true;
    task::sleep(30);
    if (side == "RED") drive.setDrive(100, 75);
    else drive.setDrive(70, 100);
    task::sleep(4000);
    drive.setDrive(0);
    runIntake(0);
    //return;

}
