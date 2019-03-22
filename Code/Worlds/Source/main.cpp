#include "vars.cpp"
#include "../cleanConfigurations.h"
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

int cataTask()
{
    // Cata Variables
    int cataPower = 0;
    int error = 0;
    int thresh = 5;
    double kp = 0.25;

    CataL.setStopping(brakeType::hold);
    CataR.setStopping(brakeType::hold);
    while(true)
    {
        // Calculating error of catapult
        error = CataPot.value(analogUnits::range12bit) - cataDown;

        if (Controller.ButtonX.pressing() || cataFire)
        {
            cataFire = false;
            CataL.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct, false);
            CataR.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct, true);
        }

        // If catapult is not in position, set cataPower based on position
        if (abs(error) > thresh)
        {
            cataReady = false;
            cataPower = error * kp;
        }
        else // If the catapult is in position, broadcast ready and stop
        {
            cataReady = true;
            cataPower = 0;
        }

        CataL.spin(directionType::fwd, cataPower, velocityUnits::pct);
        CataR.spin(directionType::fwd, cataPower, velocityUnits::pct);
        task::sleep(20);
    }
    return 0;
}

int driveTask()
{
    int accelCap = 10;
    int leftSide = 0, rightSide = 0;
    int prevLeft = 0, prevRight = 0; // To maintain acceleration
    while(true)
    {
        leftSide = 0;
        rightSide = 0;

        if (Controller.ButtonR1.pressing())
        {
          // First sets the motor power
          rightSide = leftSide = 50;

          // Next checks to see if the change in velocity is greater than
          // the acceleration cap
          if (abs(rightSide - l_RightSide) > accelCap)
            leftSide = rightSide = prevRight + accelCap;
        }
        else if (Controller.ButtonR2.pressing())
        {
          // First sets the motor power
          rightSide = leftSide = -50;

          // Next checks to see if the change in velocity is greater than
          // the acceleration cap
          if (abs(rightSide - l_RightSide) > accelCap)
            leftSide = rightSide = prevRight - accelCap;
        }

        if (abs(Controller.Axis3.value()) > 10)
            leftSide = Controller.Axis3.value();
        if (abs(Controller.Axis2.value()) > 10)
            rightSide = Controller.Axis2.value();

        FrontLeft.spin(directionType::fwd, leftSide, velocityUnits::pct);
        BackLeft.spin(directionType::fwd, leftSide, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, rightSide, velocityUnits::pct);
        BackRight.spin(directionType::fwd, rightSide, velocityUnits::pct);

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
    int cataTorque = 0;
    task taskCatapult(cataTask, 1);
    task taskIntake(intakeTask, 1);
    task taskDrive(driveTask, 1);
    task taskScraper(scraperTask, 1);
    while (true)
    {
        /*if (CataL.torque(torqueUnits::Nm) > cataTorque)
            //cataTorque = CataL.torque(torqueUnits::Nm);
        Brain.Screen.printAt(0, 30, "Temperature: %.2f", CataL.temperature());
        Brain.Screen.printAt(0, 60, "Torque: %.2f", cataTorque);*/

        Brain.Screen.printAt(0, 90, "Potentiometer: %d", CataPot.value(analogUnits::range12bit));

        task::sleep(20);
    }
}
