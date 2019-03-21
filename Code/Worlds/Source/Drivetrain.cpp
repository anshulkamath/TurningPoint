#include "../Headers/Drivetrain.h"
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;




void Drivetrain::turnTo(double angle, int speed)
{
  double kP = 1.2, P = 0;
  double kI = .0, I = 0;
  double kD = 1.8, D = 0;
  double error = 100, lError = 0;
  double motorPower = 0, prevMotorPower = 0;
  double iCap = 5;


  Controller.rumble("-.-.-");
  FrontRight.resetRotation();
      fstream file1(string("angle-334")  + string(".csv"), fstream::app);
    file1<<",Target,Current,Error,P,D,I,Power"<<endl;
    int t = 0;
    double stblConst = 2;
    double init = getAngle();
  while(true)//abs(error) >= 1 || abs(lError) >= 1 || abs(motorPower) >= 10)//true)//distance > FrontRight.rotation(rotationUnits::deg))
  {
    t++;
    error = angle - getAngle();
    P = kP * error;
    D = -kD * (error - lError);


    if(error < 20 && I < iCap)
      I += kI * error;
    else if (I >= iCap) I = iCap;

    motorPower = (P) + (I) - (D);
    int signBase = sgn(motorPower);
    motorPower = abs(motorPower);
    if(motorPower > speed) motorPower = speed;

    setDrive(-motorPower * signBase, motorPower * signBase);
    prevMotorPower = abs(motorPower);
    lError = error;
    Brain.Screen.printAt(30, 30, "%.2f, %.2f,%.2f", getAngle(), angle, motorPower);
  // file1<<t<<","<<distance<<","<<FrontRight.rotation(rotationUnits::deg)<<","<<error<<","<<abs(P)<<","<<-abs(D)<<","<<abs(I)<<","<<sgn(error) * motorPower<<endl;
   task::sleep(25);
  }
  setDrive(0);
}

void Drivetrain::drivePID(double distance, double speed, int accelCap)
{
  double kP = .25, P = 0;
  double kI = .001, I = 0;
  double kD = .5, D = 0;
  double error = 100, lError = 0;
  double motorPower = 0, prevMotorPower = 0;
  double iCap = 5;
  distance /= (4.0*3.1415);

  Controller.rumble("-.-.-");
  distance *= 360;
  FrontRight.resetRotation();
      fstream file1(string("angle-334")  + string(".csv"), fstream::app);
    file1<<",Target,Current,Error,P,D,I,Power"<<endl;
    int t = 0;
    double stblConst = 2;
    double init = getAngle();
  while(abs(error) >= 1 || abs(lError) >= 1 || abs(motorPower) >= 10)//true)//distance > FrontRight.rotation(rotationUnits::deg))
  {
    t++;
    error = distance - FrontRight.rotation(rotationUnits::deg);
    P = kP * error;
    D = -kD * (error - lError);


    if(error < 20 && I < iCap)
      I += kI * error;
    else if (I >= iCap) I = iCap;

    motorPower = (P) + (I) - (D);
    int signBase = sgn(motorPower);
    motorPower = abs(motorPower);
    if(motorPower > speed) motorPower = speed;

    if(motorPower + accelCap > prevMotorPower) motorPower = prevMotorPower + accelCap;

    if(prevMotorPower - accelCap > motorPower) motorPower = prevMotorPower - accelCap;

    if(motorPower > speed) motorPower = speed;

    double leftAdjustPwr = stblConst * (getAngle() - init);

    setDrive(motorPower * signBase - leftAdjustPwr, motorPower * signBase + leftAdjustPwr);
    prevMotorPower = abs(motorPower);
    lError = error;
    Brain.Screen.printAt(30, 30, "%.2f, %.2f,%.2f", FrontRight.rotation(rotationUnits::deg), distance, motorPower);
   file1<<t<<","<<distance<<","<<FrontRight.rotation(rotationUnits::deg)<<","<<error<<","<<abs(P)<<","<<-abs(D)<<","<<abs(I)<<","<<sgn(error) * motorPower<<endl;
   task::sleep(25);
  }
  setDrive(0);
}

void Drivetrain::setDrive(int rightVel, int leftVel)
{
  fRight.setStopping(brakeType::brake);
    fLeft.setStopping(brakeType::brake);
      bRight.setStopping(brakeType::brake);
        bLeft.setStopping(brakeType::brake);
  fRight.spin(directionType::fwd, rightVel, velocityUnits::pct);
  fLeft.spin(directionType::fwd, leftVel, velocityUnits::pct);
  bRight.spin(directionType::fwd, rightVel, velocityUnits::pct);
  bLeft.spin(directionType::fwd, leftVel, velocityUnits::pct);
}

void Drivetrain::setDrive(int vel)
{
  fRight.setStopping(brakeType::brake);
    fLeft.setStopping(brakeType::brake);
      bRight.setStopping(brakeType::brake);
        bLeft.setStopping(brakeType::brake);
  fRight.spin(directionType::fwd, vel, velocityUnits::pct);
  fLeft.spin(directionType::fwd, vel, velocityUnits::pct);
  bRight.spin(directionType::fwd, vel, velocityUnits::pct);
  bLeft.spin(directionType::fwd, vel, velocityUnits::pct);
}
