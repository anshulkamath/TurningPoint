#include "robot-config.h"
//Creates a competition object that allows access to Competition methods.
vex::competition    Competition;

#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;
using namespace vex;
double PI = 3.1415;
double wheelDiameter = 4;
double wheelBaseLength = 12;
double maxHeightLift = 36;
double liftGearRatio = 5;
double gyroValue = 0;

string side = "RED";
string toString1(double val)
{
    ostringstream v;
    v << val;
    return v.str();
}
void sideSelect()
{
    int pressX = Brain.Screen.xPosition();
    int pressY = Brain.Screen.yPosition();
    
    if(pressX < 100) side = "BLUE";
    if(pressX > 100) side = "RED";
    /*if(side == "BLUE")
        Vision.setLedColor(vex::color::blue);
    if(side == "RED")
        Vision.setLedColor(vex::color::red);*/
    Brain.Screen.clearScreen();
    Brain.Screen.print(("Side: " + side).c_str());
    
    
}
double biasX = AccelX.value(analogUnits::range12bit);
double biasY = AccelY.value(analogUnits::range12bit);
double x = 0;
double y = 0;
double velocityX = 0;
double velocityY = 0;
void pre_auton( void ) {
    GyroS.startCalibration(2);
    GyroI.startCalibration(2);    

    task::sleep(3000);
    
    biasX = AccelX.value(analogUnits::range12bit);
    biasY = AccelY.value(analogUnits::range12bit); 
    velocityX = 0;
    velocityY = 0;
    x = 0;
    y = 0;
    gyroValue = 0;
    Brain.Screen.setPenColor(vex::color::blue);
    Brain.Screen.drawRectangle(0, 0, 100, 100, vex::color::blue);
    Brain.Screen.drawRectangle(101, 0, 100, 100, vex::color::red); 
    Brain.Screen.pressed(sideSelect);
}

vector<double> xAccelValues, yAccelValues;

double angle()
{
    return (GyroS.value(analogUnits::range12bit))/10;
}

double median(double x, double y, double z)
{
	if (x > y)
    		swap(x, y);
	if (x > z)
    		swap(x, z);
	if (y > z)
    		swap(y, z);
	return y;
}

double filteredData(vector<double> data)
{
	vector<double> tempData;
	for(int i = 0; i<data.size(); i++)
	{
		if(i == 0)
		{
			tempData.push_back(data[0], data[0], data[1]);
		}else
		{
			tempData.push_back(data[i-1], data[i], data[i+1]);
		}
	}
	sort(tempData.begin(), tempData.end());
	return tempData[tempData.size()/2];
}

int fps()
{

    const double fracSec = .05;
    int lastMoveX = 0;
    int lastMoveY = 0;
    while(true)
    {
	xAccelValues.push_back((AccelX.value(analogUnits::range12bit) - biasX));
	yAccelValues.push_back((AccelY.value(analogUnit::range12bit) - biasY));
	double filteredX, filteredY;
	if(xAccelValues.size() > 6 && yAccelValues.size() > 6)
	{
		xAccelValues.erase(xAccelValues.begin());
		yAccelValues.erase(yAccelValues.begin());
		
		filteredX = filteredData(xAccelValues);
		filteredY = filteredData(yAccelValues);
		
		continue;
	}
	    
        if(abs(AccelX.value(analogUnits::range12bit) - biasX) > 1) {
            velocityX += (AccelX.value(analogUnits::range12bit) - biasX) * fracSec;
            lastMoveX = 0;
        }
        else if(lastMoveX > 5)
            velocityX = 0;
        if((AccelY.value(analogUnits::range12bit) - biasY) > 1)
        {
            velocityY += (AccelY.value(analogUnits::range12bit) - biasY) * fracSec;
            lastMoveY = 0;
        }
        else if(lastMoveY > 5)
            velocityY = 0;
        double deltaPos = velocityX * fracSec;
        x += std::cos((angle() * PI)/180) * deltaPos;
        y += std::sin((angle()* PI)/180) * deltaPos;
        deltaPos = velocityY * fracSec;
        x += std::cos((angle() * PI)/180) * deltaPos;
        y += std::sin((angle()* PI)/180) * deltaPos;
        lastMoveX++;
        lastMoveY++;
        task::sleep(20);
    }
    return 0;
}

