#include "../../Headers/Robot/DriveTrain.h"

int Drivetrain::sign(double val)
{
  return val < 0 ? -1 : 1;
}

void Drivetrain::turnToLeft(double degrees, double speed)
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

void Drivetrain::turnToRight(double degrees, double speed)
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



void Drivetrain::drive(double distance, double speed)
{
  double target = distance/(4 * M_PI) * 360;

  double kp = 0, P = 0;
  double ki = 0, I = 0;
  double kd = 0, D = 0;
  double error = 0, lastError = 0;
  double motorPower = 0;
  double start = FrontRight.get_position();

  double threshold = 20;
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

    delay(25);
    lastError = error;
  }
  setDrive(0);
}


void Drivetrain::turnTo(double degrees, double speed)
{
  double error = degrees - gyro.getAngle();

  if (abs(error) > 180)
    error = sign(error) * (abs(error) - 180);

  if (error < 0)
    turnToLeft(error, speed);
  else
    turnToRight(error, speed);
}

void Drivetrain::setDrive(int velocity)
{
  FrontRight.move_velocity(velocity);
  FrontLeft.move_velocity(velocity);
  BackLeft.move_velocity(velocity);
  BackRight.move_velocity(velocity);
}

void Drivetrain::setDrive(int rightVel, int leftVel)
{
  FrontRight.move_velocity(rightVel);
  FrontLeft.move_velocity(leftVel);
  BackLeft.move_velocity(leftVel);
  BackRight.move_velocity(rightVel);
}

void Drivetrain::setBrakeMode(const motor_brake_mode_e mode)
{
  FrontRight.set_brake_mode(mode);
  FrontLeft.set_brake_mode(mode);
  BackLeft.set_brake_mode(mode);
  BackRight.set_brake_mode(mode);
}

bool Drivetrain::isDriving()
{
    if (abs(FrontRight.get_actual_velocity()) < 5 &&
        abs(BackRight.get_actual_velocity()) < 5 &&
        abs(FrontLeft.get_actual_velocity()) < 5 &&
        abs(BackLeft.get_actual_velocity()) < 5)
        return false;
    else
      return true;
}
void Drivetrain::driveTask()
{
  pros::Controller cont (E_CONTROLLER_MASTER);
  int leftSide = 0;
  int rightSide = 0;
  double turnLimiter = 1;
  while(true)
  {
    // Introducing deadzone
    if (cont.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) > 10)
      leftSide = cont.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
    if (cont.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y) > 10)
      rightSide = cont.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);
    else
    {
      leftSide = 0;
      rightSide = 0;
    }

    // Limiting turns
    if (abs(abs(rightSide) - abs(leftSide)) > 50)
    {
      rightSide *= turnLimiter;
      leftSide *= turnLimiter;
    }

    // Slow drive mode
    if (cont.get_digital(E_CONTROLLER_DIGITAL_R1))
      rightSide = leftSide = 60 * 127/100;
    else if (cont.get_digital(E_CONTROLLER_DIGITAL_R2))
      rightSide = leftSide = -60 * 127/100;


    // Brake mode
    if (isDriving())
      setBrakeMode(E_MOTOR_BRAKE_COAST);
    else
      setBrakeMode(E_MOTOR_BRAKE_BRAKE);

    setDrive(rightSide, leftSide);
    delay(10);
  }
}
