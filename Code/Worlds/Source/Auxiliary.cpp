#include "../Headers/Auxiliary.h"
bool doubleShotOn = false;
const int puncherLineThres = 2000;
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

string run = "0";
void Auxiliary::setAngle(double angle)
{
  run += "0";
    //file.close();
    fstream file1(string("angle-")  + string(".csv"), fstream::app);
    Angler.setStopping(brakeType::hold);
    double P = 0, kp = .932;
    double I = 0, ki = 0.0;
    double D = 0, kd = 0.0;
    double error = 100, lError = 100;
    double anglePower = 0;
    int t = 0;
    file1<<",Target,Current,Error,P,D,I,Power"<<endl;
    while(fabs(error) > 1 || fabs(lError) > 1 || fabs(anglePower) > 3) //
    {
        error = angle - Angler.rotation(rotationUnits::deg);
        P = error * kp;
        //I += error * ki;
        D = (lError - error) * kd;
        if(fabs(anglePower) < 7)
          I += error * ki;
        anglePower = fabs(P) + fabs(I) - fabs(D);

        if (fabs(anglePower) > 100)
           anglePower = 100;

        Angler.spin(directionType::fwd, (sgn(error) * anglePower), velocityUnits::pct);
        t++;
        file1<<t<<","<<angle<<","<<Angler.rotation(rotationUnits::deg)<<","<<error<<","<<abs(P)<<","<<-abs(D)<<","<<abs(I)<<","<<sgn(error) * anglePower<<endl;
        task::sleep(10);
        lError = error;

    }
    Angler.stop();
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
    doubleShotOn = true;
    // Go to the first shot angle

    setAngle(angle2);
    double start = Puncher.rotation(rotationUnits::deg);
    //Puncher.rotateFor(60, rotationUnits::deg, 100, velocityUnits::pct, true); // shoot first shot
    Puncher.rotateFor(390*2-60, rotationUnits::deg, 100, velocityUnits::pct, false); // shoot first shot
    double prevToq = Puncher.torque(torqueUnits::Nm);
    while(Puncher.torque(torqueUnits::Nm) < 1.6);
    task::sleep(22);
    //while(Puncher.rotation(rotationUnits::deg) - start < 390);
    //while(puncherLine.value(analogUnits::range12bit) < puncherLineThres);
    //task::sleep(10);
    //Intake.spin(directionType::fwd, 100, velocityUnits::pct); // intake the next ball

    setAngle(angle1); // Set to the second angle

    //Puncer.stop();
    //Puncher.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct, true); // shoot second shot
    //while(puncherLine.value(analogUnits::range12bit) < 2400);
    //Intake.stop(brakeType::hold);*/
    while(Puncher.isSpinning());
    doubleShotOn = false;
}
