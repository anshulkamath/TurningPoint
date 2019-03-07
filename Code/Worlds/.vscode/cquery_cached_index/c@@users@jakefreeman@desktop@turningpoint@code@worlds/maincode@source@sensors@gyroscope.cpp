#include "../../Headers/Sensors/Gyroscope.h"

double Gyroscope::getAngle()
{
  return (reg.get_value() - inverse.get_value())/2;
}
