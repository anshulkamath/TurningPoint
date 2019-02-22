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
string side = "BLUE";
int autonNum = 1; // 0 is close to flags, 1 is far from flags
bool park = true;

string toString1(double val)
{
    ostringstream v;
    v << val;
    return v.str();
}
void sleep(int time)
{
    task::sleep(time);
}

void setBrakeMode(vex::brakeType brake)
{
    FrontLeft.setStopping(brake);
    FrontRight.setStopping(brake);
    BackLeft.setStopping(brake);
    BackRight.setStopping(brake);
}

void autonPark()
{
    if (Brain.Screen.xPosition() >= 240) park = true;
    else if (Brain.Screen.xPosition() < 240) park = false;

    string autonSide = "Side: " + side;
    string autonNummy = "Auton Num: " + toString1(autonNum);
    string parkStr = "Park: " + toString1(park);
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(1, 20, autonSide.c_str());
    //Brain.Screen.newLine();
    Brain.Screen.printAt(1, 40, autonNummy.c_str());
    //Brain.Screen.newLine();
    Brain.Screen.printAt(1, 60, parkStr.c_str());
}

void autonSelect()
{
    if(Brain.Screen.xPosition() >= 241) autonNum = 0;
    else if(Brain.Screen.xPosition() <= 240) autonNum = 1;

    Brain.Screen.clearScreen();
    Brain.Screen.drawLine(240, 0, 240, 480);
    Brain.Screen.printAt(100, 136, "PARK");
    Brain.Screen.printAt(340, 136, "NO PARK");
    Brain.Screen.pressed(autonPark);
}

void sideSelect()
{
    if(Brain.Screen.xPosition() >= 241) side = "BLUE";
    else if(Brain.Screen.xPosition() <= 240) side = "RED";

    Brain.Screen.clearScreen();
    Brain.Screen.setPenColor(vex::color::red);
    Brain.Screen.drawRectangle(0, 0, 240, 272, vex::color::white);
    Brain.Screen.drawRectangle(241, 0, 480, 272, vex::color::black);
    Brain.Screen.printAt(100, 136, "FRONT");
    Brain.Screen.printAt(340, 136, "BACK");
    Brain.Screen.render();
    Brain.Screen.pressed(autonSelect);
}

void pre_auton( void )
{   
    // Brain Screen: 480 x 272
    /*while(1==1)
    {
        int xPos = Brain.Screen.xPosition();
        int yPos = Brain.Screen.yPosition();
        // remove any old text from the screen to prevent unexpected results
        Brain.Screen.clearScreen()
        // display the current touch position
        Brain.Screen.printAt(1, 20, "current pos x: %04d, y: %04d", xPos, yPos);
        // display if the screen is pressed of not
        if (Brain.Screen.pressing()) {
            Brain.Screen.printAt(1, 40, "Screen pressed");
        } else {
            Brain.Screen.printAt(1, 40, "Screen released");
        }
        //Sleep the task for a short amount of time to prevent wasted resources.
        task::sleep(100);
    }*/
    
    Brain.Screen.clearScreen();
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.drawRectangle(0, 0, 240, 272, vex::color::blue);
    Brain.Screen.drawRectangle(241, 0, 480, 272, vex::color::red);
    Brain.Screen.pressed(sideSelect);
}

void forward(double inches, double speed = 70)
{
    setBrakeMode(vex::brakeType::brake);
    double rampConst = (double)(200) / 360;
    double rots = inches/(wheelDiameter*PI);
    rampConst = 0;
    
    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);    
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, true);
}

void setDrive(double vel)
{
    BackLeft.spin(directionType::fwd, vel, velocityUnits::pct);
    BackRight.spin(directionType::fwd, vel, velocityUnits::pct);
    FrontRight.spin(directionType::fwd, vel, velocityUnits::pct);        
    FrontLeft.spin(directionType::fwd, vel, velocityUnits::pct);
    
    if (vel = 0)
    {
        BackRight.stop();
        BackLeft.stop();        
        FrontLeft.stop();
        FrontRight.stop();
    }
}

void setDrive(double rightVel, double leftVel)
{
    BackLeft.spin(directionType::fwd, leftVel, velocityUnits::pct);
    BackRight.spin(directionType::fwd, rightVel, velocityUnits::pct);
    FrontRight.spin(directionType::fwd, rightVel, velocityUnits::pct);        
    FrontLeft.spin(directionType::fwd, leftVel, velocityUnits::pct);
}

