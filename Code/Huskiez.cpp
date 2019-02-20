#include "robot-config.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>

using namespace std;
using namespace vex;

// Global Variables
double PI = M_PI;
double wheelDiameter = 4;
double wheelBaseLength = 9.5;

// Auton Selector
string side = "BLUE";
int autonNum = 1; // 0 is close to flags, 1 is far from flags
bool park = true;
bool braked = false; // BRAKES: Hold for parking, Brake for shooting

// Helper fuctions
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
int sgn(double val)
{
    return val > 0 ? 1 : -1;
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

double getAngle()
{
    return (gyroscope.value(analogUnits::range12bit) - invertedGyro.value(analogUnits::range12bit))/20;
}

// Made this a function so it can be called by other tasks
void refresh()
{
    Controller1.Screen.print("Braked: %d", braked);
    Controller1.Screen.newLine();
    int temp = Brain.Battery.capacity(percentUnits::pct);
    Controller1.Screen.print("Battery: %d", temp);
}

// 1 is forward, 0 is stop, -1 is backward
void runIntake(int num)
{
    switch(num)
    {
        case -1:
            Intake.spin(directionType::rev, 100, velocityUnits::pct);
            break;
        case 0:
            Intake.stop(brakeType::coast);
            break;
        case 1:
            Intake.spin(directionType::fwd, 100, velocityUnits::pct);
            break;
        default:
            Intake.stop(brakeType::coast);
            break;
    }
}

// -1 is starting position, 0 is middle, 1 is scraping level

double scraperMid = 1150;
double scraperDown = 1425;
void runScraper(int num)
{
    switch(num)
    {
        case -1:
            Scraper.rotateTo(0, rotationUnits::deg, 100, velocityUnits::pct);
            break;
        case 0:
            Scraper.rotateTo(scraperMid, rotationUnits::deg, 100, velocityUnits::pct);
            break;
        case 1:
            Scraper.rotateTo(scraperDown, rotationUnits::deg, 100, velocityUnits::pct);
            break;
        default:
            break;
    }
}

int sign(double val)
{
    return val < 0 ? -1 : 1;
}

// Auton Selector Functions
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
    Brain.Screen.released(autonPark);
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
    Brain.Screen.released(autonSelect);
}

void pre_auton( void )
{
    gyroscope.startCalibration(2);
    invertedGyro.startCalibration(2);
    sleep(5000);

    Brain.Screen.clearScreen();
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.drawRectangle(0, 0, 240, 272, vex::color::blue);
    Brain.Screen.drawRectangle(241, 0, 480, 272, vex::color::red);
    Brain.Screen.released(sideSelect);
}

void forward(double inches, double speed = 70)
{
    setBrakeMode(brakeType::brake);
    double rots = inches/(wheelDiameter*PI);

    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, true);
}

void forward(double inches, double speed, int time)
{
    setBrakeMode(brakeType::brake);
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
    setBrakeMode(brakeType::brake);
    double rots = inches/(wheelDiameter*PI);

    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, true);
}

void backward1(double inches, double speed = 50)
{
    setBrakeMode(brakeType::brake);
    double rots = inches/(wheelDiameter * PI);

    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, true);
}

void backward(double inches, int time, double speed = 50)
{
    setBrakeMode(brakeType::brake);
    double rots = inches/(wheelDiameter*PI);

    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);

    Brain.resetTimer();
    while(Brain.timer(timeUnits::msec) < time && FrontRight.isSpinning() && BackRight.isSpinning());
}
double stblConst = .7;
double rampUp(double deltaV, int cycles, int timeSlice, double angle = 0)
{
    double currVel = FrontRight.velocity(velocityUnits::pct);
    FrontRight.resetRotation();

    // Gyro Stabilization variables
    double leftAdjustPwr = 0;
    double init = angle;

    for (int i = 0; i < cycles; i++)
    {
        currVel += deltaV/cycles;

        leftAdjustPwr = stblConst * (getAngle() - init); // Tracking difference in gyro value

        // Setting motor powers
        BackLeft.spin(directionType::fwd, currVel - leftAdjustPwr, velocityUnits::pct);
        BackRight.spin(directionType::fwd, currVel, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, currVel, velocityUnits::pct);
        FrontLeft.spin(directionType::fwd, currVel - leftAdjustPwr, velocityUnits::pct);

        task::sleep(timeSlice);
    }

    return FrontRight.rotation(rotationUnits::rev);
}

