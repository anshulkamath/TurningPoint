#include "robot-config.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>

using namespace std;
using namespace vex;

// Global Variables
double PI = 3.1415;
double wheelDiameter = 4;
double wheelBaseLength = 9.5;

// Auton Selector
string side = "RED";
int autonNum = 1; // 0 is close to flags, 1 is far from flags
bool park = true;

string toString1(double val)
{
    ostringstream v;
    v << val;
    return v.str();
}     

void autonPark()
{
    while(Brain.Screen.pressing());
    if (Brain.Screen.xPosition() <= 240) park = true;
    else if (Brain.Screen.xPosition() > 240) park = false;

    Brain.Screen.clearScreen();
    Brain.Screen.print("Side: %s, AutonNum: %d, Park: %d", side.c_str(), autonNum, park);
}

void autonSelect()
{
    while(Brain.Screen.pressing());
    if(Brain.Screen.xPosition() <= 240) autonNum = 0;
    else if(Brain.Screen.xPosition() >= 241) autonNum = 1;

    Brain.Screen.clearScreen();
    Brain.Screen.drawLine(240, 0, 240, 480);
    Brain.Screen.printAt(100, 136, "PARK");
    Brain.Screen.printAt(340, 136, "NO PARK");
    Brain.Screen.pressed(autonPark);
}

void sideSelect()
{
    while(Brain.Screen.pressing());
    if(Brain.Screen.xPosition() <= 240) side = "BLUE";
    else if(Brain.Screen.xPosition() >= 241) side = "RED";

    Brain.Screen.clearScreen();
    Brain.Screen.setPenColor(vex::color::red);
    Brain.Screen.drawRectangle(0, 0, 240, 272, vex::color::white);
    Brain.Screen.drawRectangle(241, 0, 480, 272, vex::color::black);
    Brain.Screen.printAt(100, 136, "FRONT");
    Brain.Screen.printAt(340, 136, "BACK");
    Brain.Screen.pressed(autonSelect);
}

void pre_auton( void )
{   
    // Brain Screen: 480 x 272
    Brain.Screen.clearScreen();
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.drawRectangle(0, 0, 240, 272, vex::color::blue);
    Brain.Screen.drawRectangle(241, 0, 480, 272, vex::color::red);
    Brain.Screen.pressed(sideSelect);
}

double rampCost(double s, double deltaV, int cycles, int timeSlice)
{
    return cycles*timeSlice *(deltaV/2 + s);
}

double rampUp(double deltaV, int cycles, int timeSlice)
{
    double currVel = FrontRight.velocity(velocityUnits::pct);
    FrontRight.resetRotation();

    for (int i = 0; i < cycles; i++)
    {
        currVel += deltaV/cycles;
        BackLeft.spin(directionType::fwd, currVel, velocityUnits::pct);
        BackRight.spin(directionType::fwd, currVel, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, currVel, velocityUnits::pct);        
        FrontLeft.spin(directionType::fwd, currVel, velocityUnits::pct);


        task::sleep(timeSlice);
    }

    return FrontRight.rotation(rotationUnits::rev);
}

void rampDown(double rots, double deltaVel, int cycles)
{
    double currVel = FrontRight.velocity(velocityUnits::pct);

    for(int i = 0; i < cycles; i++)
    {
        currVel -= (deltaVel/cycles);
        
        //if (currVel < 10) currVel = 0;

        BackLeft.spin(directionType::fwd, currVel , velocityUnits::pct);
        BackRight.spin(directionType::fwd, currVel, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, currVel, velocityUnits::pct);        
        FrontLeft.spin(directionType::fwd, currVel, velocityUnits::pct);

        
        FrontRight.resetRotation();
        while(abs(FrontRight.rotation(rotationUnits::rev)) < abs(rots/cycles)){
            task::sleep(100);
        }
    }

    BackLeft.stop(brakeType::brake);
    BackRight.stop(brakeType::brake);
    FrontRight.stop(brakeType::brake);    
    FrontLeft.stop(brakeType::brake);

}

