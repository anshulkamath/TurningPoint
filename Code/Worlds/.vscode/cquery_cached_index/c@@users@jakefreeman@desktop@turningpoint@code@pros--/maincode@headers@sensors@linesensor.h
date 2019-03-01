#include "pros/adi.hpp"
using namespace pros;


class LineSensor
{
  int threshHold = 220;
  pros::ADILineSensor & Line;
 public:
  LineSensor(pros::ADILineSensor& Liney) : Line(Liney) {}

  bool isBall()
  {
    return (threshHold > Line.get_value());
  }
};
