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
    gyroscope.startCalibration();
    invertedGyro.startCalibration();
    FrontRight.resetRotation();
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
void turnTo(double degrees, double speed = 80)
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
    //Controller1.rumble("-.-.-");
    sleep(50); // Sleep here so we do not have to in the autonomous function
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

void drive(double inches, double speed, int cycles = 15, int timeSlice = 50, double heading = -121)
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
    double count = 0;

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
    drive(-36, -75, 20, 50, 96); // Drive to shooting position
    turnTo(98); // Angle towards the flags
    fire = true; // Fire the catapult
    sleep(1000); // Wait for catapult
    turnTo(90); // Turn back to original angle
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

void skills()
{
    // PART 1 - 1 POINT
    runIntake(1); // Run intake fwd
    drive(-36, -80); // Collect the ball
    turnTo(0);
    runIntake(0);
    //drive(2, 30);  // Forward to get away from cap
    //turnTo(0);
    sleep(700);
    runIntake(-1); // Run intake backwards to flip cap
    drive(-18, -30); // Flip Cap (1 point)
    runIntake(0); // Stop running the intake
    turnTo(0);
    drive(64, 80); // Drive back to starting position
    //turnRight(90);
    turnTo(90); // Turn To 90º
    drive(-20, -40);
    scraper.rotateTo(1150, rotationUnits::deg, 100, velocityUnits::pct, true);
    // PART 2 - 6 POINTS
    /*drive(69, 60, 15, 50, 90); // Drive to shooting position
    turnTo(0);
    //drive(10, );
    // drive(10, -50);
    turnTo(90);
    skillShot(true);*/ // Fire at flags (6 points)
    newSkillShot(true);
    
    //NEW
    turnTo(90);
    drive(-24, -50);
    turnTo(-45);
    drive(-24, -50);

    scraper.rotateTo(1425, rotationUnits::deg, 500, velocityUnits::pct);
    runIntake(1);
    drive(7, 30);
    scraper.rotateTo(1150, rotationUnits::deg, 50, velocityUnits::pct, true);
    sleep(800);
    runIntake(1);
    drive(-5, -60);
    runIntake(-1);
    drive(-20, -60);
    
    
    turnTo(0);
    drive(-28, -60);
    turnTo(90);
    newSkillShot(false);
    
    turnTo(0);
    drive(-48, -100);
    
    turnTo(90);    
    drive(-24, -50);
    turnTo(-45);
    drive(-24, -50);
    
    scraper.rotateTo(1425, rotationUnits::deg, 500, velocityUnits::pct);
    runIntake(1);
    drive(7, 30);
    scraper.rotateTo(1150, rotationUnits::deg, 50, velocityUnits::pct, true);
    sleep(800);
    runIntake(1);
    drive(-5, -60);
    runIntake(-1);
    drive(-20, -60);   
    
    turnTo(0);
    drive(5, 40);
    turnTo(90);
    drive(-10, -40);
    newSkillShoot(false);
    turnTo(0);
    drive(-48, -100);
    turnTo(-90);
    drive(24, 70);
    turnTo(0);
    drive(80, 50);
   // scraper.rotateFor()
    // PART 3 - 7 POINTS
    /*turnTo(0); // Added: turn to the wall
    runIntake(-1); // Run intake in reverse to flip cap
    drive(-24, -60); // Flip forward corner cap (7 points)
    runIntake(0); // Stop the intake from running
    drive(10, 60); // Drive away from flipped cap

    // PART 4 - 8 POINTS
    turnTo(-90); // Turn to get next ball
    drive(32, 60); // Drive forward in line with the next ball
    turnTo(0); // Turn to face ball
    runIntake(1); // Run intake to take in ball
    drive(-28, -80);
    runIntake(1);
    drive(-6, -20);

    //drive(5, 30);
    //turnTo(45);
    //drive(-3, -30);
    //turnTo(-30);
    //runIntake(-1);

    //drive(-8, -30);

    // PART 5 - 11 POINTS
    //turnTo(90); // Turn to flags
    runIntake(0); // Turn off intake
    sleep(200); // Added Let balls settle
    turnTo(90);
    drive(29, 100);
    //turnTo(90);
    //drive(36, 60);
    //turnTo(90);
    //drive(-36, -50);

    //skillShot(true); // Fire at flags (11 points)
    turnTo(0);
    // Turn to 0º
    // PART 6 - 12 POINTS
    drive(-36, -80); // Drive up to the cap
    runIntake(-1); // Run intake to flip the cap
    drive(-22, -60); // Flip the cap (12 points)
    runIntake(0); // Added : Stop intake
    // PART 7 - 13 POINTS
    //drive(1, 40); // Changed (not all the way) Back in line
    turnTo(90); // Turn to 90º
    drive(-24, -80); // Back up to be in line with next cap
    turnTo(169); // Turn to 180º to be in line with second cap
    runIntake(1);
    drive(-16, -40); // Collect next ball
    runIntake(0); // Stop intake
    drive(3, 30); // Drive away from  cap
    runIntake(-1); // Run intake in reverse to flip cap
    turnTo(169);
    drive(-12, -60); // Flip cap (13 points) -->  Changed to be less
    /*
    // PART 8 - 16 POINTS
    drive(56, 75); // Drive away from flipped cap
    turnTo(-90); // Turn to flags backwards to pick up balls
    runIntake(1); // Start intake in case of any balls
    drive(-14, -75); // Line up with flags backwards
    turnTo(90); // Turn to flags
    runIntake(0); // Turn off intake
    skillShot(false, false); // Fire at flags (16 points)
    // PART 9 - 22 POINTS
    drive(-72, -100); // Come back lined up with platform
    turnTo(0); // Turn to platform
    drive(92.5, 40); // Drive onto platform (22 points)
    */
}

void autonomous( void )
{
   // drive(50, 80);
    //turnTo(180);
    //drive(-50, -80);
    // Turn on intake
    task shooterTask = task(taskShooter, 1);
    gyroscope.startCalibration(2);
    invertedGyro.startCalibration(2);
    sleep(6000);
    //drive(48, 60);
    //turnTo(180);

    //drive(48, 60);
    //turnTo(90);
    //turnTo(90);
    //turnTo(0);
    /*//fire = true;
    //turn;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;To(90);
    //turnTo(0);
    drive(-36, -100);
    runIntake(1);
    drive(-6, -20);
    sleep(250);
    drive(3, 30);
    turnTo(-30);
    drive(-3, -30);
    turnTo(30);
    runIntake(0);
    runIntake(-1);
    drive(-6, -30);*/
    //return;
    
    skills();
}

void bringDown()
{
    Shooter.rotateFor(500, rotationUnits::deg);
}

void usercontrol( void )
{
   // bringDown();
    autonomous();
    //turnTo(90);
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
