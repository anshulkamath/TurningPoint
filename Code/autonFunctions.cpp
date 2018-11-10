#include "robot-config.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;
using namespace vex;
double PI = 3.1415;
double wheelDiameter = 4;
double wheelBaseLength = 12;

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

void forward(double inches)
{
    double rots = inches/(wheelDiameter*PI);
    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, true);    
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
int intakeCont = 0;
int intakeControl()
{
    Intake.setVelocity(100, vex::velocityUnits::pct);
    while(true)
    {
        if(intakeCont == 0)
        {
            Intake.stop();
        }
        if(intakeCont == -1)
        {
            Intake.spin(vex::directionType::rev);
        }
        if(intakeCont == 1)
        {
            Intake.spin(vex::directionType::fwd);
        }
    }
    return -1;
}
bool inUse = false;
bool inTakeInUse = false;
bool fire = false;

int runVision()
{
    vector<vex::vision::object> greenFlags, redFlags, blueFlags;
    Vision.takeSnapshot(GREENFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
    {
        //if(Vision.objects[i].id == 1)
        {
            greenFlags.push_back(Vision.objects[i]);
        }
    }
    Vision.takeSnapshot(BLUEFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
    {
        //if(Vision.objects[i].id == 2)
        {
            blueFlags.push_back(Vision.objects[i]);
        }
    }
    Vision.takeSnapshot(REDFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
    {
        //if(Vision.objects[i].id == 3)
        {
            redFlags.push_back(Vision.objects[i]);
        }
    } 
    int redFlagsF1 = 0;
    string diff = "";
    double distAvg = 0;
    int num = 0;
    for(int i = 0; i<redFlags.size(); i++)
    {
        distAvg += distance(redFlags[i].width, redFlags[i].height);
        num++;
    }
    distAvg /= num;
    for(int i = 0; i<redFlags.size(); i++)
    {
       // diff += "R:" + intToString(redFlags[i].originX) +"x" + intToString(redFlags[i].width);
        for(int s = 0; s<greenFlags.size(); s++)
        {
            //diff += "G:"+intToString(greenFlags[s].originY);
            if(abs(redFlags[i].centerX - redFlags[i].width/2 - greenFlags[s].originX) < 20)
            {

              
                
                if(abs(redFlags[i].originY - greenFlags[s].originY) < 20)
                {
                  //                      diff = intToString(distAvg);
                    Brain.Screen.clearLine();
                    Brain.Screen.print(diff.c_str());

                    
                    int frontLeftValue = 0, frontRightValue = 0, backLeftValue = 0, backRightValue = 0;
                    //diff = intToString(redFlags[i].centerX);
                    redFlagsF1++;
                   // diff = distAvg;//intToString(redFlags[i].angle);
                    Controller1.Screen.clearScreen();
                    Controller1.Screen.clearLine();    
                    Controller1.Screen.print(diff.c_str());                    
                    if((redFlags[i].centerX-148) < -5)
                    { 
                        int v = -1;
                        double t = 0;
                        frontLeftValue = 20*v;
                        backLeftValue = -frontLeftValue;
                        frontRightValue = -20*v;
                        backRightValue = 20*v;                         
                    }else if((redFlags[i].centerX - 148) > 10)
                    {
                        int v = 1;
                        double t = 0;
                        frontLeftValue = 20*v;
                        backLeftValue = -frontLeftValue;
                        frontRightValue = -20*v;
                        backRightValue = 20*v;                         
                    }else
                    {
                        
                    }
                    FrontLeft.spin(directionType::fwd, frontLeftValue, velocityUnits::pct);
                    FrontRight.spin(directionType::fwd, frontRightValue, velocityUnits::pct);       
                    BackRight.spin(directionType::fwd, backRightValue, velocityUnits::pct);
                    BackLeft.spin(directionType::fwd,  backLeftValue, velocityUnits::pct);      
                    if(frontLeftValue == 0) 
                    {
                        if(distAvg > 44)
                        {
                            forward(distAvg - 42);
                        }else if(distAvg < 40)
                        {
                            backward(42- distAvg);
                        }                        
                        return 0;
                    }
                    //runVision();
                    // Red Flag Found
                }
            }
        }
    }
    int blueFlagsF1 = 0;
    for(int i = 0; i<blueFlags.size(); i++)
    {
        for(int s = 0; s<greenFlags.size(); s++)
        {
            if(abs(blueFlags[i].originX - (greenFlags[s].originX+greenFlags[s].width)) < 10)
            {
                if(abs(blueFlags[i].originY - greenFlags[s].originY) < 10)
                {
                    blueFlagsF1++;
                    // Blue Flag Found
                }
            }
        }
    }
    
    //string line = intToString(blueFlags.size()) + " " + intToString(redFlags.size()) + " " + intToString(greenFlags.size()) + " " + intToString(redFlagsF1);
    Controller1.Screen.clearScreen();
    Controller1.Screen.clearLine();    
    Controller1.Screen.print(diff.c_str());
    return -1;
}

int taskShooter()
{
    while(true)
    {
        if(Controller1.ButtonX.pressing() || fire)
        {
            fire = false;
            inUse = true;
            Shooter.spin(directionType::fwd, 100, velocityUnits::pct);
            bool notTop = false;
            while(!Limit1.pressing())
            {
                notTop = true;
            }
            
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


int main() {
    string color = "Blue";
    // Front
    task intakeTask = task(intakeControl);
    task shooterTask = task(taskShooter);
    
    intakeCont = 1;
    backward(38.5);
    
    task::sleep(500);    
    if(color == "Red")
    {
        forward(34);
    task::sleep(600);        
        turnRight(90);
    }else
    {
        forward(33);
    task::sleep(600);        
        turnLeft(90);
    }
    Brain.resetTimer();
    while(!Limit2.pressing() || Brain.timer(timeUnits::msec) < 700);
    intakeCont = -1;
    task::sleep(500);
    while(runVision() == -1);
    fire = true; 
    task::sleep(600);
    // Strafe
    forward(75);
    
}
