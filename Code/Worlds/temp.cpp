#include "robot-config.h"
using namespace vex;
int midFlag = 2403;
int highFlag = 2656;
int initPot;

void setAngle(int potenVal)
{
    Angler.setStopping(brakeType::hold);
    double P = 0, kp = 0.21, D = 0, kd = .1;
    double error = 100, lError = 100;
    double anglePower = 0;

    while(fabs(error) > 10 || fabs(lError) > 10)
    {
        error = -potenVal + Poten.value(analogUnits::range12bit);
        P = error * kp;
        //D = error - lError;
        D *= kd;
        lError = error;
        Angler.spin(directionType::fwd, P + D, velocityUnits::pct);
        task::sleep(10);
    }

    Angler.stop();

}


void doubleShot(int shot1, int shot2)
{
    setAngle(shot1);
    int start = Puncher.rotation(rotationUnits::rev);
    Puncher.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct, false);
    //while(abs(Puncher.rotation(rotationUnits::rev)) - abs(start) < .24);
    task::sleep(500);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);
    //while(Puncher.isSpinning());
    setAngle(shot2);
    Puncher.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct);
}

int main()
{
    Angler.setStopping(brakeType::hold);
    while(true)
    {
        Brain.Screen.printAt(30, 30, "%d", Poten.value(analogUnits::range12bit));

        if (Controller.ButtonX.pressing())
            Puncher.spin(directionType::fwd, 100, velocityUnits::pct);
        else
            Puncher.stop();

        if (Controller.ButtonY.pressing())
            Angler.spin(directionType::fwd, 40, velocityUnits::pct);
        else if (Controller.ButtonA.pressing())
            Angler.spin(directionType::fwd, -40, velocityUnits::pct);
        else
            Angler.stop();
        if(Controller.ButtonUp.pressing()) {
            initPot = Poten.value(analogUnits::range12bit);
            doubleShot(highFlag, midFlag);
        }
    }

}
