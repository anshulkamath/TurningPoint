#include "../../Headers/Robot/Angler.h"

bool Angler::setAngle(double angle)
{
  double P = 0, kp = 0;
  double error = 100, lError = 100;
  double anglePower = 0;

  while(fabs(error) > 10 && fabs(lError) > 10)
  {
    error = angle - pot.get_value();
    P = error * kp;

    angler.move_velocity(P);
    delay(50);
  }
  return true;
}