void turnLeft(double degrees)
{
    setBrakeMode(vex::brakeType::hold);
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 90/86 * 92.5/90;

    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, true);

    setBrakeMode(brakeType::coast);
}

void turnRight(double degrees)
{
    setBrakeMode(vex::brakeType::hold);
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 90/86 * 92.5/90;

    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 35, vex::velocityUnits::pct, true);

    setBrakeMode(brakeType::coast);
}
void turnTo(double degrees, double speed = 40)
{
    degrees *= 96.0/90.0;
    double P = 0, kp =.7, kd = .03, D = 0;
    double error = 100, motorPower = 0, lastError = 100;
    while(true)
    {
        lastError = error;
        error = degrees - getAngle();
        P = error * kp;
        D = kd *(error - lastError);
        motorPower = P + D;
        if(abs(error) <=0.1 && abs(lastError) <= .1) break;;

        if(abs(motorPower) > abs(speed))
            motorPower = speed * sgn(motorPower);

        BackLeft.spin(directionType::fwd, motorPower, velocityUnits::pct);
        BackRight.spin(directionType::fwd, -motorPower, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, -motorPower, velocityUnits::pct);
        FrontLeft.spin(directionType::fwd, motorPower, velocityUnits::pct);

        sleep(10);
    }
    BackLeft.stop(brakeType::brake);
    BackRight.stop(brakeType::brake);
    FrontRight.stop(brakeType::brake);
    FrontLeft.stop(brakeType::brake);
    Controller1.rumble("-.-.-");
    sleep(100); // Sleep here so we do not have to in the autonomous function
}

void drive(double inches, double speed, int cycles = 15, int timeSlice = 50, double heading = -121)
{
    // Converting inches to motor rotations
    double init = getAngle();
    if(heading != -121) init = heading;

    // Making sure a drive call backwards will have the same result
    if (inches < 0 || speed < 0)
    {
      inches = abs(inches) * -1;
      speed = abs(speed) * -1;
    }

    double rots = inches / (wheelDiameter * PI);
    rots -= rampUp(speed, cycles, timeSlice, init); // Subtracting ramping distance from total

    // PID Variables
    double P = 0, kp = 75;
    double I = 0, ki = .95;
    double D = 0, kd = 230;
    double error = 0, lError = 0;
    double iThresh = .5;
    double motorPower = 0, lMotorPower = 0;

    // Gyro Stabilization variables
    double leftAdjustPwr = 0;

    // Reset rotations before PID loop
    FrontRight.resetRotation();

    // Reset Brain Timer Before PID Loop
    Brain.resetTimer();
    int theoreticalTime = abs(rots / (speed * 2) / 60);

    // PID Loop
    while (abs(FrontRight.rotation(rotationUnits::rev)) < abs(rots)/* && Brain.timer(timeUnits::msec) < theoreticalTime + 1000*/)
    {
        // Setting PID Variables
        error = rots - (FrontRight.rotation(rotationUnits::rev));
        P = error * kp;
        D = (error - lError) * kd;

        if (abs(error) < iThresh)
            I += error * ki;
        else
            I = 0;

        motorPower = abs(P) + abs(I) - abs(D); // Manually setting motor power

        if (abs(motorPower) > abs(speed)) // Limiting motor power to top speed
            motorPower = abs(speed);

        if(speed < 0) // In case of going backwards
            motorPower *= -1;

        leftAdjustPwr = stblConst * (getAngle() - init); // Tracking difference in gyro value

        // Setting motor powers
        BackLeft.spin(directionType::fwd, motorPower - leftAdjustPwr, velocityUnits::pct);
        BackRight.spin(directionType::fwd, motorPower, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, motorPower, velocityUnits::pct);
        FrontLeft.spin(directionType::fwd, motorPower - leftAdjustPwr, velocityUnits::pct);

        task::sleep(50);
        lError = error;
    }

    // Stopping motors after loop ends
    BackLeft.stop(brakeType::brake);
    BackRight.stop(brakeType::brake);
    FrontRight.stop(brakeType::brake);
    FrontLeft.stop(brakeType::brake);
    turnTo(init);
    // Sleep here so we do not have to in the autonomous function
    if (abs(inches) >= 16)
        sleep(150);
    else
        sleep(50);
}


