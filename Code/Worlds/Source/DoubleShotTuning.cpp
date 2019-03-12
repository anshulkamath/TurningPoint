#include "C:/Users/JakeFreeman/Desktop/TurningPoint/Code/Worlds/cleanConfigurations.h"
#include "C:/Users/JakeFreeman/Desktop/TurningPoint/Code/Worlds/Headers/Drivetrain.h"
#include "C:/Users/JakeFreeman/Desktop/TurningPoint/Code/Worlds/Headers/Auxiliary.h"
#include "C:/Users/JakeFreeman/Desktop/TurningPoint/Code/Worlds/Source/Drivetrain.cpp"
#include "C:/Users/JakeFreeman/Desktop/TurningPoint/Code/Worlds/Source/Auxiliary.cpp"
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

using namespace vex;

int midFlag = 48.2;
int highFlag = 0;
int initPot;


void doubleShot(int angle1, int angle2)
{
    // Go to the first shot angle
    setAngle(angle1);
    Puncher.rotateFor(2, rotationUnits::rev, 100, velocityUnits::pct, false); // shoot first shot
    while(puncherLine.value(analogUnits::range12bit) < 2400);
    task::sleep(10);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct); // intake the next ball

    setAngle(angle2); // Set to the second angle
    //Puncer.stop();
    Puncher.rotateFor(1, rotationUnits::rev, 50, velocityUnits::pct, true); // shoot second shot
    //while(puncherLine.value(analogUnits::range12bit) < 2400);
    Intake.stop(brakeType::hold);
}

int main()
{

    Angler.setStopping(brakeType::hold);
    fstream file("motorTorque.csv", fstream::app);
    int t = 0;
    while(true)
    {
        t++;
        file<<t<<","<<Puncher.torque(vex::torqueUnits::Nm)<<endl;
        Brain.Screen.printAt(30, 30, "%.2f", Angler.rotation(rotationUnits::deg));
        Brain.Screen.printAt(60, 60, "%d", puncherLine.value(analogUnits::range12bit));
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
            //initPot = Poten.value(analogUnits::range12bit);
            //setAngle(71.4);
            doubleShot(highFlag, midFlag);
        }
        if(Controller.ButtonRight.pressing())
        {
            //setAngle(120);
        }
    }

}
