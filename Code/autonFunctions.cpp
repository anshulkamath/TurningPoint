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
double getAngle()
{
    double val = -GyroS.value(rotationUnits::deg) + GyroI.value(rotationUnits::deg);
    val /= 2;
    return val;
}
void getOnPlatform()
{
    forward(35, 100);
    forward(60, 100);
}

void turnTo(double degrees)
{
    if(getAngle() < degrees)
    {
        turnRight(degrees - getAngle());
    }else if(getAngle() > degrees)
    {
        turnLeft(getAngle() - degrees);
    }
}
int runVision(string color)
{
    // Take Snapshot of the field, puts variables into arrays
    vector<vex::vision::object> greenFlags, redFlags, blueFlags;
    Vision.takeSnapshot(GREENFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
        greenFlags.push_back(Vision.objects[i]);
    
    Vision.takeSnapshot(BLUEFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
        blueFlags.push_back(Vision.objects[i]);
    
    Vision.takeSnapshot(REDFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
        redFlags.push_back(Vision.objects[i]);
    
    // Finds the distance from each flag
    double distAvg = 0;
    int num = 0;
    for(int i = 0; i<((color == "RED") ? redFlags.size() : blueFlags.size()); i++)
    {
        if(color == "RED" && redFlags[i].height > 12 && redFlags[i].width > 12)
        {
            distAvg += distance(redFlags[i].width, redFlags[i].height);
            num++;
        }
        else if(color == "BLUE" && blueFlags[i].height > 12 && blueFlags[i].width > 12)
        {
            distAvg += distance(blueFlags[i].width, blueFlags[i].height);
            num++;            
        }
    }

    distAvg /= num;
    
    // Puts all color into one arraylist
    vector<vex::vision::object> flagsTemp;    
    if(color == "RED")
        flagsTemp = redFlags;
    else if(color == "BLUE")
        flagsTemp = blueFlags;
    
    // Find Flags
    for(int i = 0; i<flagsTemp.size(); i++)
    {
        for(int s = 0; s<greenFlags.size(); s++)
        {
            // Make sure that the flag and green flag indices are matching
            if(abs(flagsTemp[i].centerX - flagsTemp[i].width/2 - greenFlags[s].originX) < 20
                && abs(flagsTemp[i].originY - greenFlags[s].originY) < 20)
            {
                // Strafe to the flags 
                int frontLeftValue = 0, frontRightValue = 0, backLeftValue = 0, backRightValue = 0;
                
                if((flagsTemp[i].centerX - 136) <= -5)
                { 
                    int v = -1;
                    frontLeftValue = 15*v;
                    backLeftValue = -frontLeftValue;
                    frontRightValue = -15*v;
                    backRightValue = 15*v;                         
                }
                else if((flagsTemp[i].centerX - 136) > 10)
                {
                    int v = 1;
                    double t = 0;
                    frontLeftValue = 15*v;
                    backLeftValue = -frontLeftValue;
                    frontRightValue = -15*v;
                    backRightValue = 15*v;                         
                }
                
                FrontLeft.spin(directionType::fwd, frontLeftValue, velocityUnits::pct);
                FrontRight.spin(directionType::fwd, frontRightValue, velocityUnits::pct);       
                BackRight.spin(directionType::fwd, backRightValue, velocityUnits::pct);
                BackLeft.spin(directionType::fwd,  backLeftValue, velocityUnits::pct);      
                if(frontLeftValue == 0) 
                {
                    int d = 0;
                    //if(abs(gyroValue) < 22.5)
                    {
                        //turnTo(0);
                        d = 1;
                    }
                    //else
                      //  Controller1.rumble("-.-.-.-.-");                    
                    // Go forward/backward to the flags
                    if(distAvg > 44)
                    {
                        forward(distAvg - 42);
                    }else if(distAvg < 40)
                    {
                        backward(42- distAvg);
                    }                     
                    task::sleep(200);
                    return 0;
                }
            }
        }
    }
    return -1;
}


void park(string side)
{
    backward(11);
    if(side == "Blue")
        turnRight(45);
    else
        turnLeft(45);
    backward(50);
}

// Grabs ball, flips cap, shoots, scores bottom flag
void autonFunc1(string side)
{
    //task intakeTask = task(intakeControl);
    task shooterTask = task(taskShooter);
    Lift.rotateFor(30, rotationUnits::deg, 50, velocityUnits::pct, true);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);
    backward(39.5);
    
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);
    task::sleep(300);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);
    task::sleep(1000);    
    
    if(side == "Red")
    {
        forward(34.5);
        task::sleep(600);
        turnRight(93);
    }
    else
    {
        forward(35.5);
        task::sleep(300);        
        turnLeft(90);
    }
    task::sleep(700);
    //Brain.resetTimer();
    //while(!Limit2.pressing() || Brain.timer(timeUnits::msec) < 700);
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    task::sleep(1500);
    //while(runVision("BLUE") == -1);
    fire = true;
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);
    task::sleep(600);
    if (side == "Red")
    {
       strafeRight(2.5); 
    }
    else if (side == "Blue")
    {
        strafeLeft(3);
    }
    forward(53);
}

// What do we want to do for this auton?
void auton2(string side)
{
    
}


int main()
{
  // while(runVision("BLUE") == -1);
   autonFunc1("Red");
  // park("Red");
    //getOnPlatform();
}
