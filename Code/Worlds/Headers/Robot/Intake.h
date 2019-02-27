class Intake
{
  motor& indexer;
  motor& intake;
  LineSensor& line, puncherSensor;

public:
  int intakeMode = 0;

  Intake(motor& ind, motor& inty, LineSensor& liney, LineSensor& puncherSensor1)
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

      
      task::sleep(10);
    }
  }

  void releaseBall()
  {

  }
};
