#pragma once
#include <math.h>
#include <stdlib.h>
#include "generalFunctions.h"
#include "../Source/vars.cpp"
using namespace vex;

class Drivetrain
{
  private:
    // Objects for class to manage
    motor fRight;
    motor fLeft;
    motor bRight;
    motor bLeft;
    gyro reg;
    gyro inverted;

    // Drive variables
    int rightDrive;
    int leftDrive;

    double realVelocityFR;
    double realVelocityFL;
    double realVelocityBR;
    double realVelocityBL;

  public:
    // Creating Drivetrain class
    Drivetrain(motor frontRight, motor frontLeft, motor backRight, motor backLeft, gyro reg1, gyro inverted1)
    : fRight(frontRight), fLeft(frontLeft), bRight(backRight), bLeft(backLeft), reg(reg1), inverted(inverted1)
    {
      rightDrive = 0;
      leftDrive = 0;
    }

    // Setting Drive Functions
    void setDrive(int vel);
    void setDrive(int rightVel, int leftVel);
    void turnToSlow(double angle);
    void drivePID(double distance, double speed, int accelCap = 4, int decelCap = 3, int timeComplete = 100000, double angler = -360);
    double getTurnLimiter();
    void setRightVel(double vel);
    void setLeftVel(double vel);
    double getRightReal();
    double getLeftReal();

    void straightDrive(double distance, double speed, double init);

    void brake(double power = 400)
    {
      int leftSgn = sgn(fLeft.velocity(velocityUnits::pct));
        int rightSgn = -sgn(fRight.velocity(velocityUnits::pct));
        setDrive(power*rightSgn, leftSgn*power);
        task::sleep(20);
        setDrive(0, 0);
    }

    void slipAdjust(bool right, bool left, double speedLeft = 100, double speedRight = 100)
    {
      bool fR = false, fL = false, bL = false, bR = false;
      double thresh = 0.18;
      setDrive((right == 0 ? -1 : 1) * 100, (left == 0 ? -1 : 1) * 100);
      while(!fR || !fL || !bL || !bR)
      {
        if(fabs(fRight.torque(torqueUnits::Nm)) >= thresh)
        {
          fR = true;
          fRight.stop();
        }
        if(fabs(fLeft.torque(torqueUnits::Nm)) >= thresh)
        {
          fL = true;
          fLeft.stop();
        }
        if(fabs(bLeft.torque(torqueUnits::Nm)) >= thresh)
        {
          bL = true;
          bLeft.stop();
        }
        if(fabs(bRight.torque(torqueUnits::Nm)) >= thresh)
        {
          bR = true;
          bRight.stop();
        }
      }

    }

    double getRotationFront();


    // Autonomous Functions
    void drive(double inches, int speed);
    void turnToRight(double deg, int speed);
    void turnToLeft(double deg, int speed);
    void turnTo(double deg, int speed, int timeComplete = 1500);
};
