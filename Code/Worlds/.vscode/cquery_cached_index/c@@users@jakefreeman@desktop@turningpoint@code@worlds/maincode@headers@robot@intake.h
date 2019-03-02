#pragma once
#include "../../../include/api.h"
#include "../Sensors/Linesensor.h"

class Intake
{
  Motor& intake;
  LineSensor& line, puncherSensor;
  bool inUse = false;
public:
  int intakeMode = 0;

  Intake(Motor& inty, LineSensor& liney, LineSensor& puncherSensor1)
  : intake(inty), line(liney), puncherSensor(puncherSensor1)
  {

  }


  int intakeTask()
  {
    while(true)
    {
      bool puncherLoaded = puncherSensor.isBall();
      if(controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_L1))
      {
        intake.move_velocity(100);
      }else if(puncherLoaded){

      }else{
        if(controller_get_digital(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_L2))
        {
          intake.move_velocity(-100);
        }else if(line.isBall())
        {
          intake.move_velocity(100);
        }else{
          intake.move_velocity(0);
        }
      }
      pros::delay(10);
    }
  }

};
