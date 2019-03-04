#include "../../../include/api.h"
#include "../Sensors/lineSensor.h"

class Angler
{
  private:
    Motor& angler;
    ADIAnalogIn& pot;

  public:
    Angler(Motor& angle, ADIAnalogIn& poten) : angler(angle), pot(poten) {}

    bool setAngle(double angle);
};
