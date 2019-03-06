#pragma once
#include "robot-config.h"

using namespace vex;

class Drivetrain
{
  private:
    // Objects for class to manage
    motor& fRight;
    motor& fLeft;
    motor& bRight;
    motor& bLeft;

    // Drive variables
    int rightDrive;
    int leftDrive;
    int turnLimiter;

    double realVelocityFR;
    double realVelocityFL;
    double realVelocityBR;
    double realVelocityBL;

  public:
    // Creating Drivetrain class
    Drivetrain(motor m1, motor m2, motor m3, motor m4) : fRight(m1), fLeft(m2), bRight(m3), bLeft(m4)
    {
      rightDrive = 0;
      leftDrive = 0;
      turnLimiter = 1;
    }

    // Tasks
    void driveTask();

    // Setting Drive Functions
    void setDrive(int vel);
    void setDrive(int rightVel, int leftVel);

    // Autonomous Functions
    void drive(double inches, int speed);
    void turnToRight(double deg, int speed);
    void turnToLeft(double deg, int speed);
    void turnTo(double deg, int speed);
};
