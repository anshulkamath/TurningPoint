#pragma once
#include "robot-config.h"
#include "/Users/anshulkamath/Robotics/Turning Point/TurningPointGit/Code/Worlds/Headers/Autonomous/AutonSelector.h"
#include "/Users/anshulkamath/Robotics/Turning Point/TurningPointGit/Code/Worlds/Headers/Autonomous/Functions.h"
#include "/Users/anshulkamath/Robotics/Turning Point/TurningPointGit/Code/Worlds/Headers/Robot/DriveTrain.h"
#include "/Users/anshulkamath/Robotics/Turning Point/TurningPointGit/Code/Worlds/Headers/Robot/Puncher.h"
#include "/Users/anshulkamath/Robotics/Turning Point/TurningPointGit/Code/Worlds/Headers/Sensors/Accelerometer.h"
#include "/Users/anshulkamath/Robotics/Turning Point/TurningPointGit/Code/Worlds/Headers/Sensors/Gyroscope.h"

vex::competition    Competition;

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
    vex::task::sleep(20);
  }
}

int main()
{
    pre_auton();

    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );

    while(1)
    {
      vex::task::sleep(100);
    }

}
