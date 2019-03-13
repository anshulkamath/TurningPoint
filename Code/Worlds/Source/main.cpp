#include "C:/Users/JakeFreeman/Desktop/TurningPoint/Code/Worlds/cleanConfigurations.h"
#include "C:/Users/JakeFreeman/Desktop/TurningPoint/Code/Worlds/Headers/Drivetrain.h"
#include "C:/Users/JakeFreeman/Desktop/TurningPoint/Code/Worlds/Headers/Auxiliary.h"
#include "C:/Users/JakeFreeman/Desktop/TurningPoint/Code/Worlds/Source/Drivetrain.cpp"
#include "C:/Users/JakeFreeman/Desktop/TurningPoint/Code/Worlds/Source/Auxiliary.cpp"

using namespace vex;

Drivetrain drive(FrontRight, FrontLeft, BackRight, BackLeft);
Auxiliary aux(Intake, Puncher, Angler, Scraper, Poten);

void sleep (int time)
{
    task::sleep(time);
}

void doubleShot(int angle1, int angle2)
{
    // Go to the first shot angle
    aux.setAngle(angle1);
    Puncher.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct, true); // shoot first shot
    Puncher.rotateFor(1, rotationUnits::rev, 50, velocityUnits::pct, false); // shoot second shot
    Intake.spin(directionType::fwd, 100, velocityUnits::pct); // intake the next ball
    aux.setAngle(angle2); // Set to the second angle
    Intake.stop(brakeType::hold);
}
bool puncherUse = false;
bool intakeUse = false;
bool anglerUse = false;
int rightSide = 0, leftSide = 0;
int turnLimiter = 1;
int taskDrive()
{
    while(true)
    {
        rightSide = 0;
        leftSide = 0;
        // Constant Straight Drive Control
        if (Controller.ButtonR1.pressing())
            rightSide = leftSide = 60;
        else if (Controller.ButtonR2.pressing())
            rightSide = leftSide = -60;

        // Tank Drive Controls
        if (abs(Controller.Axis2.value()) > 20)
            rightSide = Controller.Axis2.value();
        if (abs(Controller.Axis3.value()) > 20)
            leftSide = Controller.Axis3.value();

        if(abs(abs(Controller.Axis2.value()) - abs(Controller.Axis3.value())) > 20)
        {
            leftSide = Controller.Axis3.value();
            rightSide = Controller.Axis2.value();
        }

        // Throttling Turns
        if(abs(Controller.Axis2.value() - Controller.Axis3.value()) > 50)
        {
            leftSide *= turnLimiter;
            rightSide *= turnLimiter;
        }

        // Sets Motor Powers
        drive.setDrive(rightSide, leftSide);

        sleep(25);
    }
    return 0;
}

int taskIntake()
{
    while (true)
    {
        if (!doubleShotOn)
        {
            // Controls intake
            if (Controller.ButtonL1.pressing())
                Intake.spin(directionType::fwd, 100, velocityUnits::pct);
            else if(Controller.ButtonL2.pressing())
                Intake.spin(directionType::rev, 100, velocityUnits::pct);
            else
                Intake.stop(brakeType::coast);
        }

        sleep(50);
    }
    return 0;
}

int taskScraper()
{
    while (true)
    {
        if (Controller.ButtonUp.pressing())
            Scraper.spin(directionType::rev, 100, velocityUnits::pct);
        else if (Controller.ButtonDown.pressing())
            Scraper.spin(directionType::fwd, 100, velocityUnits::pct);
        else
            Scraper.stop(brakeType::hold);

        sleep(50);
    }
}

int taskPuncher()
{
    double error = 30;
    while(true)
    {
        if (Controller.ButtonX.pressing())
            /*Puncher.rotateFor(360 + error, rotationUnits::deg, 100, velocityUnits::pct);*/Puncher.spin(directionType::fwd, 100, velocityUnits::pct);//
        else if(Controller.ButtonB.pressing())
            aux.doubleShot(50.8, 94.6);
        else if(Controller.ButtonLeft.pressing())
          aux.doubleShot(73.2, 101.4);
        else if(!puncherUse)
            Puncher.stop(brakeType::coast);

        if (Controller.ButtonY.pressing())
            Angler.spin(directionType::fwd, 50, velocityUnits::pct);
        else if (Controller.ButtonA.pressing())
            Angler.spin(directionType::fwd, -50, velocityUnits::pct);
        else if(!anglerUse)
            Angler.stop(brakeType::hold);
    }
    return 1;
}

void pre_auton( void )
{

}

void autonomous( void )
{

}

void usercontrol( void )
{
//Puncher.setStopping(brakeType::hold);
  //    Puncher.rotateFor(270, rotationUnits::deg, 100, velocityUnits::pct);

    Puncher.setMaxTorque(2, vex::torqueUnits::Nm);
    Angler.setStopping(brakeType::hold);
    task driveTask(taskDrive, 1);
    task intakeTask(taskIntake, 1);
    task scraperTask(taskScraper, 1);
    task puncherTask(taskPuncher, 1);

    Angler.resetRotation();
    double maxTorque = 0;
    // M2: M: 115.6 68
    //aux.doubleShot(2450, 2260);
    while (1)
    {
        Brain.Screen.printAt(30, 30, "%.2f", Angler.rotation(rotationUnits::deg));
        if (maxTorque <  Puncher.torque(torqueUnits::Nm))
          maxTorque =  Puncher.torque(torqueUnits::Nm);
        Brain.Screen.printAt(60, 60, "%.2f", maxTorque);
        sleep(100);
    }
}

int main()
{
    pre_auton();
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );

    while(1)
    {
        sleep(100);
    }

}
