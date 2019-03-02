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

    int sign(double val);

    void turnToLeft(double degrees, double speed);

    void turnToRight(double degrees, double speed);

  public:
    Drivetrain (pros::Motor m1, pros::Motor m2, pros::Motor m3, pros::Motor m4, Gyroscope gyro1)
    : FrontRight(m1), FrontLeft(m2), BackRight(m3), BackLeft(m4), gyro(gyro1) {}

    void driveTask(void * parms);

    void drive(double distance, double speed);

    void turnTo(double degrees, double speed);

    void setDrive(int velocity);

    void setDrive(int rightVel, int leftVel);

    void setBrakeMode(const motor_brake_mode_e mode);

    bool isDriving();


};