void drive(double inches, double speed = 60, int rampCycles = 7,  int timeSlice = 50)
{
    double rots = inches / (wheelDiameter * PI);
    double initRots = rampUp(speed, rampCycles, timeSlice);
    double rampDownConst = rampCost(speed, -speed, rampCycles, timeSlice/1000);//initRots * 1.95;
    
    // Making an all-encompassing drive function that can move forwards and backwards
    if (speed < 0) 
    {
        rots *= -1;
        initRots *= -1;
        rampDownConst *= -1;
    }
    
    if (abs(speed) > 40)
    {   
        FrontRight.resetRotation();
        // Drive at speed until time to ramp down
        Controller1.Screen.clearScreen();
        while(abs(FrontRight.rotation(rotationUnits::rev)) < abs(rots - initRots - rampDownConst))
        {
            task::sleep(50);
        }
        
        double speed = FrontRight.velocity(velocityUnits::pct);
        /*BackLeft.stop(brakeType::brake);
        BackRight.stop(brakeType::brake);
        FrontLeft.stop(brakeType::brake);
        FrontRight.stop(brakeType::brake);*/
        
        
        // Ramp down
        //rampDown(rampDownConst, speed, 20);
        rampUp(-speed, rampCycles, timeSlice);
        BackLeft.stop(brakeType::brake);
        BackRight.stop(brakeType::brake);
        FrontRight.stop(brakeType::brake);        
        FrontLeft.stop(brakeType::brake);
 
    }
    else
    {
        FrontLeft.setStopping(brakeType::brake);
        FrontRight.setStopping(brakeType::brake);
        BackLeft.setStopping(brakeType::brake);
        BackRight.setStopping(brakeType::brake);   
        
        BackLeft.rotateFor(rots, rotationUnits:: rev, speed , velocityUnits::pct, false);
        BackRight.rotateFor(rots, rotationUnits:: rev, speed , velocityUnits::pct, false);
        FrontLeft.rotateFor(rots, rotationUnits:: rev, speed , velocityUnits::pct, false);
        FrontRight.rotateFor(rots, rotationUnits:: rev, speed , velocityUnits::pct, true);
    }

}

void forward(double inches, double speed = 70)
{
    FrontLeft.setStopping(brakeType::brake);
    FrontRight.setStopping(brakeType::brake);
    BackLeft.setStopping(brakeType::brake);
    BackRight.setStopping(brakeType::brake);        
    double rots = inches/(wheelDiameter*PI);
    double rampConst = (double)(200) / 360;

    rampConst = 0;
    
    FrontLeft.rotateFor(rots - 2.5*rampConst, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots - 2.5*rampConst, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots - 2.5*rampConst, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots - 2.5*rampConst, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, true);

}

void forward(double inches, double speed, int time)
{
    FrontLeft.setStopping(brakeType::brake);
    FrontRight.setStopping(brakeType::brake);
    BackLeft.setStopping(brakeType::brake);
    BackRight.setStopping(brakeType::brake);        
    double rots = inches/(wheelDiameter*PI);


    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
        
    Brain.resetTimer();
    while(Brain.timer(timeUnits::msec) < time && FrontRight.isSpinning() && BackRight.isSpinning());    
}

void backward(double inches, double speed = 50)
{

    FrontLeft.setStopping(brakeType::brake);
    FrontRight.setStopping(brakeType::brake);
    BackLeft.setStopping(brakeType::brake);
    BackRight.setStopping(brakeType::brake);      
    double rots = inches/(wheelDiameter*PI);
    double rampConst = (double)(300) / 360;

    rampConst = 0;

    FrontLeft.rotateFor(-rots + 5*rampConst, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots + 5*rampConst, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots + 5*rampConst, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots + 5*rampConst, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, true);

    
}

void backward(double inches, int time, double speed = 50)
{

    FrontLeft.setStopping(brakeType::brake);
    FrontRight.setStopping(brakeType::brake);
    BackLeft.setStopping(brakeType::brake);
    BackRight.setStopping(brakeType::brake);      
    double rots = inches/(wheelDiameter*PI);
    double rampConst = (double)(300) / 360;

    rampConst = 0;

    FrontLeft.rotateFor(-rots + 5*rampConst, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots + 5*rampConst, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots + 5*rampConst, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots + 5*rampConst, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);

    Brain.resetTimer();
    while(Brain.timer(timeUnits::msec) < time && FrontRight.isSpinning() && BackRight.isSpinning());
}