// Tasks

// Drive Variables
double turnLimiter = .6;
bool isDriving = false;

// Drive Task
int taskDrive()
{
    int fRightEnc = 0, fLeftEnc = 0, bRightEnc = 0, bLeftEnc = 0;
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

        /*
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
        */

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
                refresh();
            }
            else
            {
                setBrakeMode(brakeType::coast);
                braked = false;
                refresh();
            }

            while (Controller1.ButtonDown.pressing());
        }

        // Checks to see if the motors have moved to determine whether or
        // not the robot should be in coast mode
        if ((FrontRight.rotation(rotationUnits::rev) - fRightEnc) == 0 &&
            ((FrontLeft.rotation(rotationUnits::rev) - fLeftEnc) == 0) &&
            ((BackRight.rotation(rotationUnits::rev) - bRightEnc) == 0) &&
            ((BackLeft.rotation(rotationUnits::rev) - bLeftEnc) == 0))
            isDriving = false;
        else
            isDriving = true;

        // Sets Motor Powers
        FrontLeft.spin(directionType::fwd, frontLeftValue, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, frontRightValue, velocityUnits::pct);
        BackLeft.spin(directionType::fwd, backLeftValue, velocityUnits::pct);
        BackRight.spin(directionType::fwd, backRightValue, velocityUnits::pct);

        task::sleep(20);

        fRightEnc = FrontRight.rotation(rotationUnits::rev);
        fLeftEnc = FrontLeft.rotation(rotationUnits::rev);
        bRightEnc = BackRight.rotation(rotationUnits::rev);
        bLeftEnc = BackLeft.rotation(rotationUnits::rev);
    }
    return 0;
}

// Shooter Variables
bool inUse = false;
bool fire = false;
double errorB = 0.1;
double errorX = 0.03;

// Shooter Task
int taskShooter()
{
    Shooter.setStopping(brakeType::hold);
    ShooterAux.setStopping(brakeType::hold);
    Shooter.resetRotation();
    double count = 0;
    while(true)
    {
        if (Controller1.ButtonX.pressing() || fire)
        {
            fire = false;
            inUse = true;

            // Rotates until slip
            Shooter.rotateFor(3+errorX, rotationUnits::rev, 100, velocityUnits::pct, false);
            ShooterAux.rotateFor(3+errorX, rotationUnits::rev, 100, velocityUnits::pct, true);

            sleep(80);
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
        if (Controller1.ButtonL1.pressing() && !inUse)
            Intake.spin(directionType::fwd, 100, velocityUnits::pct);
        else if(Controller1.ButtonL2.pressing() && !inUse)
            Intake.spin(directionType::rev, 100, velocityUnits::pct);
        else
            Intake.stop(brakeType::hold);

        if (Controller1.ButtonUp.pressing())
            Scraper.rotateTo(scraperMid, rotationUnits::deg, 100, velocityUnits::pct, false);
        else if (Controller1.ButtonLeft.pressing())
            Scraper.spin(directionType::rev, 100, velocityUnits::pct);
        else if (Controller1.ButtonRight.pressing())
            Scraper.spin(directionType::fwd, 100, velocityUnits::pct);
        else
            Scraper.stop(brakeType::hold);

        task::sleep(50);
    }
    return 0;
}

int taskScreen()
{
    while (true)
    {
        refresh();
        sleep(5000);
    }
}
// Autonomous Programs

// Pre-condition  : assumes robot is in line with bottom flag and has two balls
// Post-condition : all three flags are toggled
void autonFire(double initDist)
{
  drive(initDist, 100); // Drive into the flag at 100
  side == "RED" ? turnTo(90) : turnTo(-90); // Re-center
  drive(-38, -100); // Drive back to shooting position
  side == "RED" ? turnTo(100) : turnTo(-100); // Turn to face flags
  fire = true; // Fire at flags
  sleep(400);
  side == "RED" ? turnTo(90) : turnTo(-90);
}

// Old 1 - Front - 3 Flags 1 Cap
void oldFunc1()
{
    // Setup for Auton
    task shooterTask = task(taskShooter, 1);
    FrontLeft.setStopping(brakeType::brake);
    FrontRight.setStopping(brakeType::brake);
    BackLeft.setStopping(brakeType::brake);
    BackRight.setStopping(brakeType::brake);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);

    backward1(36, 30.0); // Back into the ball

    task::sleep(600); // Wait for ball to get into intake

    forward(41, 40); // Drive forward
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);
    task::sleep(600);
    //backward(8,30); // Drive backwards to aim at flags

    if(side == "RED")
        turnRight(90);
    else
        turnLeft(90);

    forward(20, 30); // Shoot flags
    task::sleep(250);

    fire = true;
    task::sleep(500);
    if(side == "RED")
        turnLeft(10);
    else
        turnRight(10);

    forward(25, 40); // Drive into bottom flags
    task::sleep(200);
    backward1(30, 60.0);
}

