#include "robot-config.h"
#include "../Sensors/lineSensor.h"
#include "../Robot/Intake.h"
#include "pros/motors.hpp"
#include "pros/ADI.hpp"

using namespace std;

class Puncher
{
  Motor& puncher;
  Motor& angler;
  pros::ADIDigitalIn& limter;
  ::Intake& intake;
  LineSensor& lineS;
public:
  Puncher(Motor& p1, Motor& a1, ::Intake& in, LineSensor& liney,
    pros::ADIDigitalIn& limter1) :
  puncher(p1), angler(a1), intake(in), lineS(liney), limter(limter1) {}

  void setAngle(double angle)
  {

  }

  void firePuncher()
  {
    puncher.move_velocity(100);
    while(limter.get_value());
    while(!limter.get_value());
    puncher.move_velocity(0);

  }
  void waitForBall()
  {
    while(!lineS.isBall());
  }
  void fire(double angle1, double angle2)
  {
    setAngle(angle1);
    firePuncher();
    intake.releaseBall();

    setAngle(angle2);
    waitForBall();
    firePuncher();

  }


};
