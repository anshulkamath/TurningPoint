#pragma once
#include "vex.h"

vex::brain Brain;
vex::controller Controller;

vex::motor CataL = vex::motor(vex::PORT11,vex::gearSetting::ratio36_1,false);
vex::motor CataR = vex::motor(vex::PORT12, vex::gearSetting::ratio36_1, true);

vex::motor Intake = vex::motor(vex::PORT13, vex::gearSetting::ratio6_1, true);
vex::motor Scraper = vex::motor(vex::PORT15, vex::gearSetting::ratio18_1, true);

vex::motor BackLeft = vex::motor(vex::PORT14,vex::gearSetting::ratio36_1,true);
vex::motor FrontLeft = vex::motor(vex::PORT3,vex::gearSetting::ratio36_1,true);
vex::motor BackRight = vex::motor(vex::PORT4,vex::gearSetting::ratio36_1,false);
vex::motor FrontRight = vex::motor(vex::PORT5,vex::gearSetting::ratio36_1,false);

vex::pot CataPot = vex::pot(Brain.ThreeWirePort.A);
vex::gyro gyroscope = vex::gyro(Brain.ThreeWirePort.G);
vex::gyro gyroTest = vex::gyro(Brain.ThreeWirePort.B);
vex::gyro invertedGyro = vex::gyro(Brain.ThreeWirePort.H);
