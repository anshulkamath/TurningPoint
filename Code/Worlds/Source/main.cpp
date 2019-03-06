#include "robot-config.h"
#include "/Users/anshulkamath/Robotics/Turning Point/TurningPointGit/Code/Worlds/Headers/"
using namespace vex;

void sleep (int time)
{
    task::sleep(time);
}

void pre_auton( void )
{

}

void autonomous( void )
{

}

void usercontrol( void )
{
  while (1)
  {
    sleep(20);
  }
}

int main()
{
    pre_auton();
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );

    while(1)
    {
      sleep(100);
    }

}
