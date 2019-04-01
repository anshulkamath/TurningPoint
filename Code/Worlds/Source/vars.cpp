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
double scraperReadyHeight = -400;
double scraperScrapHeight = -640;
int FRVel = 0;
int FLVel = 0;
int BRVel = 0;
int BLVel = 0;
