#include "robot-config.h"
using namespace vex;
int midFlag = 2260;
int highFlag = 2450;
int initPot;

void setAngle(int potenVal)
{
    Angler.setStopping(brakeType::hold);
    double P = 0, kp = 0.1;
    double error = 100, lError = 100;
    double anglePower = 0;

    while(fabs(error) > 10 && fabs(lError) > 10)
    {
        error = -potenVal + poten.value(analogUnits::range12bit);
        P = error * kp;

        Angler.spin(directionType::fwd, P, velocityUnits::pct);
        task::sleep(50);
    }

    Angler.stop();

}


void doubleShot(int shot1, int shot2)
{
    setAngle(shot1);
    Puncher.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct);

    task::sleep(1000);

    setAngle(shot2);
    Puncher.rotateFor(1, rotationUnits::rev, 100, velocityUnits::pct);
}

int main()
{
    Angler.setStopping(brakeType::hold);
    while(true)
    {
        Brain.Screen.printAt(30, 30, "%d", poten.value(analogUnits::range12bit));

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
            initPot = poten.value(analogUnits::range12bit);
            doubleShot(highFlag, midFlag);
        }
    }

}
