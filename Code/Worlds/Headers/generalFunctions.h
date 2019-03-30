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
double median(vector<double> data)
{
  if ((data[0] < data[1] && data[1] < data[2]) || (data[2] < data[1] && data[1] < data[0]))
      return data[1];

   // Checking for a
   else if ((data[1] < data[0] && data[0] < data[2]) || (data[2] < data[0] && data[0] < data[1]))
      return data[0];

   else
      return data[2];
}
  static vector<double> filter(3);
  int run = 0;
double getAngle()
{

//    return ((- invertedGyro.value(analogUnits::range12bit))/10.0);
    if(run < 3)
      run++;
    filter[run-1] = ((- invertedGyro.value(analogUnits::range12bit))/10.0);
    double angle = 0;
    if(run == 3 ) {
      angle = median(filter);

      filter[0] = filter[1];
      filter[1] = filter[2];
      filter[2] = angle;

      //filter.erase(filter.begin());
    }
    fstream file("test.csv", fstream::app);
    file<<filter[0]<<","<< filter[1]<<","<<filter[2]<<","<<angle<<endl;
    file.close();
    Brain.Screen.printAt(30, 150, "%.2f %.2f %.2f %.2f", filter[0], filter[1], filter[2], angle);

    return angle;
}
