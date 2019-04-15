#pragma once;

#include "../Headers/Drivetrain.h"
#include "../cleanConfig.h"
#include <vector>
#include <iostream>
using namespace std;

Drivetrain drive(FrontRight, FrontLeft, BackRight, BackLeft, gyroscope, invertedGyro);

bool intakeUse = false;
int turnLimiter = 1;

string side = "BLUE";
int autonNum = 0;
int turnVal = 0;
bool pressed = false;

int cataUp = 2525;
int cataDown = 1350;
bool intakeActive = false;
bool fire = false;
bool autonFire = false;
double autonFireRotation = 0;
double angle = 0;
double getAngle()
{
  return side == "RED" ? angle : -angle;
}
double scraperReady = -400;
double scraperHeight = -840;
int FRVel = 0;
int FLVel = 0;
int BRVel = 0;
int BLVel = 0;
