#include "robot-config.h"
  
double PI = 3.1415;
double wheelDiameter = 4;
double wheelBaseLength = 12;

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
    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, true);        
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


int main() {
    while(true)
    {
        if(Controller1.ButtonX.pressing())
        {
            turnLeft(45);
        }
        if(Controller1.ButtonY.pressing())
        {
            turnLeft(180);
        }        
        if(Controller1.ButtonUp.pressing())
        {
            turnRight(45);
        }
        if(Controller1.ButtonDown.pressing())
        {
            turnRight(180);
        }        
    }
    turnLeft(45);
}
