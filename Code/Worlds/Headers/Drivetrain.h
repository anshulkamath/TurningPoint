#pragma once
#include <math.h>
#include <stdlib.h>

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

    // Tasks
    static int driveTask();

    // Setting Drive Functions
    void setDrive(int vel);
    void setDrive(int rightVel, int leftVel);
    void drivePID(double distance, double speed, int accelCap = 2);
    double getTurnLimiter();

    // Autonomous Functions
    void drive(double inches, int speed);
    void turnToRight(double deg, int speed);
    void turnToLeft(double deg, int speed);
    void turnTo(double deg, int speed);
};
