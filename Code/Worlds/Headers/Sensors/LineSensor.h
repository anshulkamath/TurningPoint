
class LineSensor
{
  int threshHold = 0;
  vex::line& Line;
 public:
  LineSensor(vex::line& Liney) : Line(Liney) {}

  bool isBall()
  {
    return (threshHold < Line.value(analogUnits::range12bit));
  }
};