double distance(double x, double y)
{
    double W0 = 38;
    double H0 = 36;
    double dist0 = 39.2;
    double dx = (W0/x)*dist0;
    double dy = (H0/y)*dist0;
    //double dgy = (H0/greenY) * dist0;
    double d = (dx+dy)/2;
    //d = cos(.3594444 * PI) * d;
    return d;
}

void forward(double inches, double speed = 70)
{
    double rots = inches/(wheelDiameter*PI);
    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, true);    
}

void backward(double inches, double speed = 50)
{
    double rots = inches/(wheelDiameter*PI);
    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, speed, vex::velocityUnits::pct, true);        
}

void turnLeft(double degrees)
{
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 1.693;
    
    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, true);    
}

void turnRight(double degrees)
{
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 1.71;
    
    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, true);    
}

void strafeLeft(double inches)
{
    double rots = inches/(wheelDiameter*PI) * sqrt(2);
    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, true);
}

void strafeRight(double inches)
{
    double rots = inches/(wheelDiameter*PI) * sqrt(2);
    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 70, vex::velocityUnits::pct, true);
}

vector<vex::vision::object> getFlags(string color)
{
    vector<vex::vision::object> greenFlags, flags, flagsToReturn;
    Vision.takeSnapshot(GREENFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
        greenFlags.push_back(Vision.objects[i]);
    if(color=="BLUE") {
        Vision.takeSnapshot(BLUEFLAG);
        for(int i = 0; i<Vision.objectCount; i++)
            flags.push_back(Vision.objects[i]);
    }
    if(color == "RED")
    {
        Vision.takeSnapshot(REDFLAG);
        for(int i = 0; i<Vision.objectCount; i++)
            flags.push_back(Vision.objects[i]);
    } 
    
    for(int i = 0; i<flags.size(); i++)
    {
        for(int s = 0; s<greenFlags.size(); s++)
        {
            if(abs(flags[i].centerX - flags[i].width/2 - greenFlags[s].originX) < 12
                && abs(flags[i].originY - greenFlags[s].originY) < 10
              )
            {        
                if(flags[i].height > 15 && flags[i].width > 15 && flags[i].width <= flags[i].height + 5 
                   && abs(flags[i].width/flags[i].height - 1) < .25)
                {
                    flagsToReturn.push_back(flags[i]);
                }
            }
        }
    }
    return flagsToReturn;
}

class Object
{
public:
    int width, height, x, y, centerX, centerY;
    Object(int w, int h, int x1, int y1, int centerX1, int centerY1)
    {
        width = w;
        height = h;
        x = x1;
        y = y1;
        centerX = centerX1;
        centerY = centerY1;
    }
};

vector<Object> averageFlag(vector<vector<vex::vision::object>> in)
{
    vector<Object> total;
    for(int i = 0; i<in.size(); i++)
    {
        double width = 0, height = 0, x = 0, y = 0, centerX = 0, centerY = 0;
        for(int s = 0; s<in[i].size(); s++)
        {
            width += in[i][s].width;
            height += in[i][s].height;
            x += in[i][s].originX;
            y += in[i][s].originY;
            centerX += in[i][s].centerX;
            centerY += in[i][s].centerY;
        }
        int num = in[i].size();
        width /= num;
        height /= num;
        x /= num;
        y /= num;
        centerX /= num;
        centerY /= num;
        total.push_back(Object(width, height, x, y, centerX, centerY));
    }
    return total;
}

int runAutoAlign(string color)
{
    bool done = false;
    double lastDistance = 0;
    while(!done)
    {
        vector<vector<vision::object>> objs;
        for(int i = 0; i<5; i++)
            objs.push_back(getFlags(color));
        vector<Object> d = averageFlag(objs);
        Object target = d[0];
        int frontLeftValue = 0, frontRightValue = 0, backLeftValue = 0, backRightValue = 0;

        lastDistance = distance(target.width, target.height);

        if(!done  && (target.x - 142) < -1)
        { 
            int v = 1;
            frontLeftValue = -6.5*v;
            backLeftValue = frontLeftValue;
            frontRightValue =6.5*v;
            backRightValue = 6.5*v;                         
        }
        else if(!done && (target.x - 142) > 1)
        {
            int v = 1;
            double t = 0;
            frontLeftValue = 6.5*v;
            backLeftValue = frontLeftValue;
            frontRightValue = -6.5*v;
            backRightValue = -6.5*v;                         
        }
        if(frontLeftValue == 0) done = true;
    }
    if(lastDistance < 39)
        backward(40-lastDistance, 40);
    else if(lastDistance > 41)
        forward(lastDistance - 40, 40);
    return 0;
}

int intakeCont = 0;
int intakeControl()
{
    Intake.setVelocity(100, vex::velocityUnits::pct);
    while(true)
    {
        if(intakeCont == 0)
            Intake.stop();
        else if(intakeCont == -1)
            Intake.spin(vex::directionType::rev);
        else if(intakeCont == 1)
            Intake.spin(vex::directionType::fwd);
    }
    return -1;
}
bool inUse = false;
bool inTakeInUse = false;
bool fire = false;
int taskShooter()
{
    while(true)
    {
        if(Controller1.ButtonX.pressing() || fire)
        {
            fire = false;
            inUse = true;
            Shooter.spin(directionType::fwd, 100, velocityUnits::pct);
            bool notTop = false;
            while(!Limit1.pressing())
            {
                notTop = true;
            }
            
            Shooter.stop();
            task::sleep(500);
            Shooter.spin(directionType::fwd, 100, velocityUnits::pct);
            
            while(Limit1.pressing());
            
            Shooter.stop();
            Shooter.rotateFor(1800,rotationUnits::deg,100,velocityUnits::pct); 
            inUse = false;
        }       
        if(Limit2.pressing())
        {
            inTakeInUse = true;
            Intake.spin(directionType::rev, 100, velocityUnits::pct);
            task::sleep(500);
            Intake.stop();
            inTakeInUse = false;
        }
        
    }
    return 0;
}

void moveArm(double inches)
{
    double percentOfMax = inches/maxHeightLift;
    
    double rot = percentOfMax * liftGearRatio;
    
    Lift.rotateFor(rot, rotationUnits::rev, 100, velocityUnits::pct);
}

void getOnPlatform()
{
    forward(35, 100);
    forward(60, 100);
}


void park(string side)
{
    backward(11);
    if(side == "Blue")
        turnRight(45);
    else
        turnLeft(45);
    backward(50);
}

// Grabs ball, flips cap, shoots, scores bottom flag
void autonFunc1(string side)
{
    //task intakeTask = task(intakeControl);
    task shooterTask = task(taskShooter);
    Lift.rotateFor(-30, rotationUnits::deg, 50, velocityUnits::pct, true);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);
    backward(39.5);
    
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);
    task::sleep(300);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);
    task::sleep(1000);    
    
    if(side == "RED")
    {
        forward(34.5);
        task::sleep(600);
        turnRight(90);
    }
    else
    {
        forward(29.5);
        task::sleep(300);        
        turnLeft(90);
    }
    task::sleep(700);
    //Brain.resetTimer();
    //while(!Limit2.pressing() || Brain.timer(timeUnits::msec) < 700);
    Intake.spin(directionType::rev, 100, velocityUnits::pct);
    task::sleep(1500);
    //while(runVision("BLUE") == -1);
    forward(14, 50);
    fire = true;
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);
    task::sleep(1000);
    if (side == "RED")
    {
       strafeRight(2.5); 
    }
    else if (side == "BLUE")
    {
        strafeLeft(3);
    }
    fire = false;
    forward(53);
}