void turnLeft(double degrees)
{

    FrontLeft.setStopping(brakeType::hold);
    FrontRight.setStopping(brakeType::hold);
    BackLeft.setStopping(brakeType::hold);
    BackRight.setStopping(brakeType::hold);     
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 90/86 * 92.5/90;  
    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, true);    
    FrontLeft.setStopping(brakeType::coast);
    FrontRight.setStopping(brakeType::coast);
    BackLeft.setStopping(brakeType::coast);
    BackRight.setStopping(brakeType::coast); 
}

void turnRight(double degrees)
{
    
    FrontLeft.setStopping(brakeType::hold);
    FrontRight.setStopping(brakeType::hold);
    BackLeft.setStopping(brakeType::hold);
    BackRight.setStopping(brakeType::hold);       
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 90/86 * 92.5/90;

    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, true); 
    
    FrontLeft.setStopping(brakeType::coast);
    FrontRight.setStopping(brakeType::coast);
    BackLeft.setStopping(brakeType::coast);
    BackRight.setStopping(brakeType::coast);    
}

// Tasks

// Drive Variables
double turnLimiter = .6;
bool braked = false;
bool inverted = false;

int sign(double val)
{
    return val < 0 ? -1 : 1;
}

// Drive Task
int taskDrive()
{
    while (true)
    {
        int frontLeftValue = 0, 
        frontRightValue = 0, 
        backLeftValue = 0, 
        backRightValue = 0;        
        // Constant Straight Drive Control
        if (Controller1.ButtonR1.pressing())
            frontLeftValue = frontRightValue = backLeftValue = backRightValue = 50;
        else if (Controller1.ButtonR2.pressing())
            frontLeftValue = frontRightValue = backLeftValue = backRightValue = -50;       

        // Tank Drive Controls
        else
        {
            // Scale = x^2/100
            if (abs(Controller1.Axis2.value()) > 20)
            {
                frontRightValue = Controller1.Axis2.value();
                backRightValue = frontRightValue;
            }
            if (abs(Controller1.Axis3.value()) > 20)
            {
                frontLeftValue  = Controller1.Axis3.value();
                backLeftValue = frontLeftValue;
            }
        }
        
        if(abs(abs(Controller1.Axis2.value()) - abs(Controller1.Axis3.value())) > 20)
        {
            frontLeftValue = backLeftValue = Controller1.Axis3.value();
            frontRightValue = backRightValue = Controller1.Axis2.value();
        }
        // Throttling Turns
        if(abs(Controller1.Axis2.value() - Controller1.Axis3.value()) > 50)
        {
            frontLeftValue = backLeftValue *= turnLimiter; 
            frontRightValue = backRightValue *= turnLimiter; 
        }

        // Enables Brake Mode
        if (Controller1.ButtonDown.pressing())
        {
            if(!braked) 
            {
                FrontLeft.setStopping(brakeType::hold);
                FrontRight.setStopping(brakeType::hold);
                BackLeft.setStopping(brakeType::hold);
                BackRight.setStopping(brakeType::hold);
                
                braked = true;
            }
            else
            {
                FrontLeft.setStopping(brakeType::coast);
                FrontRight.setStopping(brakeType::coast);
                BackLeft.setStopping(brakeType::coast);
                BackRight.setStopping(brakeType::coast);
                
                braked = false;
            }

            while (Controller1.ButtonDown.pressing());
        }

        // Sets Motor Powers
        FrontLeft.spin(directionType::fwd, frontLeftValue, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, frontRightValue, velocityUnits::pct);
        BackLeft.spin(directionType::fwd, backLeftValue, velocityUnits::pct);
        BackRight.spin(directionType::fwd, backRightValue, velocityUnits::pct);

        task::sleep(20);
    }
    return 0;
}

// Shooter Variables
bool inUse = false;
bool inTakeInUse = false;
bool fire = false;
bool catapultDown = false;
double errorB = 0.1;
double errorX = 0.0185;

