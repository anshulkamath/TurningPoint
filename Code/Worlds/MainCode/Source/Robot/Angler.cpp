#include "../../Headers/Robot/Angler.h"

boolean setAngle(double angle)
{
  double P = 0, kp = 0;
  double error = 0;
  double anglePower = 0;

  while(abs(error) > 10 && abs(lError) > 10)
  {
    error = angle - pot.getValue();
    P = error * kp;

    angler.move_velocity(P);
    delay(100);
  }

}