// What do we want to do for this auton?
void auton2(string side)
{
   task shooterTask = task(taskShooter);
    Lift.rotateFor(-30, rotationUnits::deg, 50, velocityUnits::pct, true);    
    forward(32);
    if(side == "BLUE")
    {
        turnRight(90);
    }else if(side == "RED")
    {
        turnLeft(90);
    }
    forward(38, 100);
}
int taskLift()
{
    task::sleep(600);
Lift.rotateFor(440, rotationUnits::deg, 100, velocityUnits::pct, false);       
    return 0 ;
}
void autoFuncBack(string side)
{
    Lift.rotateFor(-1100, rotationUnits::deg, 100, velocityUnits::pct, true);
    //while(abs(Lift.rotation(rotationUnits::deg)) < 1900);
Lift.rotateFor(-940, rotationUnits::deg, 100, velocityUnits::pct, false);  
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);
    backward(30);
    backward(2, 40);
    Intake.spin(directionType::fwd, 0, velocityUnits::pct);
    task::sleep(300);
    Intake.spin(directionType::fwd, 100, velocityUnits::pct);
    task::sleep(750); 
    forward(12);
    if(side == "BLUE")
        turnRight(149);
    else
        turnLeft(130);
    forward(18, 100);
Lift.rotateFor(600, rotationUnits::deg, 100, velocityUnits::pct, true); 
    if(side == "BLUE")
        turnRight(30);
    else
        turnLeft(30);
    forward(13, 50);
