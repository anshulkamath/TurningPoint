#pragma once;
#include "../Headers/Drivetrain.h"
#include "../cleanConfig.h"
#include "vars.cpp"

int intakeTask()
{
  Intake.setStopping(brakeType::coast);
  int intakePower = 0;
  while(true)
  {
      if (Controller.ButtonL1.pressing() && intakeActive)
          intakePower = 70;
      else if (Controller.ButtonL2.pressing())
          intakePower = -70;
      else
          intakePower = 0;

      // If catapult is not in position, set cataPower based on position
      if (abs(CataPot.value(analogUnits::range12bit) - cataDown) > 200)
          intakeActive = false;
      else // If the catapult is in position, broadcast ready and stop
          intakeActive = true;

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
        if (Controller.ButtonX.pressing() || fire || (autonFire && autonFireRotation <= abs(FrontRight.rotation(rotationUnits::rev))))
        {
          fire = false;
          CataL.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct, false);
          CataR.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct, true);
          autonFire = false;

          resetCata();
        }

        CataL.spin(directionType::fwd, 0, velocityUnits::pct);
        CataR.spin(directionType::fwd, 0, velocityUnits::pct);

        task::sleep(20);
    }
    return 0;
}

int FRCount = 0, BRCount = 0, FLCount = 0, BLCount = 0;
void setAutoBrake()
{
  const int timeCheck = 50;

  if (FrontRight.velocity(velocityUnits::rpm))
    FRCount++;
  else
    FRCount = 0;

  if (BackRight.velocity(velocityUnits::rpm))
    BRCount++;
  else
    BRCount = 0;

  if (FrontLeft.velocity(velocityUnits::rpm))
    FLCount++;
  else
    FLCount = 0;

  if (BackLeft.velocity(velocityUnits::rpm))
    BLCount++;
  else
    BLCount = 0;

  if (FRCount > timeCheck)
    FrontRight.setStopping(brakeType::hold);
  else
    FrontRight.setStopping(brakeType::coast);

  if (BRCount > timeCheck)
    BackRight.setStopping(brakeType::hold);
  else
    BackRight.setStopping(brakeType::coast);

  if (FLCount > timeCheck)
    FrontLeft.setStopping(brakeType::hold);
  else
    FrontLeft.setStopping(brakeType::coast);

  if (BLCount > timeCheck)
    BackLeft.setStopping(brakeType::hold);
  else
    BackLeft.setStopping(brakeType::coast);
}

int leftSide = 0, rightSide = 0;
int driveTask()
{
    const int accelCap = 30;
    int prevLeft = 0, prevRight = 0;

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
            leftSide = Controller.Axis3.value();
        if (abs(Controller.Axis2.value()) > 10)
            rightSide = Controller.Axis2.value();

        //setAutoBrake();

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

int angleMonitor()
{
  double iprevAngle = 0;
  double icurrentAngle = 0;
  double prevAngle = 0;
  double currentAngle = 0;
  double ithreshold = .15;
  double rthreshold = .2;
  double threshold = .15;
  double iAngle = 0;
  double rAngle = 0;

  double anglePrev = 0;
  // File stuff
  {
      fstream file1(string("filter")  + string(".csv"), fstream::app);
      file1<<"Prev,"<<",Current"<< ",Angle"<<",iPrev"<<",iCurrent"<< ",iAngle"<<",tAngle"<<endl;
      file1.close();
  }

  while(true)
  {
    fstream file1(string("filter")  + string(".csv"), fstream::app);
    icurrentAngle = (-invertedGyro.value(analogUnits::range12bit)/10.0);
    currentAngle = (gyroscope.value(analogUnits::range12bit)/10.0);
    if(fabs(icurrentAngle - iprevAngle) > ithreshold)
      iAngle += icurrentAngle - iprevAngle;
    if(fabs(currentAngle - prevAngle) > rthreshold )
      rAngle += currentAngle - prevAngle;

    double angleTemp = (rAngle + iAngle)/2;
    if(fabs(anglePrev - angleTemp) > threshold)
      angle += angleTemp - anglePrev;

    anglePrev = angle;
    iprevAngle = icurrentAngle;
    prevAngle = currentAngle;
    file1<<","<<anglePrev<<","<<currentAngle<<","<<rAngle<<","<<iprevAngle<<","<<icurrentAngle<<","<<iAngle<<","<<angle<<endl;
   file1.close();
    task::sleep(50);
  }
}


int wheelVelocity()
{
  {
      fstream file1(string("filter")  + string(".csv"), fstream::app);
      file1<<"currVel,velocity"<<endl;
      file1.close();
  }
  double error = 0;
  double kp = 1;
  FrontRight.spin(directionType::fwd);
  FrontRight.setStopping(brakeType::hold);
  while(true)
  {
    // Front Right
    error = FRVel - FrontRight.velocity(velocityUnits::pct);
    FrontRight.setVelocity(FRVel, velocityUnits::pct);
      fstream file1(string("wheel")  + string(".csv"), fstream::app);
      file1<<FRVel<<","<<FrontRight.velocity(velocityUnits::pct)<<endl;
      file1.close();
  }
}
