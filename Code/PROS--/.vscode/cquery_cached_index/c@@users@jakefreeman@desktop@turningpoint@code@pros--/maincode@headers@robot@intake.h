#include "../Sensors/Linesensor.h"
class Intake
{
  motor& indexer;
  motor& intake;
  LineSensor& line, puncherSensor;
  bool inUse = false;
public:
  int intakeMode = 0;

  Intake(Motor& ind, Motor& inty, LineSensor& liney, LineSensor& puncherSensor1)
  {
    indexer = ind;
    intake = inty;
    line = liney;
    punsherSensor = puncherSensor1;
  }


  int intakeTask()
  {
    while(true)
    {
      if(line.isBall() && !inUse)
      {
        indexer.spin(direction::rev);
      }

      task::sleep(10);
    }
  }

  void releaseBall()
  {

  }
};