void forward(double inches, double speed, int time)
{
    setBrakeMode(vex::brakeType::brake);   
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
    setBrakeMode(vex::brakeType::brake);     
    double rots = inches/(wheelDiameter*PI);
    double rampConst = (double)(300) / 360;

    rampConst = 0;

    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);    
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, true);
}
void backward1(double inches, double speed = 50)
{
    setBrakeMode(vex::brakeType::brake);     
    double rots = inches/(wheelDiameter*PI);
    double rampConst = (double)(300) / 360;

    rampConst = 0;

    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);    
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, true);
}
void backward(double inches, int time, double speed = 50)
{
    setBrakeMode(vex::brakeType::brake);     
    double rots = inches/(wheelDiameter*PI);
    double rampConst = (double)(300) / 360;

    rampConst = 0;

    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);

    Brain.resetTimer();
    while(Brain.timer(timeUnits::msec) < time && FrontRight.isSpinning() && BackRight.isSpinning());
}

void drive(double inches, double speed = 60, int rampCycles = 7,  int timeSlice = 50)
{
    double rots = inches / (wheelDiameter * PI);
    double initRots= 0;
    setBrakeMode(vex::brakeType::brake);
        
    BackLeft.rotateFor(rots, rotationUnits:: rev, speed , velocityUnits::pct, false);
    BackRight.rotateFor(rots, rotationUnits:: rev, speed , velocityUnits::pct, false);
    FrontRight.rotateFor(rots, rotationUnits:: rev, speed , velocityUnits::pct, false);        
    FrontLeft.rotateFor(rots, rotationUnits:: rev, speed , velocityUnits::pct, true);

}

void turnLeft(double degrees)
{

    setBrakeMode(vex::brakeType::hold);    
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 90/86 * 92.5/90;  
    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, true);    
    setBrakeMode(vex::brakeType::coast);  
}

void turnRight(double degrees)
{
    setBrakeMode(vex::brakeType::hold);       
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 90/86 * 92.5/90;

    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, true); 
    
    setBrakeMode(vex::brakeType::coast);  
}

// Tasks

// Drive Variables
double turnLimiter = .6;
bool braked = false;
bool inverted = false;
bool isDriving = false;

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
            frontLeftValue = frontRightValue = backLeftValue = backRightValue = 60;
        else if (Controller1.ButtonR2.pressing())
            frontLeftValue = frontRightValue = backLeftValue = backRightValue = -60;       

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
        
        if (Controller1.ButtonRight.pressing())
        {
            backLeftValue = frontLeftValue = 30;
            frontRightValue = backRightValue = -30;
        }
        else if (Controller1.ButtonLeft.pressing())
        {
            backLeftValue = frontLeftValue = -30;
            frontRightValue = backRightValue = 30;
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
                setBrakeMode(brakeType::coast);
                braked = false;
            }

            while (Controller1.ButtonDown.pressing());
        }

        if (frontLeftValue == 0 && backLeftValue == 0 && frontRightValue == 0 && backRightValue == 0)
            isDriving = false;
        else
            isDriving = true;
        
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
bool manual = true;
bool inTakeInUse = false;
bool fire = false;
bool catapultDown = false;
double errorB = 0.1;
double errorX = 0.02;

