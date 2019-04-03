#pragma once;
#include "../Headers/Drivetrain.h"
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;

void Drivetrain::turnToSlow(double angle)
{
  double lastError = 0, error = 0;
  while(true)
  {
      error = angle - getAngle();
      setDrive(-4, 4);
      task::sleep(10);
      lastError = error;
      if(abs(lastError) < 2 && abs(error) < 1) break;
  }
    setDrive(0);
}

void Drivetrain::turnTo(double angle, int speed, int timeComplete)
{
    // for 90
    double P = 0, I = 0, D = 0;
    double kP, kI, kD;
    int PDCap = 5;
    if (fabs(angle - getAngle()) <= 45)
    {
      kP = .335;
      kI = 0.004;
      kD = .3;
      PDCap = 10;
    }
    else if (fabs(angle - getAngle()) <= 90)
    {
      kP = .45;
      kI = 0.01;
      kD = .42;
    }
    else if (angle  <= 135)
    {
      kP = .57;
      kI = 0.015;
      kD = .55;
    }
    else
    {
      kP = .55;
      kI = 0.01;
      kD = 1;
    }

    int iCap = 102;
    int iThresh = 25;

    double error = 100, lError = 0;
    double motorPower = 0;

    FrontRight.resetRotation();

    // File stuff
    {
        fstream file1(string("angle")  + string(".csv"), fstream::app);
        file1<<",Target,Current,Error,P,D,I,Power"<<endl;
        file1.close();
    }

    int t = 0;
    double init = getAngle();
    double velly = 0;
    int maxError = 100;

    double initialError = angle - init;
    bool first = true;
    Brain.resetTimer();
    while(true)//abs(error) > 0 || abs(lError) > 0 || abs(motorPower) >= 3)
    {
        fstream file1(string("angle")  + string(".csv"), fstream::app);
        t++;
        lError = error;
        error = angle - getAngle();

        P = error * kP;
        if (abs(error) < iThresh)
            I += error * kI;
        D = kD * (error - lError);

        motorPower = P + I + D;

        // if (fabs(error) < PDCap)
        // {
        //   if (first)
        //   {
        //     brake(10);
        //     first = false;
        //   }
        //   else motorPower = I;
        // }


        if(abs(motorPower) > abs(speed))
            motorPower = speed * sgn(motorPower);

        setDrive(-motorPower, motorPower);

        lError = error;

        Brain.Screen.printAt(0, 30, "Angle: %.2f", getAngle());

         file1<<t<<","<<angle<<","<<getAngle()<<","<<error<<","<<(P)<<","<<(D)<<","<<(I)<<","<<motorPower<<","<<FrontRight.rotation(rotationUnits::deg)<<","<<FrontRight.torque(torqueUnits::Nm)<<","<<endl;
        file1.close();
        if(abs(error) <= 1 && abs(lError) <= 1 && fabs(motorPower) <= 1) break; // Break statement


        if(Brain.timer(timeUnits::msec) > timeComplete) break;
        task::sleep(50);
    }
    brake(speed);
    setDrive(0);
}