// 1 - Shoot 3 flags, flip cap, flip bottom flag
void autonFunc1()
{
    task shooterTask = task(taskShooter, 1);

    runIntake(1);
    drive(-37.5, -100); // Drive backwards and get ball
    runIntake(0);
    drive(39, 100); // Drive to shooting position

    side == "RED" ? turnTo(90) : turnTo(-90); // Turn to face flags
    drive(20, 60); // Drive up to flag
    fire = true; // Fire
    sleep(400); // Wait for fire
    turnTo(0); // Turn to wall
    drive(3, 30); // Move out of alignment with pole
    side == "RED" ? turnTo(90) : turnTo(-90); // Turn back to flags
    drive(32, 100); // Hit the flags
    drive(-22, -100); // Come back in line with second cap
    turnTo(0); // Turn in line with second cap
    runIntake(-1); // Run intake backwards to flip cap
    drive(-24, -50); // Run backwards to flip the cap
    runIntake(0); // Stop the intake

    if (park)
    {
        drive(-9, -40); // Drive in line with the platform
        side == "RED" ? turnTo(-90) : turnTo(90); // Turn to face flags
        drive(36, 100); // Drive up to platform
        drive(30, 40); // Drive onto the platform
    }
    else
    {
        drive(-26, -75); // Drive backwards in line with bottom flag
        turnTo(90); // Turn to bottom flag
        drive(33, 100); // Drive forward to turn bottom flag
        drive(-33, -100); // Drive backwards to reset
    }
}

// 1 - Shoot 3 flags, flip cap, flip bottom flag
void autonFunc1Revamped()
{
    task shooterTask = task(taskShooter, 1);

    runIntake(1);
    drive(-37.5, -100); // Drive backwards and get ball
    runIntake(0);
    drive(40.5, 100); // Drive to shooting position

    side == "RED" ? turnTo(90) : turnTo(-90); // Turn to face flags
    drive(84, 100); // Drive into the flag at 100
    side == "RED" ? turnTo(90) : turnTo(-90); // Re-center
    drive(-63, -100); // Drive back to shooting position
    side == "RED" ? turnTo(100) : turnTo(-100); // Turn to face flags
    fire = true; // Fire at flags
    sleep(400);
    turnTo(0); // Turn to face the cap
    runIntake(-1); // Run intake backwards to flip cap
    drive(-30, -50); // Flip the near cap
    runIntake(0); // Stop intake after cap has been flipped

    if (park)
    {
        drive(-9, -40); // Drive in line with the platform
        side == "RED" ? turnTo(-90) : turnTo(90); // Turn to face flags
        drive(36, 100); // Drive up to platform
        drive(30, 40); // Drive onto the platform
    }
    else
    {
        drive(-26, -75); // Drive backwards in line with bottom flag
        turnTo(90); // Turn to bottom flag
        drive(33, 100); // Drive forward to turn bottom flag
        drive(-33, -100); // Drive backwards to reset
    }
}

