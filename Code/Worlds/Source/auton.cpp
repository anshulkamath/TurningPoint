#pragma once;

#include "Drivetrain.cpp"

int cataTask()
{
    // Cata Variables
    int cataPower = 0;
    int error = 0;
    int thresh = 1;
    double kp = 0.25;

    CataL.setStopping(brakeType::hold);
    CataR.setStopping(brakeType::hold);
    while(true)
    {
        // Calculating error of catapult
        error = CataPot.value(analogUnits::range12bit) - cataDown;

        if (Controller.ButtonX.pressing() || fire)
        {
            fire = false;
            CataL.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct, false);
            //CataR.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct, true);
        }

        cataPower = error * kp;
        // If catapult is not in position, set cataPower based on position
        if (abs(error) > thresh)
            cataReady = false;
        else // If the catapult is in position, broadcast ready and stop
            cataReady = true;

        CataL.spin(directionType::fwd, cataPower, velocityUnits::pct);
        CataR.spin(directionType::fwd, cataPower, velocityUnits::pct);
        task::sleep(20);
    }
    return 0;
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
  drive.drivePID(-36, 100, 3);
  runIntake(0);
  drive.drivePID(40, 100, 3);
  drive.turnTo(-90, 60);
  drive.drivePID(12, 40, 4);
  drive.turnTo(-90, 60);
}
