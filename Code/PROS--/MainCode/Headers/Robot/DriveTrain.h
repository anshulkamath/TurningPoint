#include "pros/motors.hpp"
#include "okapi/api.hpp"
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
  public:
    Drivetrain (pros::Motor m1, pros::Motor m2, pros::Motor m3, pros::Motor m4, Gyroscope gyro1)
    : FrontRight(m1), FrontLeft(m2), BackRight(m3), BackLeft(m4), gyro(gyro1)
    {
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

    void turnToLeft(double degrees, double speed)
    {

      double kp = 0, P = 0;
      double ki = 0, I = 0;
      double kd = 0, D = 0;
      double error = 0, lastError = 0;
      double motorPower = 0;


      double thresHold = 20;
      while(true)
      {
        error = degrees - gyro.getAngle();
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


      double thresHold = 20;
      while(true)
      {
        error = degrees - gyro.getAngle();
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
        setDrive(motorPower, -motorPower);
      }
      setDrive(0);
    }
    void turnTo(double degrees, double speed)
    {
      
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


};
