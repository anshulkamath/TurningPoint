#include "robot-config.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
using namespace vex;
double PI = 3.1415;
double wheelDiameter = 4;
double wheelBaseLength = 12;
double maxHeightLift = 36;
double liftGearRatio = 5;

double distance(double x, double y)
{
    double W0 = 38;
    double H0 = 40;
    double dist0 = 36.5;
    double dx = (W0/x)*dist0;
    double dy = (H0/y)*dist0;
    double d = (dx+dy)/2;
    
    return d;
}

void forward(double inches, double speed = 70)
{
    double rots = inches/(wheelDiameter*PI);
    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, true);    
}

void backward(double inches)
{
    double rots = inches/(wheelDiameter*PI);
    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 50, vex::velocityUnits::pct, true);        
}

void turnLeft(double degrees)
{
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 1.693;
    
    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, true);    
}

void turnRight(double degrees)
{
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 1.71;
    
    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, true);    
}

void strafeLeft(double inches)
{
    double rots = inches/(wheelDiameter*PI) * sqrt(2);
    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, true);
}

void strafeRight(double inches)
{
    double rots = inches/(wheelDiameter*PI) * sqrt(2);
    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, true);
}

int intakeCont = 0;
int intakeControl()
{
    Intake.setVelocity(100, vex::velocityUnits::pct);
    while(true)
    {
        if(intakeCont == 0)
            Intake.stop();
        else if(intakeCont == -1)
            Intake.spin(vex::directionType::rev);
        else if(intakeCont == 1)
            Intake.spin(vex::directionType::fwd);
    }
    return -1;
}
bool inUse = false;
bool inTakeInUse = false;
bool fire = false;

int taskShooter()
{
    while(true)
    {
        if(fire)
        {
            fire = false;
            inUse = true;
            Shooter.spin(directionType::fwd, 100, velocityUnits::pct);
            bool notTop = false;
            while(!Limit1.pressing())
                notTop = true;
            
            Shooter.stop();
            task::sleep(500);
            Shooter.spin(directionType::fwd, 100, velocityUnits::pct);
            
            while(Limit1.pressing());
            
            Shooter.stop();
            Shooter.rotateFor(1800,rotationUnits::deg,100,velocityUnits::pct); 
            inUse = false;
        }       
        if(Limit2.pressing())
        {
            inTakeInUse = true;
            Intake.spin(directionType::rev, 100, velocityUnits::pct);
            task::sleep(500);
            Intake.stop();
            inTakeInUse = false;
        }
        
    }
    return 0;
}

void moveArm(double inches)
{
    double percentOfMax = inches/maxHeightLift;
    
    double rot = percentOfMax * liftGearRatio;
    
    Lift.rotateFor(rot, rotationUnits::rev, 100, velocityUnits::pct);
}

void getOnPlatform()
{
    forward(35, 100);
    forward(60, 100);
}

// Grabs ball, flips cap, shoots, scores bottom flag
void autonFunc1(string side)
{
    //task intakeTask = task(intakeControl);
    task shooterTask = task(taskShooter);
    Lift.rotateFor(30, rotationUnits::deg, 50, velocityUnits::pct, true);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);
    backward(37.5);
    
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);
    task::sleep(300);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);
    task::sleep(1000);    
    
    if(side == "Red")
    {
        forward(38);
        task::sleep(600);
        turnRight(90);
    }
    else
    {
        forward(35);
        task::sleep(600);        
        turnLeft(90);
    }
    task::sleep(700);
    //Brain.resetTimer();
    //while(!Limit2.pressing() || Brain.timer(timeUnits::msec) < 700);
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    task::sleep(1500);
    //while(runVision() == -1);
    fire = true;
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);
    task::sleep(600);
    if (side == "Red")
    {
       strafeRight(5.5); 
    }
    else if (side == "Blue")
    {
        strafeLeft(3);
    }
    forward(43);
}

// What do we want to do for this auton?
void auton2(string side)
{
    
}


int main()
{
   autonFunc1("Red");
    //getOnPlatform();
}
