#include "../../Headers/Robot/Intake.h"

void Intake::intakeTask(void * params)
{
  Controller cont(E_CONTROLLER_MASTER);
  while(true)
  {
    bool puncherLoaded = puncherSensor.isBall();
    if(cont.get_digital( E_CONTROLLER_DIGITAL_L1))
    {
      intake.move_velocity(100);
    }
    else if(puncherLoaded)
    {

    }
    else
    {
      if(cont.get_digital(E_CONTROLLER_DIGITAL_L2))
      {
        intake.move_velocity(-100);
      }
      else if(line.isBall())
      {
        intake.move_velocity(100);
      }else{
        intake.move_velocity(0);
      }
    }
    pros::delay(10);
  }
}
void Intake::runIntake(int state)
{
  switch (state)
  {
    case -1:
      intake.move_velocity(-100);
      break;
    case 0:
      intake.move_velocity(0);
      break;
    case 1:
      intake.move_velocity(100);
      break;
  }
}
