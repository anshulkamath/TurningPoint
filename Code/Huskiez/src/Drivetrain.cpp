#pragma once
#include "../headers/Drivetrain.h"
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
    if(fabs(lastError) < 2 && fabs(error) < 1) break;
}
  setDrive(0);
}

void Drivetrain::turnTo(double angle, int speed)
{
  double kP = .416*1.025, P = 0;
  double kI = 0.0, I = 0;
  double kD = 0.25*1.025, D = 0;

  int iCap = 2;
  int iThresh = 15;

  double error = 100, lError = 0;
  double motorPower = 0;

  FrontRight.resetRotation();

  // File stuff
  {
      fstream file1(string("angle-54")  + string(".csv"), fstream::app);
      //    Brain.SDcard.appendFile("filename.txt",buff,100);
      file1<<",Target,Current,Error,P,D,I,Power"<<endl;
      file1.close();
  }

  int t = 0;
  // double stblConst = 0;
  // double init = getAngle();
  // int maxError = 100;
  while(true)//abs(error) > 0 || abs(lError) > 0 || abs(motorPower) >= 3)
  {
      fstream file1(string("angle-54")  + string(".csv"), fstream::app);
      t++;
      lError = error;
      error = angle - getAngle();

      P = error * kP;
      if (fabs(error) < iThresh)
          I += error * kI;
      D = kD * (error - lError);

      if (fabs(I) > iCap)
          I = sgn(I) * iCap;

      motorPower = P + I + D;



      if(fabs(motorPower) > abs(speed))
          motorPower = speed * sgn(motorPower);

      BackLeft.spin(directionType::fwd, motorPower, velocityUnits::pct);
      BackRight.spin(directionType::fwd, -motorPower, velocityUnits::pct);
      FrontRight.spin(directionType::fwd, -motorPower, velocityUnits::pct);
      FrontLeft.spin(directionType::fwd, motorPower, velocityUnits::pct);

      lError = error;

      Brain.Screen.printAt(0, 30, "Angle: %.2f", getAngle());

      file1<<t<<","<<angle<<","<<getAngle()<<","<<error<<","<<(P)<<","<<(D)<<","<<(I)<<","<<motorPower<<","<<FrontRight.rotation(rotationUnits::deg)<<","<<FrontRight.torque(torqueUnits::Nm)<<","<<getAngle1()<<endl;
      file1.close();
//if(abs(error) <= 0.01 && abs(lError) <= .01 && abs(motorPower) < 3) break; // Break statement

      task::sleep(10);
  }
  setDrive(0);
}

void Drivetrain::drivePID(double distance, double speed, int accelCap, int decelCap, int timeComplete, double angler)
{
  // distance *= 36/40;
  double kP = .4175, P = 0;
  double kI = 0.03, I = 0;
  double kD = 2.5, D = 0;
  double error = 100, lError = 0;
  double motorPower = 0, prevMotorPower = 0;
  double iCap = 5;
  distance /= (4.0*3.1415);
  distance /= 2.33333333;
  {
      fstream file1(string("drivePID")  + string(".csv"), fstream::app);
      //    Brain.SDcard.appendFile("filename.txt",buff,100);
      file1<<",Target,Current,Error,P,D,I, GyroAdjust, Power"<<endl;
      file1.close();
  }
  distance *= 360;
  FrontRight.resetRotation();
  FrontLeft.resetRotation();


  int t = 0;
  double stblConst = .5;//2.0/360.0 * 1.66;
  double init = getAngle();
  if(angler != -360)
      init = angler;

  Brain.resetTimer();
  while((fabs(error) >= 0.007 || fabs(lError) >= 0.007) && Brain.timer(timeUnits::msec) < timeComplete)// || abs(motorPower) >= 3)//true)//distance > FrontRight.rotation(rotationUnits::deg))
  {
      fstream file1(string("drivePID")  + string(".csv"), fstream::app);
      t++;
      error = distance - getRotationFront();
      P = kP * error;
      D = -kD * (error - lError);


      if(fabs(error) < 20)
          I += kI * error;

      if (fabs(I) >= iCap) I = sgn(I) * iCap;

      motorPower = (P) + (I) - (D);
      int signBase = sgn(motorPower);
      motorPower = fabs(motorPower);
      if(motorPower > speed) motorPower = speed;

      if(motorPower + accelCap > prevMotorPower) motorPower = prevMotorPower + accelCap;

      if(prevMotorPower - decelCap > motorPower) motorPower = prevMotorPower - decelCap;

      if(motorPower > speed) motorPower = speed;

      double leftAdjustPwr = -stblConst * (getAngle() - init);

      setDrive(motorPower * signBase - leftAdjustPwr, motorPower * signBase + leftAdjustPwr);
      prevMotorPower = fabs(motorPower);
      lError = error;
      Brain.Screen.printAt(30, 30, "%.2f, %.2f,%.2f", getRotationFront(), distance, motorPower);
      file1<<t<<","<<distance<<","<<getRotationFront()<<","<<error<<","<<fabs(P)<<","<<-fabs(D)<<","<<fabs(I)<<","<<leftAdjustPwr<<","<<sgn(error) * motorPower<<endl;
      task::sleep(25);
      file1.close();
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
double Drivetrain::getRotationFront()
{
return (fRight.rotation(rotationUnits::deg) + fLeft.rotation(rotationUnits::deg)) / 2;
}

void Drivetrain::setBrakeMode(vex::brakeType type)
{
  FrontRight.setStopping(type);
  BackLeft.setStopping(type);
  BackRight.setStopping(type);
  FrontLeft.setStopping(type);
}