// Shooter Task
int taskShooter()
{
    Shooter.setStopping(brakeType::coast);
    ShooterAux.setStopping(brakeType::coast);
    Shooter.resetRotation();
    double count = 0;
    while(true)
    {
        if (manual && (Controller1.ButtonX.pressing() || fire))
        {
            fire = false;
            inUse = true;
            // Rotates until slip
            Shooter.setStopping(brakeType::hold);
            ShooterAux.setStopping(brakeType::hold);
            //count += 3 + errorX;
            Shooter.rotateFor(3+errorX, rotationUnits::rev, 100, velocityUnits::pct, false);
            ShooterAux.rotateFor(3+errorX, rotationUnits::rev, 100, velocityUnits::pct, true);
            task::sleep(80);
            //Shooter.setStopping(brakeType::coast);
            //ShooterAux.setStopping(brakeType::coast);
            inUse = false;
        }
        else if(manual && Controller1.ButtonB.pressing())
        {
            Shooter.setStopping(brakeType::brake);
            ShooterAux.setStopping(brakeType::brake);
            Shooter.rotateFor(3 - 0.04, rotationUnits::rev, 100, velocityUnits::pct, false);
            ShooterAux.rotateFor(3 - 0.04, rotationUnits::rev, 100, velocityUnits::pct, true);
            task::sleep(10);
            Shooter.setStopping(brakeType::coast);
            ShooterAux.setStopping(brakeType::coast);            
        }
        else if (!manual && (Controller1.ButtonX.pressing() || fire))
        {
            fire = false;
            inUse = true;
            Shooter.spin(directionType::fwd, 100, velocityUnits::pct);
            ShooterAux.spin(directionType::fwd, 100, velocityUnits::pct);
            
            while (!sBumper.pressing()) // Initial slip
                sleep(50);
            while (sBumper.pressing())
                sleep(50);
            while (!sBumper.pressing()) // Reset to slip
                sleep(50);
            
            Shooter.stop();
            ShooterAux.stop();
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
        if (Controller1.ButtonL1.pressing() && !inUse)
            Intake.spin(directionType::fwd, 100, velocityUnits::pct);
        else if(Controller1.ButtonL2.pressing() && !inUse)
            Intake.spin(directionType::rev, 100, velocityUnits::pct);
        else
            Intake.stop(brakeType::hold);
        
        /*
        if (Controller1.ButtonRight.pressing())
            Descore.spin(directionType::fwd, 50, velocityUnits::pct);
        else if (Controller1.ButtonLeft.pressing())
            Descore.spin(directionType::rev, 50, velocityUnits::pct);
        else
            Descore.stop(brakeType::hold);*/
        sleep(50);
    }
    return 0;
}


void oldSkills()
{
    task shooterTask = task(taskShooter, 1);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);    
    drive(36, -40); // Back into the ball
    task::sleep(500);
    drive(4, 30);
    task::sleep(700);
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    drive(19, -30); // 1 point
    
    drive(43, 40);
    sleep(100);
    turnRight(5); 
    drive(10, 40);
    turnLeft(5);
    forward(15, 40, 2000);
    sleep(300);
    // Added
    /*backward(21, 30.0);
    turnRight(35);
    // Reverse into the cap to flip it
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    drive(32, -40);
    Intake.stop(brakeType::hold);  // 2 points
    drive(46, 40);
    turnLeft(35);
    forward(37.5, 35);*/
    // Added
    
    sleep(500);
    // turn
    drive(10, -40);
    turnRight(90);
    
    // Center on field
    drive(68, 40);
    task::sleep(500);
    turnLeft(90);
    task::sleep(100);
    forward(19.5, 40, 1000);
    task::sleep(100);
    drive(10, -40);
    task::sleep(100);
    turnRight(90);
    task::sleep(200);
    
    // Ready to fire
    fire = true; // 6 points
    task::sleep(1000);
    turnLeft(16.5);
    drive(14, 40);
    turnRight(16.5);
    //forward(40, 60); // Drive into bottom flags
    forward(15, 60, 1000);
    
    task::sleep(200); // 7 points
    backward(22, 40.0); 
   
    // Go for the middle caps
    turnLeft(90);
    sleep(400);
    forward(12, 40, 3000);
    sleep(400);
    Intake.spin(directionType::rev, 100, velocityUnits::pct);  
    drive(30, -40);
    sleep(500);
    Intake.stop(brakeType::hold);
    drive(25, 40);
    sleep(200);
    turnLeft(90);
    sleep(200);
    drive(21, 40);// 47
    
    // Added
    turnRight(90);
    forward(12, 40, 3000);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);    
    drive(48, -40); // Back into the ball
    task::sleep(500);
    drive(4, 30);
    task::sleep(700);
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    drive(19, -30); // 1 point
    sleep(100);
    drive(11, 30);
    sleep(100);
    turnRight(100);
    Intake.stop(brakeType::hold);
    //backward(10, 1000, 40);
    drive(26, 40);
    sleep(500);
    fire = true;
    sleep(500);
    drive(24, -30);
    //backward(10, 1000, 40);
    turnLeft(90);
    drive(33, 40);
    //drive(47, 40);
    sleep(100);
    forward(15, 40, 2000);
    sleep(300);
    backward(24, 40);

    sleep(500);
    turnLeft(90);
    drive(16, 40);
    // Added   
    turnLeft(90);
    sleep(200);
    backward(12, 1000, 40);
    sleep(200);
    forward(92.5, 50);
}

void autonomous( void ) {
    oldSkills();
}

void usercontrol() 
{
    vex::task shooter = vex::task(taskShooter, 1);
    vex::task(taskDrive, 1);
    vex::task(taskIntakes, 1);
    vex::task(taskScreen, 2);
    
    setBrakeMode(brakeType::coast); 
    Controller1.Screen.clearScreen();
    FrontRight.resetRotation();
    
    while(true)
    {   
        if(Controller1.ButtonA.pressing())
        {
            shooter.suspend(); // previously shooter.stop();
            Shooter.stop(brakeType::coast);
            ShooterAux.stop(brakeType::coast);
            while (Controller1.ButtonA.pressing())
                sleep(50);
            shooter.resume();  // previously vex::task shooter = vex::task(taskShooter, 1);
        }
        
        /*if (!inUse || isDriving)
            setBrakeMode(brakeType::coast);
        else if (inUse && !isDriving)
            setBrakeMode(brakeType::hold);  */

        sleep(100);
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