Lift.rotateFor(-600, rotationUnits::deg, 100, velocityUnits::pct, false);    
    task::sleep(1800);
Lift.rotateFor(300, rotationUnits::deg, 100, velocityUnits::pct, false);    
    backward(21);
    if(side == "BLUE")
        turnRight(95);
    else
        turnLeft(95);
    forward(49, 100);
     //turnRight(15);
    //forward(20, 100);
    /*turnLeft(20);
Lift.rotateFor(-2040, rotationUnits::deg, 100, velocityUnits::pct, false); 
    forward(10, 100);
    turnRight(20);
    forward(10, 100);
    turnRight(90);
    forward(10, 100);
    turnLeft(90);
    turnRight(45);
    forward(25, 100);
    Lift.rotateFor(100, rotationUnits::deg, 100, velocityUnits::pct);
    backward(15, 40);
    turnLeft(90);*/
}
void autonomous( void ) {
    //autonFunc1(side);
    //auton2(side);
    autoFuncBack("RED");
}
string toString(double val)
{
    ostringstream ss;
    ss<<val;
    return ss.str();
}
;


void turnTo(double degrees)
{
    if(gyroValue < degrees)
    {
        turnRight(degrees - gyroValue);
    }else if(gyroValue > degrees)
    {
        turnLeft(gyroValue - degrees);
    }
}
int prevError = 100;