// Shooter Task
int taskShooter()
{
    while(true)
    {
        if (Controller1.ButtonX.pressing() || fire)
        {
            fire = false;
            inUse = true;
            // Rotates until slip
            Shooter.setStopping(brakeType::brake);
            ShooterAux.setStopping(brakeType::brake);
            Shooter.rotateFor(3 + errorX, rotationUnits::rev, 100, velocityUnits::pct, false);
            ShooterAux.rotateFor(3 + errorX, rotationUnits::rev, 100, velocityUnits::pct, true);
            task::sleep(10);
            Shooter.setStopping(brakeType::coast);
            ShooterAux.setStopping(brakeType::coast);
            inUse = false;
        }
        else if(Controller1.ButtonB.pressing())
        {
            Shooter.setStopping(brakeType::brake);
            ShooterAux.setStopping(brakeType::brake);
            Shooter.rotateFor(3 - 0.04, rotationUnits::rev, 100, velocityUnits::pct, false);
            ShooterAux.rotateFor(3 - 0.04, rotationUnits::rev, 100, velocityUnits::pct, true);
            task::sleep(10);
            Shooter.setStopping(brakeType::coast);
            ShooterAux.setStopping(brakeType::coast);            
        }
        else if (Controller1.ButtonY.pressing())
        {
            Shooter.spin(directionType::fwd, 60, velocityUnits::pct);
            ShooterAux.spin(directionType::fwd, 60, velocityUnits::pct);
            while(Controller1.ButtonY.pressing());
            Shooter.stop(brakeType::coast);
            ShooterAux.stop(brakeType::coast);
        }
        else
        {
            Shooter.stop(brakeType::coast);
            ShooterAux.stop(brakeType::coast);
        }

        task::sleep(50);
    }
    return 0;
}

int taskIntakes()
{
    while (true)
    {
        // Controls intake
        if (Controller1.ButtonL1.pressing())
            Intake.spin(directionType::fwd, 100, velocityUnits::pct);
        else if(Controller1.ButtonL2.pressing())
            Intake.spin(directionType::rev, 100, velocityUnits::pct);
        else if(!inTakeInUse)
            Intake.stop(brakeType::hold);
    }
    return 0;
}

void autonFunc1Ramp(string side)
{
    // Setup for Auton
    task shooterTask = task(taskShooter, 1);
    FrontLeft.setStopping(brakeType::hold);
    FrontRight.setStopping(brakeType::hold);
    BackLeft.setStopping(brakeType::hold);
    BackRight.setStopping(brakeType::hold);    
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);

    drive(24, -75); // Back into the ball

    task::sleep(600); // Wait for ball to get into intake

    drive(48, 75); // Drive forward
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);
    task::sleep(600);

    if(side == "RED")
        turnRight(95);
    else
        turnLeft(95);

    drive(20, 40); // Shoot flags
    task::sleep(250);

    fire = true;
    task::sleep(500);
    if(side == "RED")
        turnLeft(10);
    else
        turnRight(10);

    drive(30, 75); // Drive into bottom flags
    task::sleep(200);
    drive(65, -75);
    
    if (side == "RED")
        turnRight(100);
    else
        turnLeft(100);
    
    drive(55, 100);
}

void autonFunc1_GoRamp(string side)
{
    // Setup for Auton
    task shooterTask = task(taskShooter, 1);
    FrontLeft.setStopping(brakeType::hold);
    FrontRight.setStopping(brakeType::hold);
    BackLeft.setStopping(brakeType::hold);
    BackRight.setStopping(brakeType::hold);    
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);

    drive(24, -75); // Back into the ball

    task::sleep(600); // Wait for ball to get into intake

    drive(48, 75); // Drive forward
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);
    task::sleep(600);

    if(side == "RED")
        turnRight(95);
    else
        turnLeft(95);

    drive(20, 40); // Shoot flags
    task::sleep(250);

    fire = true;
    task::sleep(500);
    if(side == "RED")
        turnLeft(10);
    else
        turnRight(10);

    drive(30, 75); // Drive into bottom flags
    task::sleep(200);
    drive(30, -75);
    
    if (side == "RED")
        turnLeft(120);
    else
        turnRight(120);
    
    Intake.spin(directionType::fwd, -100, velocityUnits::pct);    
    drive(-10, 100);
    task::sleep(300);
    drive(-20, 60);
    
}

// Front - 3 Flags 1 Cap
void autonFunc1(string side)
{
    // Setup for Auton
    task shooterTask = task(taskShooter, 1);
    FrontLeft.setStopping(brakeType::brake);
    FrontRight.setStopping(brakeType::brake);
    BackLeft.setStopping(brakeType::brake);
    BackRight.setStopping(brakeType::brake);    
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);

    backward(36, 30); // Back into the ball

    task::sleep(600); // Wait for ball to get into intake

    forward(36, 40); // Drive forward
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);
    task::sleep(600);
    //backward(8,30); // Drive backwards to aim at flags

    if(side == "RED")
        turnRight(95);
    else
        turnLeft(95);

    forward(20, 30); // Shoot flags
    task::sleep(250);

    fire = true;
    task::sleep(500);
    if(side == "RED")
        turnLeft(10);
    else
        turnRight(10);

    forward(20, 40); // Drive into bottom flags
    task::sleep(200);
    backward(30, 60);
}



