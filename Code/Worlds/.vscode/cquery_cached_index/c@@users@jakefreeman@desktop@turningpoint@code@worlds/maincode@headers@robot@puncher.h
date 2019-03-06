#include "../../../include/api.h"
#include "../Sensors/lineSensor.h"
#include "../Robot/Intake.h"
#include "../Robot/Angler.h"
#include "../../Source/Robot/Angler.cpp"

using namespace std;

class Puncher
{
  pros::Motor puncher;
  Angler angler;
  pros::ADIDigitalIn limter;
  ::Intake intake;
  LineSensor lineS;
public:
  Puncher(pros::Motor p1, Angler a1, ::Intake in, LineSensor liney,
    pros::ADIDigitalIn limter1) :
  puncher(p1), angler(a1), intake(in), lineS(liney), limter(limter1) {}

  void puncherDriveTask()
  {
     pros::Controller cont (E_CONTROLLER_MASTER);
    while(true)
    {
      if(cont.get_digital( E_CONTROLLER_DIGITAL_UP))
      {
      //
      angler.angle(2240);
      firePuncher();
      //  fire(2240, 2445);
        delay(50);
      }
    }
  }

  void firePuncher()
  {
  //  puncher.move_relative(340, 100);
    //delay(1000);
    puncher.move_velocity(100);
    delay(1000);
    puncher.move_velocity(0);

  }
  void waitForBall()
  {
    while(!lineS.isBall());
  }
  void fire(double angle1, double angle2)
  {
    angler.angle(angle1);
    firePuncher();
    //while(!lineS.isBall());

    angler.angle(angle2);
    delay(1000);
    firePuncher();

  }


};