// Red = 0, Blue = 0
int runVision(string color, bool check = false)
{
    // Take Snapshot of the field, puts variables into arrays
    vector<vex::vision::object> greenFlags, redFlags, blueFlags;
    Vision.takeSnapshot(GREENFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
        greenFlags.push_back(Vision.objects[i]);
    if(color=="BLUE") {
        Vision.takeSnapshot(BLUEFLAG);
        for(int i = 0; i<Vision.objectCount; i++)
            blueFlags.push_back(Vision.objects[i]);
    }
    if(color == "RED")
    {
        Vision.takeSnapshot(REDFLAG);
        for(int i = 0; i<Vision.objectCount; i++)
            redFlags.push_back(Vision.objects[i]);
    }
    
    // Finds the distance from each flag
    double distAvg = 0;
    int num = 0;
    // Puts all color into one arraylist
    vector<vex::vision::object> flagsTemp;    
    if(color == "RED")
        flagsTemp = redFlags;
    else if(color == "BLUE")
        flagsTemp = blueFlags;
    for(int i = 0; i<flagsTemp.size(); i++)
    {
        for(int s = 0; s<greenFlags.size(); s++)
        {
            // Make sure that the flag and green flag indices are matching
            if(abs(flagsTemp[i].centerX - flagsTemp[i].width/2 - greenFlags[s].originX) < 12
                && abs(flagsTemp[i].originY - greenFlags[s].originY) < 10
              )
            {        
                if(flagsTemp[i].height > 15 && flagsTemp[i].width > 15 && flagsTemp[i].width <= flagsTemp[i].height + 5 && abs(flagsTemp[i].width/flagsTemp[i].height - 1) < .25)
                {
                    distAvg += distance(flagsTemp[i].width, flagsTemp[i].height);
                    num++;
                }
            }
        } 
    }

    distAvg /= num;
    
                Controller1.Screen.clearLine();
                string diff = toString(flagsTemp.size()) + " " + color;
    Controller1.Screen.print(diff.c_str());
    // Find Flags
    for(int i = 0; i<flagsTemp.size(); i++)
    {
        for(int s = 0; s<greenFlags.size(); s++)
        {
            // Make sure that the flag and green flag indices are matching
            if(abs(flagsTemp[i].centerX - flagsTemp[i].width/2 - greenFlags[s].originX) < 20
                && abs(flagsTemp[i].originY - greenFlags[s].originY) < 10 
              )
            {
                // Strafe to the flags 
                int frontLeftValue = 0, frontRightValue = 0, backLeftValue = 0, backRightValue = 0;
                int error = abs(flagsTemp[i].originX - 128);
                bool done = false;
               // if(check && (error + prevError)/2 < 1.5) done = true;
                //if(!check && (error + prevError)/2 < 8) done = true;
                prevError = error;
                if(!done && !check && (flagsTemp[i].originX - 118) < -1)
                { 
                    int v = 1;
                    frontLeftValue = -6.5*v;
                    backLeftValue = frontLeftValue;
                    frontRightValue =6.5*v;
                    backRightValue = 6.5*v;                         
                }
                else if(!done && !check && (flagsTemp[i].originX - 118) > 1)
                {
                    int v = 1;
                    double t = 0;
                    frontLeftValue = 6.5*v;
                    backLeftValue = frontLeftValue;
                    frontRightValue = -6.5*v;
                    backRightValue = -6.5*v;                         
                }                
                /*if(!done && check && (flagsTemp[i].originX - 142) < -1)
                { 
                    int v = 1;
                    frontLeftValue = -6.5*v;
                    backLeftValue = frontLeftValue;
                    frontRightValue =6.5*v;
                    backRightValue = 6.5*v;                         
                }
                else if(!done && check && (flagsTemp[i].originX - 142) > 1)
                {
                    int v = 1;
                    double t = 0;
                    frontLeftValue = 6.5*v;
                    backLeftValue = frontLeftValue;
                    frontRightValue = -6.5*v;
                    backRightValue = -6.5*v;                         
                }*/

                Controller1.Screen.print(diff.c_str());
                FrontLeft.spin(directionType::fwd, frontLeftValue, velocityUnits::pct);
                FrontRight.spin(directionType::fwd, frontRightValue, velocityUnits::pct);       
                BackRight.spin(directionType::fwd, backRightValue, velocityUnits::pct);
                BackLeft.spin(directionType::fwd,  backLeftValue, velocityUnits::pct);      
                if(frontLeftValue == 0 || done) 
                {
                    if(check == true) return 0;
                    task::sleep(100);
                    int d = 0;                 
                    // Go forward/backward to the flags
                    if(distAvg > 38)
                    {
                        forward(distAvg - 37, 20);
                    }else if(distAvg < 36)
                    {
                        backward(37 - distAvg, 20);
                    }
                    
                    if(check) return 0;
                   task::sleep(100);                    
                    while(runVision(color, true) == -1);
                    task::sleep(300);
                    return 0;
 
                   // return 0;
                }
            }
        }
    }
    if(flagsTemp.size() < 1) return 2;
    return -1;
}




int taskGyro()
{
    double prevVal, currVal, delta;
    while (true)
    {
        currVal = GyroS.value(rotationUnits::deg) - GyroI.value(rotationUnits::deg);
        currVal /= 2;
        delta = currVal - prevVal;
        
        // Gyro roll-over
		if(abs(delta) > 180)
		{
		
			if(delta > 0)
			{
				delta = -(360 - delta);
			}else
			{
				delta = (delta + 360);
			}
		}

		if(abs(delta) > 5)
		{
			int x = 0;
		}
       /* if (currVal >= 0 && currVal < 90 && prevVal > 270 && prevVal < 360)
        {
            delta += 360;
        }
        else if (prevVal >= 0 && prevVal < 90 && currVal > 270 && currVal < 360)
        {
            delta -= 360;
        }else if (currVal <= 0 && currVal > -90 && prevVal < -270 && prevVal > -360)
        {
            delta -= 360;
        }
        else if (prevVal <= 0 && prevVal > -90 && currVal < -270 && currVal > -360)
        {
            delta += 360;
        }*/

        gyroValue += fmod(((delta) * 4/3), 360);
        gyroValue = fmod(gyroValue, 360);
        prevVal = currVal;
        string isBraked = toString(gyroValue);
       // Controller1.Screen.clearLine();
     //   Controller1.Screen.print(isBraked.c_str());        
        prevVal = currVal;
        task::sleep(10);
    }
    
    return -1;
}


void usercontrol() {
    int encoderPositionShooter = Shooter.rotation(rotationUnits::deg);
    //task shooterTask(taskShooter);
    vex::task(taskShooter, 1);
    vex::task(taskGyro, 1);
    task(fps, 1);
    //vex::task(visionTask, 1);
    double firstHundred = 0;
    double secondHundred = 0;
    double thirdHundred = 0;
    double fourthHundred = 0;
    double angleStrafe = 0;
    bool strafing = false;
    bool slow = false;

    
    bool braked = false;
    Controller1.Screen.clearScreen();
    //while(GyroS.isCalibrating() || GyroI.isCalibrating());
    while(true)
    {  
        int frontLeftValue = 0, frontRightValue = 0, backLeftValue = 0, backRightValue = 0;
        
        if ( abs(Controller1.Axis3.value()) > 20)
        {
            frontLeftValue = Controller1.Axis3.value();
            backLeftValue = Controller1.Axis3.value();  
            strafing = false;
        }
           
        if (abs(Controller1.Axis2.value()) > 20)
        {
            frontRightValue = Controller1.Axis2.value();
            backRightValue = frontRightValue;
            strafing = false;
        }
        
        
        
        
        // 100.65 -455.6
        // 199.9  -914
       
       if(Controller1.ButtonRight.pressing() || Controller1.ButtonLeft.pressing())
        {
            if(!strafing) angleStrafe = gyroValue; 
            strafing = true;
            int v = 1;
            if(Controller1.ButtonLeft.pressing()) v = -1;
            double t = 0;
           // t = (gyroValue - angleStrafe);
            frontLeftValue = 100*v + t*2;
            backLeftValue = -frontLeftValue -t*2;
            frontRightValue = -100*v -t*2;
            backRightValue = 100*v + t*2;           
        }   
        
        if (Controller1.ButtonR1.pressing())
            Lift.spin(directionType::fwd, 100, velocityUnits::pct);
        else if (Controller1.ButtonR2.pressing())
            Lift.spin(directionType::rev, 100, velocityUnits::pct);
        else
            Lift.stop(brakeType::hold);
        if (slow)
        {
            frontRightValue /= 2;
            backLeftValue /= 2;
            backRightValue /2;
            frontLeftValue /=2;
        }        
        if(Controller1.ButtonDown.pressing())
        {
            if(!braked) 
            {
                FrontLeft.stop(brakeType::hold);
                FrontRight.stop(brakeType::hold);
                BackLeft.stop(brakeType::hold);
                BackRight.stop(brakeType::hold);    
            }
            else
            {
                FrontLeft.setStopping(brakeType::coast);
                FrontRight.setStopping(brakeType::coast);
                BackLeft.setStopping(brakeType::coast);
                BackRight.setStopping(brakeType::coast);                 
            }
            braked = !braked;
            
            while(Controller1.ButtonDown.pressing()){}
        }
        
        if (Controller1.ButtonUp.pressing())
        {
            slow = !slow;
            while(Controller1.ButtonUp.pressing()){}
        }
        
        if (Controller1.ButtonY.pressing())
            Shooter.spin(directionType::fwd, 60, velocityUnits::pct);
        else if(!inUse)
            Shooter.stop();
        
        if(Controller1.ButtonB.pressing())
        {
            int d = 1;
            if(d == 1)
            {
                int c = -1;
                while(c == -1) c = runVision(side);
                if(c == 2)
                {
                    
                }else
                    fire = true;
            }
        }
        
        if (Controller1.ButtonL1.pressing())
            Intake.spin(directionType::fwd, 100, velocityUnits::pct);
        
        else if(Controller1.ButtonL2.pressing())
            Intake.spin(directionType::rev, 100, velocityUnits::pct);
        
        else if(!inTakeInUse)
            Intake.stop(brakeType::hold);
        
        if (Controller1.ButtonUp.pressing())
            SpinnyThingy.spin(directionType::fwd, 50,  velocityUnits::pct);

        else if(Controller1.ButtonDown.pressing())
            SpinnyThingy.spin(directionType::rev, 50,  velocityUnits::pct);            
        
        else
            SpinnyThingy.stop(brakeType::hold);            
        
       
        FrontLeft.spin(directionType::fwd, frontLeftValue, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, frontRightValue, velocityUnits::pct);       
        BackRight.spin(directionType::fwd, backRightValue, velocityUnits::pct);
        BackLeft.spin(directionType::fwd,  backLeftValue, velocityUnits::pct);     
        
        string toque = toString(gyroValue) + " " + toString(GyroS.value(rotationUnits::rev)) + " "
            + toString(GyroI.value(rotationUnits::rev));
        
         string val1 = /*"Angle " + toString1(angle()) +" X " + toString1(x)
            +*/ " Y " + toString1(y);
        Controller1.Screen.clearScreen();
        Controller1.Screen.print(val1.c_str());       

        
        string isSlow = "Slow: " + toString(slow) + " " + "Brake: " + toString(braked);
       // Controller1.Screen.print(isSlow.c_str());
        
        task::sleep(20);
    }
}
//
// Main will set up the competition functions and callbacks.
//
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