// Front - 2 Flags 2 Caps
void autonFunc2(string side)
{
    // Setup
    task shooterTask = task(taskShooter, 1);
    FrontLeft.setStopping(brakeType::brake);
    FrontRight.setStopping(brakeType::brake);
    BackLeft.setStopping(brakeType::brake);
    BackRight.setStopping(brakeType::brake);    

    // Set up catapult and back into the ball
    fire = true;
    backward(36, 40);

    // Intake the ball and flip the cap
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);
    task::sleep(300);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);
    task::sleep(600);    

    // Forward, center on wall, backwards
    forward(43.5, 80);
    task::sleep(600);
    backward(8);

    // Turn towards flags
    if(side == "RED")
        turnRight(100);
    else
        turnLeft(100);

    // Forward to get into position to shoot
    forward(20, 40);
    task::sleep(250);

    // Fire
    Shooter.rotateFor(200, rotationUnits::deg, 100, velocityUnits::pct);
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);

    // Turn towards the next cap
    if(side == "RED")
        turnRight(55.5);
    else
        turnLeft(55.5);

    // Drive into the cap
    forward(34, 50);
}

// Back - Flip Caps
void autonFunc3(string side)
{
    // Setup for Auton
    task shooterTask = task(taskShooter, 1);
    FrontLeft.setStopping(brakeType::brake);
    FrontRight.setStopping(brakeType::brake);
    BackLeft.setStopping(brakeType::brake);
    BackRight.setStopping(brakeType::brake);    

    // Start intake and get ball under cap
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);
    backward(36, 30); // Back into the ball
    task::sleep(600); // Wait for ball to get into intake
    Intake.stop();    // Stop the intake

    // Forward and turn to the next cap
    forward(6, 30);
    side == "RED" ? turnRight(70) : turnLeft(70);

    // Reverse into the cap to flip it
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    backward(25, 35);
    Intake.stop(brakeType::hold);

    // Park
    forward(19, 40);
    if(park)
    {
        side == "RED" ? turnRight(30) : turnLeft(30);
        forward(50, 60); // Drive onto the platform
    }
}

void autonSkillsRamp()
{
    task shooterTask = task(taskShooter, 1);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);    
    drive(28, -60); // Back into the ball
    task::sleep(500);
    drive(4, 30);
    task::sleep(700);
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    drive(19, -30); // 1 point
    
    drive(43, 60);
    task::sleep(100);
    turnRight(35);

    // Reverse into the cap to flip it
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    drive(33, -40);
    Intake.stop(brakeType::hold);  // 2 points
    drive(45, 40);
    turnLeft(35);
    task::sleep(500);
    //forward(37.5, 35);
    
    forward(40, 35, 1000);
    
    task::sleep(500);
    // turn
    drive(10, -40);
    turnRight(92);
    
    // Center on field
    drive(66, 75);
    task::sleep(500);
    turnLeft(92);
    task::sleep(100);
    forward(12, 40, 1000);
    task::sleep(100);
    drive(10, -40);
    task::sleep(100);
    turnRight(92);
    
    // Ready to fire
    fire = true; // 6 points
    task::sleep(600);
    turnLeft(10);
    drive(15, 40);
    turnRight(10);
    //forward(40, 60); // Drive into bottom flags
    forward(30, 60, 1000);
    
    task::sleep(200); // 7 points
    turnRight(10);
    task::sleep(200);
    drive(24, -60); 
    
    // Go for the middle caps
    turnLeft(90);
    task::sleep(400);
    forward(12, 40, 3000);
    task::sleep(400);
    Intake.spin(directionType::rev, 100, velocityUnits::pct);  
    drive(30, -40);
    task::sleep(500);
    Intake.stop(brakeType::hold);
    drive(25, 40);
    task::sleep(200);
    turnLeft(90);
    task::sleep(200);
    drive(50, 75);
    task::sleep(500);
    turnLeft(90);
    task::sleep(200);
    backward(12, 40, 3000);
    task::sleep(200);
    drive(100, 100);
    
    /*
    // Second Cap
    drive(4, 60);
    task::sleep(100);
    turnLeft(95);
    task::sleep(100);
    drive(28, 70);
    task::sleep(100);
    turnRight(90);
    task::sleep(100);
    forward(40, 60, 5000);
    */

      
   /* backward(36, 30); // Back into the ball
    task::sleep(600);    
    Intake.spin(directionType::fwd, -100, velocityUnits::pct);
    backward(10, 30); // 8 points
    forward(10, 30);
    // Prepare to shoot another flag
    turnRight(90);
    forward(25, 30);
    task::sleep(500);
    fire = true; // 10 points
    task::sleep(600);
    forward(25, 30); // Drive into bottom flags
    task::sleep(200); // 11 points
    backward(35, 30);  
    // Prepare to park
    turnLeft(90);
    forward(15, 30);
    turnLeft(90);
    forward(50, 70); 
    turnLeft(90); // lower platform
    forward(50, 70); // Upper platform 17 points
    */
}

