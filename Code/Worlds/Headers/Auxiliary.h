#pragma once
#include "robot-config.h"

class Auxiliary
{
  private:
    motor& intake;
    motor& puncher;
    motor& angler;
    motor& scraper;

    pot& poten;

  public:
    Auxiliary(motor int, motor punch, motor angle, motor scrape, pot potent) :
    intake(int), puncher(punch), angler(angle), scraper(scrape), poten(potent) {}

    // Tasks
    void intakeTask(int state);
    void puncherTask();
    void anglerTask();
    void scraperTask();·

    // Autonomous functions
    void runIntake(int state);
    void runPuncher();
    void setAngler(double angle);
    void setScraper(double val);

    // Driver functions
    void doubleShot(int angle1, int angle2);
};
