#pragma once;
#include "../Headers/Drivetrain.h"
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;




void Drivetrain::turnTo(double angle, int speed)
{
    // 180 - .6, .007, 1.56
    // double kP = .43, P = 0;
    // double kI = 0.04, I = 0;
    // double kD = .65, D = 0;
    //     double iCap = 7;
    //     int iThresh = 15;
    // if(angle - getAngle() < 0)
    // {
    //   kP = .4;
    //   kI = .002;
    //   iCap = 3;
    //   iThresh= 30;
    // }

    double kP = .55, P = 0;
    double kI = 0.001, I = 0;
    double kD = 5, D = 0;
    double iCap = 7;
    int iThresh = 15;

    double error = 100, lError = 0;
    double motorPower = 0, prevMotorPower = 0;


    FrontRight.resetRotation();
    fstream file1(string("angle-54")  + string(".csv"), fstream::app);
    char * buff = "Hello";
    //    Brain.SDcard.appendFile("filename.txt",buff,100);
    file1<<",Target,Current,Error,P,D,I,Power"<<endl;
    int t = 0;
    double stblConst = 0;
    double init = getAngle();
    double velly = 0;
    int maxError = 100;

    while(abs(error) > 0 || abs(lError) > 0 || abs(motorPower) >= 3)
    {
        t++;

        error = angle - getAngle();

        if (error < maxError)
          maxError = error;

        P = kP * error;
        D = kD * (error - lError);

        if(abs(error) < iThresh)
            I += kI * error;

        if (abs(I) >= iCap) I = sgn(I) * iCap;
        motorPower = 100 - (D);
        int signBase = sgn(motorPower);
        motorPower = abs(motorPower);
        if(motorPower > speed) motorPower = speed;

        setDrive(-motorPower * signBase, motorPower * signBase);
        prevMotorPower = abs(motorPower);
        lError = error;
        Brain.Screen.printAt(30, 30, "%.2f, %.2f,%.2f,%d,%d   ", getAngle(), angle, motorPower, sgn(I), maxError);
        file1<<t<<","<<angle<<","<<getAngle()<<","<<error<<","<<abs(P)<<","<<-abs(D)<<","<<abs(I)<<","<<motorPower<<endl;
        task::sleep(25);
    }
    setDrive(0);
}

void Drivetrain::drivePID(double distance, double speed, int accelCap, double angler )
{
    double kP = .25*1.67, P = 0;
    double kI = .001*1.67, I = 0;
    double kD = 2, D = 0;
    double error = 100, lError = 0;
    double motorPower = 0, prevMotorPower = 0;
    double iCap = 5;
    distance /= (4.0*3.1415);
    distance /= 2.33333333;

    Controller.rumble("-.-.-");
    distance *= 360;
    FrontRight.resetRotation();
    fstream file1(string("angle-334")  + string(".csv"), fstream::out);
    file1<<",Target,Current,Error,P,D,I,Power"<<endl;
    int t = 0;
    double stblConst = .6;//2.0/360.0 * 1.66;
        double init = getAngle();
    if(angler != -360)
      init = angler;
    while(abs(error) >= 1 || abs(lError) >= 1)// || abs(motorPower) >= 3)//true)//distance > FrontRight.rotation(rotationUnits::deg))
    {
        t++;
        error = distance - FrontRight.rotation(rotationUnits::deg);
        P = kP * error;
        D = -kD * (error - lError);


        if(abs(error) < 20)
            I += kI * error;

        if (abs(I) >= iCap) I = sgn(I) * iCap;

        motorPower = (P) + (I) - (D);
        int signBase = sgn(motorPower);
        motorPower = abs(motorPower);
        if(motorPower > speed) motorPower = speed;

        if(motorPower + accelCap > prevMotorPower) motorPower = prevMotorPower + accelCap;

        if(prevMotorPower - accelCap > motorPower) motorPower = prevMotorPower - accelCap;

        if(motorPower > speed) motorPower = speed;

        double leftAdjustPwr = -stblConst * (getAngle() - init);

        setDrive(motorPower * signBase - leftAdjustPwr, motorPower * signBase + leftAdjustPwr);
        prevMotorPower = abs(motorPower);
        lError = error;
        Brain.Screen.printAt(30, 30, "%.2f, %.2f,%.2f", FrontRight.rotation(rotationUnits::deg), distance, motorPower);
        file1<<t<<","<<distance<<","<<FrontRight.rotation(rotationUnits::deg)<<","<<error<<","<<abs(P)<<","<<-abs(D)<<","<<abs(I)<<","<<sgn(error) * motorPower<<endl;
        task::sleep(25);
    }
    setDrive(0);
}

void Drivetrain::setDrive(int rightVel, int leftVel)
{
    fRight.setStopping(brakeType::brake);
    fLeft.setStopping(brakeType::brake);
    bRight.setStopping(brakeType::brake);
    bLeft.setStopping(brakeType::brake);
    fLeft.spin(directionType::fwd, leftVel, velocityUnits::pct);
    fRight.spin(directionType::fwd, rightVel, velocityUnits::pct);

    bRight.spin(directionType::fwd, rightVel, velocityUnits::pct);
    bLeft.spin(directionType::fwd, leftVel, velocityUnits::pct);
}

void Drivetrain::setDrive(int vel)
{
    fRight.setStopping(brakeType::brake);
    fLeft.setStopping(brakeType::brake);
    bRight.setStopping(brakeType::brake);
    bLeft.setStopping(brakeType::brake);
    fLeft.spin(directionType::fwd, vel, velocityUnits::pct);
    fRight.spin(directionType::fwd, vel, velocityUnits::pct);

    bRight.spin(directionType::fwd, vel, velocityUnits::pct);
    bLeft.spin(directionType::fwd, vel, velocityUnits::pct);
}


void Drivetrain::setRightVel(double vel)
{
  realVelocityFR = vel;
  realVelocityBR = vel;
}
void Drivetrain::setLeftVel(double vel)
{
  realVelocityFL = vel;
  realVelocityBL = vel;
}
double Drivetrain::getRightReal()
{
  return realVelocityFR;
}
double Drivetrain::getLeftReal()
{
  return realVelocityFL;
}