// 2 - Shoot middle, flip back two caps, park
void autonFunc2()
{
    task shooterTask = task(taskShooter, 1);
    fire = true;
    sleep(400);
    //forward(2, 40);
    side == "BLUE" ? turnRight(120) : turnLeft(120);
    Controller1.rumble("-.-.-");
    forward(10, 40, 800);
    // Setup for Auton

    FrontLeft.setStopping(brakeType::brake);
    FrontRight.setStopping(brakeType::brake);
    BackLeft.setStopping(brakeType::brake);
    BackRight.setStopping(brakeType::brake);

    // Start intake and get ball under cap
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);
    backward1(42.0, 40.0); // Back into the ball
    task::sleep(600); // Wait for ball to get into intake
    Intake.stop();    // Stop the intake

    // Forward and turn to the next cap
    forward(12, 30);
    side == "RED" ? turnRight(50) : turnLeft(50);

    // Reverse into the cap to flip it
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    backward1(30, 35);
    Intake.stop(brakeType::hold);

    // Park
    forward(16, 40);
    if(park)
    {
        side == "RED" ? turnRight(40) : turnLeft(40);
        forward(43, 40); // Drive onto the platform
    }
}

void alphaAuton()
{
    task shooterTask = task(taskShooter, 1);

    runIntake(1);
    drive(-37.5, -100); // Drive backwards and get ball
    runIntake(0);
    drive(42, 100); // Drive to shooting position
    side == "RED" ? turnTo(90) : turnTo(-90); // Turn to face flags
    drive(-12, -80); // Run back to the line to flip out scraper

    // Bring scraper out
    Scraper.rotateTo(scraperDown, rotationUnits::deg, 100, velocityUnits::pct);

    // Wait until we are inside height
    while (Scraper.rotation(rotationUnits::deg) < scraperMid) { sleep(50); }

    autonFire(72); // Fire all flags

    drive(-16, -75); // Drive back to line up with balls on cap
    // Triangle: Legs = 22, 24, 32.5, Theta = 42.5
    turnTo(-42.5); // Turn to the theta value made by the triangle
    drive(-32.5, -100); // Drive up to the balls
    runScraper(1); // Run scraper to ball level
    runIntake(1); // Run intake to intake the balls
    drive(6, 40); // Scrape balls off of cap
    sleep(1500); // Wait for balls to be intaken
    runScraper(0); // Bring scraper back to middle

    runIntake(-1); // Run intake backwards to flip cap
    drive(-10, -60); // Flip cap

    // BE ADVISED :: The order of the shot is different than autonFire FOR TESTING PURPOSES
    // SHOOT BOTH FLAGS IN CASE THERE IS NOT ENOUGH TIME
    // CHANGE AFTER IF THERE IS AMPLE TIME

    turnTo(0); // Face the wall
    drive(-26, -100); // Drive to line up with flags
    side == "RED" ? turnTo(104) : turnTo(-104); // Turn to face flags
    fire = true; // Shoot next two balls

    side == "RED" ? turnTo(95) : turnTo(-95); // Turn to face flags
    drive(36, 100); // Flip the bottom flag

}

// EXPERIMENTAL - DO NOT RUN
void experimental()
{
    task(taskShooter, 1);
    runIntake(-1);
    drive(-42, -100);
    runIntake(0);
    drive(18, 75);
    side == "RED" ? turnTo(63) : turnTo(-63);
    runIntake(-1);
    drive(-27, -75); // Drive up to cap
    drive(-6, -30); // Flip cap
    drive(33, 100); // Line up with platform
    drive(12, 75); // Drive up to platform
    drive(43, 40); // Drive onto the platform
    drive(48, 100); // Drive to line up with Flags
    side == "RED" ? turnTo(135) : turnTo(-135);
    fire = true;
    drive(40, 75); // Flip bottom flag
}

void autonomous( void )
{
    if (autonNum == 0)
        autonFunc1();
    else if (autonNum == 1)
        autonFunc2();
}

void usercontrol()
{
    Controller1.Screen.clearScreen();
    vex::task shooter = vex::task(taskShooter, 1);
    vex::task(taskDrive, 1);
    vex::task(taskIntakes, 1);
    vex::task(taskScreen, 2);

    setBrakeMode(brakeType::coast);
    FrontRight.resetRotation();

    while(true)
    {
        if(Controller1.ButtonA.pressing())
        {
            shooter.stop();
            Shooter.stop(brakeType::coast);
            ShooterAux.stop(brakeType::coast);
            while (Controller1.ButtonA.pressing())
                sleep(50);
            vex::task shooter = vex::task(taskShooter, 1);
        }

        // If the shooter IS in use and the drive is NOT moving turn on braking
        if (inUse && !isDriving)
            setBrakeMode(brakeType::brake);
        else
            setBrakeMode(brakeType::coast);

        sleep(100);
    }
}

int main()
{
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
