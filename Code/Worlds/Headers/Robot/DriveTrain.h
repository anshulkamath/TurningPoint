using namespace vex;

class Drivetrain
{
  private:
    motor FrontRight;
    motor FrontLeft;
    motor BackRight;
    motor BackLeft;
  public:
    Drivetrain (motor m1, motor m2, motor m3, motor m4) : FrontRight(m1), FrontLeft(m2), BackRight(m3), BackLeft(m4) {}
    void setDrive(int velocity)
    {
      FrontRight.spin(directionType::fwd, velocity, velocityUnits::pct);
      FrontLeft.spin(directionType::fwd, velocity, velocityUnits::pct);
      BackLeft.spin(directionType::fwd, velocity, velocityUnits::pct);
      BackRight.spin(directionType::fwd, velocity, velocityUnits::pct);
    }
    void setDrive(int rightVel, int leftVel)
    {
      FrontRight.spin(directionType::fwd, rightVel, velocityUnits::pct);
      FrontLeft.spin(directionType::fwd, leftVel, velocityUnits::pct);
      BackLeft.spin(directionType::fwd, rightVel, velocityUnits::pct);
      BackRight.spin(directionType::fwd, leftVel, velocityUnits::pct);
    }
};
