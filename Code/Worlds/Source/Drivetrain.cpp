#pragma once
#include "../Headers/Drivetrain.h"
#include <math.h>

int Drivetrain::driveTask()
{
  while(true)
  {
    int fLeftValue = 0,
        fRightValue = 0,
        bLeftValue = 0,
        bRightValue = 0;

        // Constant Straight Drive Control
        if (Controller.ButtonR1.pressing())
            fLeftValue = fRightValue = bLeftValue = bRightValue = 60;
        else if (Controller.ButtonR2.pressing())
            fLeftValue = fRightValue = bLeftValue = bRightValue = -60;

        // Tank Drive Controls
        else
        {
            if (abs(Controller.Axis2.value()) > 20)
            {
                fRightValue = Controller.Axis2.value();
                bRightValue = fRightValue;
            }
            if (abs(Controller.Axis3.value()) > 20)
            {
                fLeftValue  = Controller.Axis3.value();
                bLeftValue = fLeftValue;
            }
        }

        if(abs(abs(Controller.Axis2.value()) - abs(Controller.Axis3.value())) > 20)
        {
            fLeftValue = bLeftValue = Controller.Axis3.value();
            fRightValue = bRightValue = Controller.Axis2.value();
        }

        // Throttling Turns
        if(abs(Controller.Axis2.value() - Controller.Axis3.value()) > 50)
        {
            fLeftValue = bLeftValue *= turnLimiter;
            fRightValue = bRightValue *= turnLimiter;
        }


        if (Controller.ButtonRight.pressing())
        {
            bLeftValue = fLeftValue = 30;
            fRightValue = bRightValue = -30;
        }
        else if (Controller.ButtonLeft.pressing())
        {
            bLeftValue = fLeftValue = -30;
            fRightValue = bRightValue = 30;
        }

        // Sets Motor Powers
        fLeft.spin(directionType::fwd, fLeftValue, velocityUnits::pct);
        fRight.spin(directionType::fwd, fRightValue, velocityUnits::pct);
        bLeft.spin(directionType::fwd, bLeftValue, velocityUnits::pct);
        bRight.spin(directionType::fwd, bRightValue, velocityUnits::pct);

        task::sleep(25);
    }
    return 0;
  }
}
