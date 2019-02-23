#include "robot-config.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>

using namespace std;
using namespace vex;

int wheelDiameter = 4;
double PI = M_PI;
double wheelBaseLength = 9.5;
double gyroOff = 0;

void pre_auton( void )
{
    //task shooterTask = task(taskShooter, 1);
    gyroscope.startCalibration(2);
    invertedGyro.startCalibration(2);
    task::sleep(6000);
    FrontRight.resetRotation();
    Scraper.resetRotation();
    Controller1.rumble("-----");
}

double scraperMid = 1150;
double scraperDown = 1620;
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
// Helper functions
void sleep(int time)
{
    task::sleep(time);
}

int sgn(double val)
{
    return val > 0 ? 1 : -1;
}

void setBrakeMode(vex::brakeType brake)
{
    FrontLeft.setStopping(brake);
    FrontRight.setStopping(brake);
    BackLeft.setStopping(brake);
    BackRight.setStopping(brake);
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

// Pre-condition  : assumes robot is in line with bottom flag and has two balls
// Post-condition : all three flags are toggled
int scraperTarget = 0;
int taskScraper()
{
    while(true)
    {
        Scraper.rotateTo(scraperTarget, rotationUnits::deg, 100, velocityUnits::pct);
    }
    return 0;
}

double getAngle()
{
    return (gyroscope.value(analogUnits::range12bit) - invertedGyro.value(analogUnits::range12bit))/20 - gyroOff;
}

// Sets the offset of the gyro when centered against a wall
void setOffset(double targetValue)
{
    if(abs(targetValue - getAngle()) < 3) // in the case that the robot did not center and gyroDrift would be too great
        gyroOff += targetValue - getAngle();
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

// Turn Functions
void turnTo(double degrees, double speed = 40)
{
    degrees *= 96.0/90.0;
    double P = 0, kp =.7;
    double I = 0, ki = 0.001;
    double D = 0, kd = 0.03;
    double error = 100, lastError = 100;
    double motorPower = 0;
    int iThresh = 5;
    while(true)
    {
        lastError = error;
        error = degrees - getAngle();

        if (abs(error) > 180)
            error =  sgn(error) * (360 - abs(error));

        P = error * kp;
        D = kd * (error - lastError);

        if (abs(error) < iThresh)
            I += error * ki;

        if (abs(I) > 15)
          I = 15 * sgn(error);

        motorPower = P + I + D;

        if(abs(error) <= 0.3 && abs(lastError) <= .3) break; // Break statement

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
    sleep(50); // Sleep here so we do not have to in the autonomous function
}

void forward(double inches, double speed = 70)
{
    setBrakeMode(brakeType::brake);
    double rots = inches/(wheelDiameter*PI);

    FrontLeft.rotateFor(rots, rotationUnits::rev, speed, velocityUnits::pct, false);
    FrontRight.rotateFor(rots, rotationUnits::rev, speed, velocityUnits::pct, false);
    BackRight.rotateFor(rots, rotationUnits::rev, speed, velocityUnits::pct, false);
    BackLeft.rotateFor(rots, rotationUnits::rev, speed, velocityUnits::pct, true);
}


void backward(double inches, double speed = 50)
{
    setBrakeMode(brakeType::brake);
    double rots = inches/(wheelDiameter*PI);

    FrontLeft.rotateFor(-rots, rotationUnits::rev, speed, velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, rotationUnits::rev, speed, velocityUnits::pct, false);
    BackRight.rotateFor(-rots, rotationUnits::rev, speed, velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, rotationUnits::rev, speed, velocityUnits::pct, true);
}

double stblConst = .7;

// Drive functions
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

void drive(double inches, double speed, int cycles = 15, int timeSlice = 50, double heading = -121, bool turn = true)
{
    // Converting inches to motor rotations
    double init = getAngle();
    if(heading != -121) init = heading;
    double rots = inches / (wheelDiameter * PI);
    rots -= rampUp(speed, cycles, timeSlice, init); // Subtracting ramping distance from total

    // PID Variables
    double P = 0, kp = 75;
    double I = 0, ki = .95;
    double D = 0, kd = 230;
    double error = 0, lError = 0;
    double iThresh = .5;
    double motorPower = 0, lmotorPower = 0;

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

        //if(motorPower - (lmotorPower) > 15)
        // motorPower = abs(lmotorPower) + 15;

        if (abs(motorPower) > abs(speed)) // Limiting motor power to top speed
            motorPower = abs(speed);
        lmotorPower = motorPower;
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
    BackLeft.stop(brakeType::hold);
    BackRight.stop(brakeType::hold);
    FrontRight.stop(brakeType::hold);
    FrontLeft.stop(brakeType::hold);
    if(turn)
        turnTo(init);
    // Sleep here so we do not have to in the autonomous function
    if (abs(inches) >= 16)
        sleep(100);
    else
        sleep(50);
}

// Catapult Task

// Shooter Variables
bool fire = false;
double errorX = 0.005;

// Shooter Task
int taskShooter()
{
    Shooter.resetRotation();

    Shooter.setStopping(brakeType::hold);
    ShooterAux.setStopping(brakeType::hold);

    while(true)
    {
        if (fire)
        {
            // Stops the intake
            runIntake(0);
            fire = false; // Sets catapult toggle to false
            Shooter.setStopping(brakeType::hold);
            ShooterAux.setStopping(brakeType::hold);
            // Rotates until slip
            Shooter.rotateFor(3 - errorX, rotationUnits::rev, 100, velocityUnits::pct, false);
            ShooterAux.rotateFor(3 - errorX, rotationUnits::rev, 100, velocityUnits::pct, true);

            sleep(80);
        }


        sleep(50);
    }
    return 0;
}
string side = "RED";
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
// Auton for skills auton
// Precondition - Robot is in line with the cap
// Postcondition - Robot flips cap and retrieves ball
void flipCap(bool isRed, double capDist, bool intakeBall)
{
    isRed ? turnTo(0) : turnTo(180); // Turn to face the wall
    intakeBall ? runIntake(1) : runIntake(-1); // Turn intake depending on ball

    // Drive up to the cap
    if (capDist > 5)
        drive(capDist - 5, -100);

    // Either intake the ball or flip the corner cap
    intakeBall ? drive(-5, -40) : drive(-11, -40); // Flip forward corner cap (7 points) at lower power
    runIntake(0); // Stop the intake from running
    drive(3, 30); // Drive away from flipped cap

    if (intakeBall)
    {
        runIntake(-1); // Run intake in reverse to flip cap
        drive(-20, -40); // Flip cap (8 points)
        drive(16, 75); // Drive away from flipped cap
    }

}

// Pre-condition - Robot is in position to shoot
// Postcondition - Robot is back to original starting place (toggleable)
void skillShot(bool isRed,  bool retreat = true)
{
    fire = true; // Fire at middle column of flags (10 [or 12] points)
    sleep(1000);

    isRed ? turnTo(80) : turnTo(180); // Turn to 180º

    sleep(100);
    drive(7,60); // Move out of alignment with the pole

    sleep(100);
    turnTo(95); // Turn to the bottom flag

    sleep(100);
    drive(31, 75); // Flip bottom flag (11 points)
    turnTo(95);

    sleep(200);
    if (retreat)
    {
        drive(-26, -75); // Back up to line up with corner cap
        sleep (200); // Sleep here so we do not have to in the autuonmous function
    }
}

void newSkillShot(bool isFar)
{
    runIntake(1);
    isFar ? drive(78, 100, 20, 50, 96) : drive(36, 100, 20, 50, 96); // Drives into the wall
    //setOffset(90); // Accounts for any gyro drift
    drive(-32, -75, 20, 50, 96); // Drive to shooting position
    turnTo(98); // Angle towards the flags
    fire = true; // Fire the catapult
    sleep(1000); // Wait for catapult
    turnTo(90); // Turn back to original angle
}

void shoot(double initDist = 36)
{
    drive(initDist, 90, 15, 50, 0); // Hit bottom flag
    drive(-46, -80, 15, 50, 0); // Drive back to shooting position
    turnTo(5, 40); // Turn to face flags
    sleep(50);
    fire = true; // Shoot flags (5 points)
    sleep(500);
    turnTo(0);
}

// Miscellaneous Auton
void miscAuton(){
    // ADDED CODE FOR FLIPPING SECOND BACK CAP - IGNORE
    /*backward(21, 30.0);
    turnRight(35);
    // Reverse into the cap to flip it
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    drive(32, -40);
    Intake.stop(brakeType::hold);  // 2 points
    drive(46, 40);
    turnLeft(35);
    forward(37.5, 35);*/

    // CENTERING ON FIELD
    /*
    turnLeft(90);
    task::sleep(100);
    forward(19.5, 40, 1000);
    task::sleep(100);
    drive(10, -40);
    task::sleep(100);
    turnRight(90);
    task::sleep(200);
    */
}

// Skills Code Layout:
/*
    22 POINT AUTON
    1 - Grab ball from back cap and flip the cap
      - (MAYBE) Flip second back cap
    2 - Drive to the near flags
      - Flip all three flags
    3 - Back up and flip near cap
    4 - Turn left, move forward a tile, turn right
      - Collect next ball and flip cap
    5 - Move forward half a tile
      - Shoot a flag
      - Drive into bottom flag
    6 - Back up and turn left
      - Back into next cap
    7 - Turn right and back up one tile
      - Back up a tile and a half to collect ball
      - Flip cap
    8 - Drive forward 2 tiles and turn left
      - Move forward half a tile and shoot
      - Drive into bottom flag
    9 - Back up 3 tile
      - Turn left and square against the wall
      - Drive onto platform
*/

void skills16pt()
{
    // Start facing
    task shooterTask = task(taskShooter, 1);
    task scraperTask = task(taskScraper, 1);
    //turnTo(25);
    scraperTarget = -250;
    //backward(6, 50);
    backward(16.5, -40); // 15.5
    scraperTarget = -40;
    //backwards1()
    runIntake(1);
    //turnTo(-90, 90);
    //backward(9.5, 50);
    // drive(-9, -100, 5, 50, 0, false);

    sleep(500);
    //while(Scraper.isSpinning());
    //runScraper(1);
    forward(15, 40); // 15
    turnTo(1, 40);

    //  sleep(100); // Wait for ball to go into intake
    // drive(15, 80);
    //turnTo(0, 100);
    //runScraper(0);
    //drive(3, 40);
    runIntake(1);
    //turnTo(0);
    //forward(6, 40);
    drive(29, 70, 15, 50, 1); //20
    //turnTo(0, 60);
    sleep(50);
    fire = true;
    sleep(500);
    //turnTo(0);

    //autonFire(64);

    runIntake(1);
    backward(22, 40);
    // drive(-24.5, -80, 0, 0, 0, false);
    scraperTarget = -140;
    turnTo(-96);
    runIntake(1);
    drive(-46, -80,15, 50, -96, false);
    //forward(5, 40);
    scraperTarget = -260;
    turnTo(-215);
    backward(17, -60);
    //drive(-15, -80);
    scraperTarget = -40;
    sleep(1500);
    scraperTarget = -350;
    sleep(600);
    runIntake(-1);
    backward(13, 40);
    forward(15, 40);
    /*turnTo(-90);
    drive(26, 80);
    turnTo(0);*/

    turnTo(27);
    forward(8, 20);
    //runScraper(1);
    sleep(500);
    fire = true;
    scraperTarget = -40;
    sleep(200);
    turnTo(40);
    drive(-60, -100);
    turnTo(90);
    //backward(10,);
    forward(75, 40);
    //autonFire(64);

    /*turnTo(0);
    drive(-96, -100);
    runScraper(1);
    drive(10, 40);
    turnTo(90);
    drive(-30, -100);
    turnTo(0);
    runScraper(1);
    drive(10, 40);
    runScraper(0);
    drive(14, 40);
    turnTo(0);
    drive(24, 100);
    turnTo(90);
    autonFire(64);
    turnTo(0);
    //runIntake(-1);
    drive(-24, -100);
    turnTo(-90);
    drive(60, 40);
    turnTo(0);
    drive(60, 40);*/

}

void skills18pt()
{
    // Start facing
    task shooterTask = task(taskShooter, 1);
    task scraperTask = task(taskScraper, 1);

    scraperTarget = -250; // Bring Scraper up to capture ball
    backward(16.5, -40); // 15.5
    runIntake(1); // Intake the ball
    scraperTarget = -40; // Bring scraper down to catch ball

    sleep(500);
    forward(15, 40); // 15
    turnTo(0, 40);

    shoot(56); // Shoot all flags
    backward(12, 40); // Move back in line with cap
    sleep(300);
    turnTo(-90); // Turn to face caps
    runIntake(1); // Intake the ball

    scraperTarget = -140; // Set the scraper
    drive(-46, -80 ,15 , 50, -90, false); // Drive back to intake the ball
    scraperTarget = -260; // Move the scraper up

    turnTo(-215); // Turn to be in line with the next cap
    backward(17, -50); // Back into the next cap
    scraperTarget = -40; // Move intake to capture the next cap
    sleep(1250);
    scraperTarget = -350; // Move intake down to take the ball
    sleep(600);
    runIntake(-1); // Run intake to flip cap
    backward(13, 40); // Flip cap
    forward(13, 40); // Back to middle of tile

    turnTo(90); // Face the wall
    drive(28, 80, 15, 50, 90); // Drive in line with second bottom cap
    turnTo(0); // Face bottom flag
    drive(36, 80, 15, 50, 0); // Flip bottom flag
    turnTo(0); // Re-center
    drive(-36, -80, 15, 50, 0); // Drive back to position
    turnTo(5, 40); // Face the flags
    fire = true; // Shoot the flags

    sleep(500);
    turnTo(45.7); // Leg: 48, 54
    scraperTarget = -40;
    sleep(200);
    drive(-71, -100); // Legs: 48, 54
    turnTo(90);
    backward(12, 20);
    forward(75, 40);
}

void skills21pt()
{
    // Start facing
    task shooterTask = task(taskShooter, 1);
    task scraperTask = task(taskScraper, 1);

    scraperTarget = -250; // Bring Scraper up to capture ball
    backward(16.5, -40); // 15.5
    runIntake(1); // Intake the ball
    scraperTarget = -40; // Bring scraper down to catch ball

    sleep(500);
    forward(15, 40); // 15
    turnTo(0, 40);

    shoot(56); // Shoot all flags
    backward(12, 40); // Move back in line with cap
    sleep(300);
    turnTo(-90); // Turn to face caps
    runIntake(1); // Intake the ball

    scraperTarget = -140; // Set the scraper
    drive(-46, -80 ,15 , 50, -90, false); // Drive back to intake the ball
    scraperTarget = -260; // Move the scraper up

    turnTo(-215); // Turn to be in line with the next cap
    backward(17, -50); // Back into the next cap
    scraperTarget = -40; // Move intake to capture the next cap
    sleep(1250);
    scraperTarget = -350; // Move intake down to take the ball
    sleep(600);
    runIntake(-1); // Run intake to flip cap
    backward(13, 40); // Flip cap
    forward(13, 40); // Back to middle of tile

    turnTo(90); // Face the wall
    drive(28, 80, 15, 50, 90); // Drive in line with second bottom cap
    turnTo(0); // Face bottom flag
    drive(36, 80, 15, 50, 0); // Flip bottom flag
    turnTo(0); // Re-center
    drive(-36, -80, 15, 50, 0); // Drive back to position
    turnTo(5, 40); // Face the flags
    fire = true; // Shoot the flags
    sleep(500);

    turnTo(-90); // Face away from cap
    runIntake(-1); // Run intake out
    drive(-52, -100, 15, 50, -90); // Flip cap
    turnTo(0); // Turn to bottom flag
    drive(36, 80, 15, 50, 0); // Flip the bottom flag
    drive(-60, -100, 15, 50, 0); // Drive back in line with second cap
    turnTo(-90, -60); // Turn to second cap

    runIntake(-1); // Run flip the cap
    scraperTarget = -140; // Set the scraper
    drive(-35, -80 ,15, 50, 90, false); // Drive back to flip the cap
    scraperTarget = -260; // Move the scraper up

    drive(74, 100, 15, 50, 90); // Drive to wall
    turnTo(180, 60); // Turn to bottom of field
    drive(36, 100, 15, 50, 180); // Drive in line with platform
    turnTo(270, 60); // Turn to face platform
    backward(3, 30); // Back into wall
    forwrd(70, 40); // Drive onto platform
}

void skills25pt()
{
    // Start facing
    task shooterTask = task(taskShooter, 1);
    task scraperTask = task(taskScraper, 1);

    scraperTarget = -250; // Bring Scraper up to capture ball
    backward(16.5, -40); // 15.5
    runIntake(1); // Intake the ball
    scraperTarget = -40; // Bring scraper down to catch ball

    sleep(500);
    forward(15, 40); // 15
    turnTo(0, 40);

    shoot(56); // Shoot all flags
    backward(12, 40); // Move back in line with cap
    sleep(300);
    turnTo(-90); // Turn to face caps
    runIntake(1); // Intake the ball

    scraperTarget = -140; // Set the scraper
    drive(-46, -80 ,15 , 50, -90, false); // Drive back to intake the ball
    scraperTarget = -260; // Move the scraper up

    turnTo(-215); // Turn to be in line with the next cap
    backward(17, -50); // Back into the next cap
    scraperTarget = -40; // Move intake to capture the next cap
    sleep(1250);
    scraperTarget = -350; // Move intake down to take the ball
    sleep(600);
    runIntake(-1); // Run intake to flip cap
    backward(13, 40); // Flip cap
    forward(13, 40); // Back to middle of tile

    turnTo(90); // Face the wall
    drive(28, 80, 15, 50, 90); // Drive in line with second bottom cap
    turnTo(0); // Face bottom flag
    drive(36, 80, 15, 50, 0); // Flip bottom flag
    turnTo(0); // Re-center
    drive(-36, -80, 15, 50, 0); // Drive back to position
    turnTo(5, 40); // Face the flags
    fire = true; // Shoot the flags
    sleep(500);

    turnTo(-90); // Face away from cap
    runIntake(-1); // Run intake out
    drive(-52, -100, 15, 50, -90); // Flip cap
    turnTo(0); // Turn to bottom flag
    drive(36, 80, 15, 50, 0); // Flip the bottom flag
    drive(-60, -100, 15, 50, 0); // Drive back in line with second cap
    turnTo(-90, -60); // Turn to second cap

    runIntake(1); // Run intake to intake ball
    scraperTarget = -140; // Set the scraper
    drive(-35, -80 ,15 , 50, 90, false); // Drive back to intake the ball
    scraperTarget = -260; // Move the scraper up

    drive(29, 80, 15, 50, 90); // Run back to be in line with second ball
    turnTo(0, 60); // Turn to be in line with platform ball
    backward(12, -40); // Back into platform ball
    runIntake(1); // Intake the ball
    scraperTarget = -40; // Bring scraper down to catch ball

    sleep(500);
    forward(15, 40); // 15
    turnTo(0, 40); // 0 off of the platform
    forward(8, 50); // Intake the ball while scraping
    turnTo(90, 60); // Turn in line with wall
    forward(8, 50); // Move to be in line with flags
    turnTo(0, 60); // Turn to flags
    shoot(57); // Shoot
    drive(-48, -100, 15, 50, 0);
    turnTo(-90, -60); // Turn to the left facing the platform
    forward(45, 40); // Drive onto platform
}

void autonomous( void )
{
    // Turn on intake
    task shooterTask = task(taskShooter, 1);
    skills16pt();
}

void usercontrol( void )
{
    autonomous();
}

int main()
{
    //Run the pre-autonomous function.
    pre_auton();
    //autonomous();
    //Set up callbacks for autonomous and driver control periods.
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );

    //Prevent main from exiting with an infinite loop.
    while(true)
        task::sleep(100);//Sleep the task for a short amount of time to prevent wasted resources.
}
