#pragma once

#include "../src/vars.cpp"
#include "../include/cleanConfig.h"
#include "../headers/Drivetrain.h"
#include "../src/task.cpp"

using namespace vex;

int sideSelect()
{
    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(vex::color::red);
    Brain.Screen.drawRectangle(1,1,200,200);
    Brain.Screen.printAt(40, 40, false, "RED");
    Brain.Screen.setFillColor(vex::color::blue);
    Brain.Screen.drawRectangle(202,1,200,200);
    Brain.Screen.printAt(242, 40, false, "BLUE");
    while(!Brain.Screen.pressing());
    while(Brain.Screen.pressing());
    Brain.Screen.clearScreen();
    pressed = false;
    if(Brain.Screen.xPosition() < 200)
    {
        side = "RED";
    }else if(Brain.Screen.xPosition() > 200)
    {
        side = "BLUE";
    }
    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(vex::color::yellow);
    Brain.Screen.printAt(1,40,"Hello");
    Brain.Screen.drawRectangle(1,1,200,200);
    Brain.Screen.setFillColor(vex::color::white);
    Brain.Screen.drawRectangle(202,1,200,200);

    while(!Brain.Screen.pressing());
    while(Brain.Screen.pressing());
    Brain.Screen.clearScreen();
    if(Brain.Screen.xPosition() < 200)
    {
        autonNum = 1;
    }else if(Brain.Screen.xPosition() > 200)
    {
        autonNum = 2;
    }
    Brain.Screen.clearScreen();
    Brain.Screen.setPenColor(vex::color::red);
    Brain.Screen.setFillColor(vex::color::black);
    Brain.Screen.printAt(1,40, side.c_str());
    Brain.Screen.printAt(1, 80, "%d", autonNum);
    return 0;
}


int main()
{
    task taskCatapult(cataTask, 1);
    task taskIntake(intakeTask, 1);
    task taskDrive(driveTask, 1);
    task taskScraper(scraperTask, 1);

    while (true)
    {
        Brain.Screen.printAt(0, 30, "RightVel: %.2f", FrontRight.velocity(velocityUnits::rpm));

        task::sleep(20);
    }
}
