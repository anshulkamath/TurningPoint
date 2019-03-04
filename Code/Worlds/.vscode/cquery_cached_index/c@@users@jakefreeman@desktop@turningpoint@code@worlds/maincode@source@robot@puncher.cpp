#include "../../Headers/Robot/Puncher.h"

void Puncher::firePuncher()
{
  puncher.move_velocity(100);
  while(limter.get_value());
  while(!limter.get_value());
  puncher.move_velocity(0);

}
void Puncher::waitForBall()
{
  while(!lineS.isBall());
}

void Puncher::fire(double angle1)
{
  setAngle(angle1);
  waitForBall();
  firePuncher();
}

void Puncher::fire(double angle1, double angle2)
{
  setAngle(angle1);
  firePuncher();
  intake.releaseBall();

  setAngle(angle2);
  waitForBall();
  firePuncher();
}
