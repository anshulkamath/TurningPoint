#include "C:/Users/JakeFreeman/Desktop/TurningPoint/Code/Worlds/testConfigurations.h"
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

bool doubleShotOn = false;
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
                Intake.stop(brakeType::hold);
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
    while(true)
    {
        if (Controller.ButtonX.pressing())
            Puncher.spin(directionType::fwd, 100, velocityUnits::pct);
        else if(Controller.ButtonB.pressing())
            doubleShot(2450, 2190); // Temporary, replace with variables later
        else
            Puncher.stop(brakeType::coast);

        if (Controller.ButtonY.pressing())
            Angler.spin(directionType::fwd, 50, velocityUnits::pct);
        else if (Controller.ButtonA.pressing())
            Angler.spin(directionType::fwd, -50, velocityUnits::pct);
        else
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
    Angler.setStopping(brakeType::hold);
    task driveTask(taskDrive, 1);
    task intakeTask(taskIntake, 1);
    task scraperTask(taskScraper, 1);
    task puncherTask(taskPuncher, 1);

    Angler.resetRotation();
    //task angler(taskAngler, 1);
    //aux.doubleShot(2450, 2260);
    while (1)
    {
        Brain.Screen.printAt(30, 30, "%d", Poten.value(analogUnits::range12bit));

        sleep(1000);
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
