class Gyroscope
{
    gyro reg, inverse;
  public:
    Gyroscope(gyro reg1, gyro inverse1) { reg = reg1; inverse = inverse1; }
    
    double getAngle()
    {
      return (reg.value(analogUnits::range12bit) - reg.value(analogUnits::range12bit))/20;
    }
};
