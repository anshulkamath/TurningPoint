#include "../cleanConfigurations.h"
using namespace vex;
using namespace std;

void sleep (int time)
{
    task::sleep(time);
}

int sgn(double v)
{
  return v < 0 ? -1 : 1;
}

double getAngle()
{
    return (gyroscope.value(analogUnits::range12bit) - invertedGyro.value(analogUnits::range12bit))/20 - 0;
}
