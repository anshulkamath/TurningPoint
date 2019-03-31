#include "../cleanConfig.h"
#include "../Source/vars.cpp"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
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
  return angle;
}
