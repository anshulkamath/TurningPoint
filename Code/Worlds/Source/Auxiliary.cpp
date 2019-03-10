#include "../Headers/Auxiliary.h"

string Auxiliary::toStr(int val)
{
    ostringstream v;
    v << val;
    return v.str();
}

/*void Auxiliary::writeFile(double error, double lError, double P, double I, double D, double anglePower)
{
  ofstream write;
  write.open("output.txt");
  write << "Error: ";
  write << toStr((int)error);
  write << ", lError: ";
  write << toStr((int)lError);
  write << ", P: ";
  write << toStr((int)P);
  write << ", I: ";
  write << toStr((int)I);
  write << ", D: ";
  write << toStr((int)D);
  write << ", anglePower: ";
  write << toStr((int)anglePower) << endl;
}*/


int Auxiliary::intakeTask()
{
    while (true)
    {
        // Controls intake
        if (Controller.ButtonL1.pressing())
            intake.spin(directionType::fwd, 100, velocityUnits::pct);
        else if(Controller.ButtonL2.pressing())
            intake.spin(directionType::rev, 100, velocityUnits::pct);
        else
            intake.stop(brakeType::hold);

        task::sleep(50);
    }
    return 0;
}

int Auxiliary::puncherTask()
{
  while(true)
  {
    if (Controller.ButtonX.pressing())
        puncher.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct);
    else if(Controller.ButtonB.pressing())
        doubleShot(2450, 2260); // Temporary, replace with variables later
    else
        puncher.stop(brakeType::coast);

    if (Controller.ButtonY.pressing())
      angler.spin(directionType::fwd, 50, velocityUnits::pct);
    else if (Controller.ButtonA.pressing())
      angler.spin(directionType::fwd, -50, velocityUnits::pct);
    else
      angler.stop(brakeType::hold);
  }
  return 1;
}

int sgn(int val)
{
  return val < 0 ? -1 : 1;
}

void Auxiliary::setAngle(int angle)
{
    angler.setStopping(brakeType::hold);
    double P = 0, kp = 0.1;
    double I = 0, ki = 0.0001;
    double D = 0, kd = 0.0;
    double error = 0, lError = 0;
    double anglePower = 30;
    while(fabs(error) > 10 || fabs(lError) > 10 || fabs(anglePower) > 3) //
    {
        error = -angle + Poten.value(analogUnits::range12bit);
        P = error * kp;
        //I += error * ki;
        D = (lError - error) * kd;

        anglePower = fabs(P) + fabs(I) - fabs(D);

        if (fabs(anglePower) > 60)
           anglePower = sgn(anglePower) * 45;

        angler.spin(directionType::fwd, sgn(error) * anglePower, velocityUnits::pct);

        task::sleep(40);
        lError = error;

        writeFile(error, lError, P, I, D, anglePower);
    }
    angler.stop();
}

void Auxiliary::runIntake(int state)
{
  switch(state)
  {
    case 1:
      intake.spin(directionType::fwd, 100, velocityUnits::pct);
      break;
    case 0:
      intake.stop(brakeType::hold);
      break;
    case -1:
      intake.spin(directionType::fwd, -100, velocityUnits::pct);
      break;
    default:
      intake.stop(brakeType::hold);
      break;
  }
}

void Auxiliary::doubleShot(int angle1, int angle2)
{
    // Go to the first shot angle
    setAngle(angle1);
    puncher.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct, true); // shoot first shot
    puncher.rotateFor(1, rotationUnits::rev, 50, velocityUnits::pct, false); // shoot second shot
    intake.spin(directionType::fwd, 100, velocityUnits::pct); // intake the next ball
    setAngle(angle2); // Set to the second angle
    intake.stop(brakeType::hold);
}