void Drivetrain::drivePID(double distance, double speed, int accelCap, int decelCap, int timeComplete, double angler)
{
    // distance *= 36/40;
    double kP = .4175, P = 0;
    double kI = 0.015, I = 0;
    double kD = 3.3, D = 0;
    double error = 100, lError = 0;
    double motorPower = 0, prevMotorPower = 0;
    double iCap = 5;
    distance /= (4.0*3.1415);
    distance /= 2.33333333;
    {
        fstream file1(string("drivePID")  + string(".csv"), fstream::app);
        char * buff = "Hello";
        //    Brain.SDcard.appendFile("filename.txt",buff,100);
        file1<<",Target,Current,Error,P,D,I, GyroAdjust, Power"<<endl;
        file1.close();
    }
    distance *= 360;
    FrontRight.resetRotation();
    FrontLeft.resetRotation();


    int t = 0;
    double stblConst = 4.8;//2.0/360.0 * 1.66;
    double percentDone = 0;

    double init = getAngle();
    if(angler != -360)
        init = angler;

    Brain.resetTimer();
    while((abs(error) >= 0.007 || abs(lError) >= 0.007) && Brain.timer(timeUnits::msec) < timeComplete)// || abs(motorPower) >= 3)//true)//distance > FrontRight.rotation(rotationUnits::deg))
    {
        fstream file1(string("drivePID")  + string(".csv"), fstream::app);
        t++;
        error = distance - getRotationFront();
        percentDone = abs(1 - getRotationFront()/distance);

        P = kP * error;
        D = -kD * (error - lError);


        if(abs(error) < 20)
            I += kI * error;

        if (abs(I) >= iCap) I = sgn(I) * iCap;

        motorPower = (P) + (I) - (D);
        int signBase = sgn(motorPower);
        motorPower = abs(motorPower);
        if(motorPower > speed) motorPower = speed;

        if(speed > 40)
        {

          if(motorPower + accelCap > prevMotorPower) motorPower = prevMotorPower + accelCap;

          if(prevMotorPower - decelCap > motorPower) motorPower = prevMotorPower - decelCap;
        }

        if(motorPower > speed) motorPower = speed;

        double leftAdjustPwr = -stblConst * (getAngle() - init);
        leftAdjustPwr *= percentDone;

        setDrive(motorPower * signBase, motorPower * signBase + leftAdjustPwr);
        prevMotorPower = abs(motorPower);
        lError = error;
        Brain.Screen.printAt(30, 30, "%.2f, %.2f,%.2f", getRotationFront(), distance, motorPower);
        file1<<t<<","<<distance<<","<<getRotationFront()<<","<<error<<","<<abs(P)<<","<<-abs(D)<<","<<abs(I)<<","<<leftAdjustPwr<<","<<sgn(error) * motorPower<<endl;
        task::sleep(25);
        file1.close();
    }
    double smallPowerConst = 6, smallPower;
    /*
    while(true)
    {
      smallPower = smallPowerConst;
      error = init -   getAngle();
      if(abs(error) <= .75 && abs(lError) <= .75) break;
      if(error < 0) smallPower *= -1;

      Brain.Screen.printAt(30, 30, "%d             ", gyroscope.value(analogUnits::range12bit));
      Brain.Screen.printAt(30, 60, "%d             ", invertedGyro.value(analogUnits::range12bit));
      Brain.Screen.printAt(30, 90, "%.2f           ", getAngle());
      FrontRight.spin(directionType::fwd, -smallPower, vex::velocityUnits::pct);
      FrontLeft.spin(directionType::fwd, smallPower, vex::velocityUnits::pct);
      BackLeft.spin(directionType::fwd, smallPower, vex::velocityUnits::pct);
          BackRight.spin(directionType::fwd, -smallPower, vex::velocityUnits::pct);
      lError = error;
      task::sleep(20);
    }*/
    fRight.stop(brakeType::hold);
    fLeft.stop(brakeType::hold);
    bRight.stop(brakeType::hold);
    bLeft.stop(brakeType::hold);
    task::sleep(20);
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
void Drivetrain::straightDrive(double distance, double speed, double init)
{
  distance /= (4.0*3.1415);
  distance /= 2.33333333;
  distance *= 360;
  FrontRight.resetRotation();
  FrontLeft.resetRotation();

  fRight.rotateFor(distance, rotationUnits::deg, speed, velocityUnits::pct, false);
  fLeft.rotateFor(distance, rotationUnits::deg, speed, velocityUnits::pct, false);
  bLeft.rotateFor(distance, rotationUnits::deg, speed, velocityUnits::pct, false);
  bRight.rotateFor(distance, rotationUnits::deg, speed, velocityUnits::pct, true);
  double error = 100, lError = 0;
  double motorPower = 0, prevMotorPower = 0;
  double smallPowerConst = 6, smallPower;
  while(true)
  {
    smallPower = smallPowerConst;
    error = init -   angle;
    if(abs(error) <= .75 && abs(lError) <= .75) break;
    if(error < 0) smallPower *= -1;

    FrontRight.spin(directionType::fwd, -smallPower, vex::velocityUnits::pct);
    FrontLeft.spin(directionType::fwd, smallPower, vex::velocityUnits::pct);
    BackLeft.spin(directionType::fwd, smallPower, vex::velocityUnits::pct);
        BackRight.spin(directionType::fwd, -smallPower, vex::velocityUnits::pct);
    lError = error;
    task::sleep(20);
  }
  fRight.stop(brakeType::hold);
  fLeft.stop(brakeType::hold);
  bRight.stop(brakeType::hold);
  bLeft.stop(brakeType::hold);
  task::sleep(20);
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
double Drivetrain::getRotationFront()
{
  return (fRight.rotation(rotationUnits::deg) + fLeft.rotation(rotationUnits::deg)) / 2;
}
