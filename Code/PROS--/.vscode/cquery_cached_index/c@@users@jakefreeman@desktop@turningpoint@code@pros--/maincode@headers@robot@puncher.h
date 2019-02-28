#include "robot-config.h"

using namespace vex;
using namespace std;

class Puncher
{
  motor& puncher;
  motor& angler;
  limit& limter;
  Intake& intake;
  LineSensor& lineS;
public:
  Puncher(motor& p1, motor& a1, Intake& in, LineSensor& liney) :
  puncher(p1), angler(a1), intake(in), lineS(liney) {}

  void setAngle(double angle)
  {

  }

  void firePuncher()
  {
    puncher.spin(directionType::fwd);
    while(limter.pressing());
    while(!limter.pressing());
    puncher.stop();

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
