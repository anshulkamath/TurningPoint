#pragma once

class Auxiliary
{
  private:
    motor& intake;
    motor& puncher;
    motor& angler;
    motor& scraper;

    pot& poten;

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
    void setAngler(double angle);
    void setScraper(double val);

    // Driver functions
    void doubleShot(int angle1, int angle2);
};
