#pragma once;

#include "../Headers/Drivetrain.h"
#include "../cleanConfig.h"
#include <vector>
#include <iostream>
using namespace std;

Drivetrain drive(FrontRight, FrontLeft, BackRight, BackLeft, gyroscope, invertedGyro);

bool intakeUse = false;
int turnLimiter = 1;

string side = "";
int autonNum = 0;
bool pressed = false;

int cataUp = 2525;
int cataDown = 1350;
bool cataReady = false;
bool fire = false;
double angle = 0;
double getAngle()
{
  return angle;
}
double scraperReady = -400;
double scraperHeight = -640;
