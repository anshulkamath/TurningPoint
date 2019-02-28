#include "pros/adi.hpp"
using namespace pros;

class Gyroscope
{
    pros::ADIGyro reg, inverse;
  public:
    Gyroscope(pros::ADIGyro reg1, pros::ADIGyro inverse1) : reg(reg1), inverse(inverse1) {}

    double getAngle()
    {
      return (reg.get_value() - inverse.get_value())/2;
    }
};
