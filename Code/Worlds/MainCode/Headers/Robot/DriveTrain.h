#pragma once

#include "../../../include/api.h"
#include "okapi/api.hpp"
#include <math.h>
#include "../maincode/headers/sensors/Gyroscope.h"
using namespace pros;
using namespace okapi;

class Drivetrain
{
  private:
    pros::Motor FrontRight;
    pros::Motor FrontLeft;
    pros::Motor BackRight;
    pros::Motor BackLeft;
    Gyroscope gyro;

    int sign(double val)
    {
      return val < 0 ? -1 : 1;
    }

    void turnToLeft(double degrees, double speed)
    {

      double kp = 0, P = 0;
      double ki = 0, I = 0;
      double kd = 0, D = 0;
      double error = 0, lastError = 0;
      double motorPower = 0;


      double threshold = 20;
      while(true)
      {
        error = degrees - gyro.getAngle();
        P = kp * error;

        if(error < threshold)
        {
            I += ki * error;
        }

        D = kd * (error - lastError);

        motorPower = abs(P) + abs(I) - abs(D);

        if(motorPower > abs(speed))
          motorPower = abs(speed);

        if(error < 0) motorPower *= -1;
        setDrive(-motorPower, motorPower);
      }
      setDrive(0);
    }

    void turnToRight(double degrees, double speed)
    {

      double kp = 0, P = 0;
      double ki = 0, I = 0;
      double kd = 0, D = 0;
      double error = 0, lastError = 0;
      double motorPower = 0;


      double threshold = 20;
      while(true)
      {
        error = degrees - gyro.getAngle();
        P = kp * error;

        if(error < threshold)
        {
            I += ki * error;
        }

        D = kd * (error - lastError);

        motorPower = abs(P) + abs(I) - abs(D);

        if(motorPower > abs(speed))
          motorPower = abs(speed);

        if(error < 0) motorPower *= -1;
        setDrive(motorPower, -motorPower);
      }
      setDrive(0);
    }

  public:
    Drivetrain (pros::Motor m1, pros::Motor m2, pros::Motor m3, pros::Motor m4, Gyroscope gyro1)
    : FrontRight(m1), FrontLeft(m2), BackRight(m3), BackLeft(m4), gyro(gyro1) {}

    int driveTask();

    void drive(double distance, double speed);

    void turnTo(double degrees, double speed);

    void setDrive(int velocity);

    void setDrive(int rightVel, int leftVel);

    void setBrakeMode(const motor_brake_mode_e mode);

    bool isDriving();


};
