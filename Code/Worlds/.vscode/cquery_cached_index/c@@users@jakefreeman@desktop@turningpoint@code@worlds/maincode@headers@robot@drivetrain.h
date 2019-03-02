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

    int driveTask()
    {
      pros::Controller cont (E_CONTROLLER_MASTER);
      while(true)
      {
        int leftSide = cont.get_analog(ANALOG_LEFT_Y);
        int rightSide = cont.get_analog(ANALOG_RIGHT_Y);
        FrontLeft.move(leftSide);
        FrontRight.move(rightSide);
        BackRight.move(rightSide);
        BackLeft.move(leftSide);
        delay(10);
      }
    }

    void drive(double distance, double speed)
    {
      double target = distance/(4 * 3.1415) * 360;

      double kp = 0, P = 0;
      double ki = 0, I = 0;
      double kd = 0, D = 0;
      double error = 0, lastError = 0;
      double motorPower = 0;
      double start = FrontRight.get_position();

      double thresHold = 20;
      while(true)
      {
        error = FrontRight.get_position() - (start);
        P = kp * error;

        if(error < 0)
        {
            I += ki * error;
        }

        D = kd * (error - lastError);

        motorPower = abs(P) + abs(I) - abs(D);
        if(motorPower > abs(speed))
          motorPower = abs(speed);
        if(error < 0) motorPower *= -1;
          setDrive(motorPower);
      }
      setDrive(0);
    }

    void turnTo(double degrees, double speed)
    {
      double error = degrees - gyro.getAngle();

      if (abs(error) > 180)
        error = sign(error) * (abs(error) - 180);

      if (error < 0)
        turnToLeft(error, speed);
      else
        turnToRight(error, speed);
    }

    void setDrive(int velocity)
    {
      FrontRight.move_velocity(velocity);
      FrontLeft.move_velocity(velocity);
      BackLeft.move_velocity(velocity);
      BackRight.move_velocity(velocity);
    }

    void setDrive(int rightVel, int leftVel)
    {
      FrontRight.move_velocity(rightVel);
      FrontLeft.move_velocity(leftVel);
      BackLeft.move_velocity(leftVel);
      BackRight.move_velocity(rightVel);
    }

    void setBrakeMode(const motor_brake_mode_e mode)
    {
        FrontRight.set_brake_mode(mode);
        FrontLeft.set_brake_mode(mode);
        BackLeft.set_brake_mode(mode);
        BackRight.set_brake_mode(mode);
    }

    bool isDriving()
    {
        if (abs(FrontRight.get_actual_velocity()) < 5 &&
            abs(BackRight.get_actual_velocity()) < 5 &&
            abs(FrontLeft.get_actual_velocity()) < 5 &&
            abs(BackLeft.get_actual_velocity()) < 5)
            return false;
        else
          return true;
    }


};