void autonSkills()
{
    task shooterTask = task(taskShooter, 1);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);    
    backward(36, 30); // Back into the ball
    task::sleep(500);
    forward(4, 30);
    task::sleep(700);
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    backward(16, 30); // 1 point
    
    forward(19, 30);
    turnRight(70);

    // Reverse into the cap to flip it
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    backward(19, 30);
    Intake.stop(brakeType::hold);  // 2 points
    forward(22, 35);
    turnLeft(67);
    //forward(37.5, 35);
    
    forward(40, 35, 7000);
    
    task::sleep(500);
    // turn
    backward(12, 40);
    turnRight(93);
    
    
    forward(69, 35);
    task::sleep(150);
    // Ready to fire
    fire = true; // 6 points
    task::sleep(600);
    turnLeft(7);
    //forward(40, 60); // Drive into bottom flags
    forward(40, 60, 5000);
    
    task::sleep(200); // 7 points
    backward(35, 60); 
    /*
    // Go for the middle caps
    turnLeft(90);
    
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);    
    backward(36, 30); // Back into the ball
    task::sleep(600);    
    Intake.spin(directionType::fwd, -100, velocityUnits::pct);
    backward(10, 30); // 8 points
    forward(10, 30);
    // Prepare to shoot another flag
    turnRight(90);
    forward(25, 30);
    task::sleep(500);
    fire = true; // 10 points
    task::sleep(600);
    forward(25, 30); // Drive into bottom flags
    task::sleep(200); // 11 points
    backward(35, 30);  
    // Prepare to park
    turnLeft(90);
    forward(15, 30);
    turnLeft(90);
    forward(50, 70); 
    turnLeft(90); // lower platform
    forward(50, 70); // Upper platform 17 points
    */
}

void autonomous( void ) {
    autonSkillsRamp();
    //autonFunc1Ramp("RED");

}

void usercontrol() {

    vex::task(taskShooter, 1);
    vex::task(taskDrive, 1);
    vex::task(taskIntakes, 1);
    
    FrontLeft.setStopping(brakeType::coast);
    FrontRight.setStopping(brakeType::coast);
    BackLeft.setStopping(brakeType::coast);
    BackRight.setStopping(brakeType::coast);   
    Controller1.Screen.clearScreen();
    
    while(true)
    {
        if(Controller1.ButtonA.pressing())
        {
            Shooter.stop(brakeType::coast);
            ShooterAux.stop(brakeType::coast);  
            inUse = false;
        }

        Controller1.Screen.print("Braked: %d", braked);        
        Controller1.Screen.newLine();
        Controller1.Screen.print("Inverted: %d", inverted);        
        Controller1.Screen.newLine();
        int temp = Brain.Battery.capacity(percentUnits::pct);
        Controller1.Screen.print("Battery: %d", temp);        

        task::sleep(5000);
    }
}

int main() {

    //Run the pre-autonomous function. 
    pre_auton();

    //Set up callbacks for autonomous and driver control periods.
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );

    //Prevent main from exiting with an infinite loop.                        
    while(1) {
        vex::task::sleep(100);//Sleep the task for a short amount of time to prevent wasted resources.
    }    
    return 0;

}
