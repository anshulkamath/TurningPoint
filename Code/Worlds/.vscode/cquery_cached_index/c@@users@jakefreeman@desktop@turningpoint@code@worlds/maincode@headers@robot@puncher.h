#include "../../../include/api.h"
#include "../Sensors/lineSensor.h"
#include "../Robot/Intake.h"

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

  void firePuncher();
  void waitForBall();

  // Fire with two different sets of arguments in order to have
  // a double shot as well as a single shot
  void fire(double angle1);
  void fire(double angle1, double angle2);


};
