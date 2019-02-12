#include "robot-config.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>

using namespace std;
using namespace vex;

int wheelDiameter = 4;
double PI = M_PI;
double wheelBaseLength = 9.5;
int sgn(double val)
{
    return val > 0 ? 1 : -1;
}
void setBrakeMode(vex::brakeType brake)
{
    FrontLeft.setStopping(brake);
    FrontRight.setStopping(brake);
    BackLeft.setStopping(brake);
    BackRight.setStopping(brake);
}


double getAngle()
{
    return (gyroscope.value(analogUnits::range12bit) - invertedGyro.value(analogUnits::range12bit))/20;
}
void turnLeft(double degrees)
{

    setBrakeMode(vex::brakeType::hold);    
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 90/86 * 92.5/90;  
    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, true);    
    setBrakeMode(vex::brakeType::coast);  
}

void turnRight(double degrees)
{
    setBrakeMode(vex::brakeType::hold);       
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 90/86 * 92.5/90;

    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, true); 
    
    setBrakeMode(vex::brakeType::coast);  
}


double rampUp(double deltaV, int cycles, int timeSlice)
{
    double currVel = FrontRight.velocity(velocityUnits::pct);
    FrontRight.resetRotation();

    for (int i = 0; i < cycles; i++)
    {
        currVel += deltaV/cycles;
        BackLeft.spin(directionType::fwd, currVel, velocityUnits::pct);
        BackRight.spin(directionType::fwd, currVel, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, currVel, velocityUnits::pct);        
        FrontLeft.spin(directionType::fwd, currVel, velocityUnits::pct);

        task::sleep(timeSlice);
    }

    return FrontRight.rotation(rotationUnits::rev);
}

void drive(double inches, double speed, int cycles = 10, int timeSlice = 50)
{
    double rots = inches / (wheelDiameter * PI);
    rots -= rampUp(speed, cycles, timeSlice);
    FrontRight.resetRotation();
    
    double P = 0, kp = 75;
    double I = 0, ki = .95;
    double D = 0, kd = 230;
    double error = 0, lError = 0;
    double iThresh = .5;
    double motorPower = 0, lMotorPower = 0;
    double leftAdjustPwr = 0;
    double init = getAngle();
    while (abs(FrontRight.rotation(rotationUnits::rev)) < abs(rots))
    {
        error = rots - (FrontRight.rotation(rotationUnits::rev));
        P = error * kp;
        D = (error - lError) * kd;
        
        if (abs(error) < iThresh)
            I += error * ki;
        else
            I = 0;
        
        motorPower = abs(P) + abs(I) - abs(D);

        if (abs(motorPower) > abs(speed))
            motorPower = abs(speed);
        if(speed < 0)
        {
            motorPower *= -1;
        }
        leftAdjustPwr = getAngle() - init;
        BackLeft.spin(directionType::fwd, motorPower - 2*leftAdjustPwr, velocityUnits::pct);
        BackRight.spin(directionType::fwd, motorPower, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, motorPower, velocityUnits::pct);        
        FrontLeft.spin(directionType::fwd, motorPower - 2*leftAdjustPwr, velocityUnits::pct);
        lError = error;        
        task::sleep(50);        

    }
    
    BackLeft.stop(brakeType::brake);
    BackRight.stop(brakeType::brake);
    FrontRight.stop(brakeType::brake);
    FrontLeft.stop(brakeType::brake);
}


void turnTo(double degrees, double speed)
{
    double P = 0, kp = .8;
    double error = 100, motorPower = 0;
    while(abs(getAngle()) <= degrees && abs(error) > .2)
    {
        error = degrees - getAngle();
        P = error * kp;
        motorPower = P;
        if(abs(motorPower) > abs(speed))
            motorPower = speed;
        if(error < 0)
            motorPower *= -1;
        BackLeft.spin(directionType::fwd, motorPower, velocityUnits::pct);
        BackRight.spin(directionType::fwd, -motorPower, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, -motorPower, velocityUnits::pct);        
        FrontLeft.spin(directionType::fwd, motorPower, velocityUnits::pct);
        task::sleep(50);
    }
    BackLeft.stop(brakeType::brake);
    BackRight.stop(brakeType::brake);
    FrontRight.stop(brakeType::brake);
    FrontLeft.stop(brakeType::brake);    
}

void printGyro()
{
    
}
int main() {
    gyroscope.startCalibration();
    invertedGyro.startCalibration();
    task::sleep(8000);

    turnTo(90, 50);
    task::sleep(1000);
    Brain.Screen.print("%d", getAngle());    
    turnTo(180, 50);
    
    //drive(36, 100);
    //task::sleep(1000);
    //drive(-36, -100);
}
