#pragma once
#include <math.h>
// #include <iostream>
// #include <string>
// #include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Auxiliary
{
  private:
    motor intake;
    motor puncher;
    motor angler;
    motor scraper;

    pot poten;

    string toStr(int val);
    void writeFile(double error, double lError, double P, double I, double D, double anglePower);

  public:
    Auxiliary(motor intk, motor punch, motor angle, motor scrape, pot potent) :
    intake(intk), puncher(punch), angler(angle), scraper(scrape), poten(potent) {}

    // Tasks
    int intakeTask();
    int puncherTask();
    int scraperTask();

    // Autonomous functions
    void runIntake(int state);
    void firePuncher();
    void setAngle(int angle);
    void setScraper(double val);

    // Driver functions
    void doubleShot(int angle1, int angle2);
};
