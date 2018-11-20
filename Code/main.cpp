#include "robot-config.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <vector>
#include <thread>
#include <fstream>
using namespace std;
using namespace vex;

const double PI = 3.1415;
double gyroValue = 0;
double wheelDiameter = 4;
double wheelBaseLength = 12;
double maxHeightLift = 36;
double liftGearRatio = 5;


string toString(double val)
{
    ostringstream ss;
    ss<<val;
    return ss.str();
}
;

void turnLeft(double degrees)
{
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 1.693;
                 FrontLeft.setStopping(brakeType::brake);
                FrontRight.setStopping(brakeType::brake);
                BackLeft.setStopping(brakeType::brake);
                BackRight.setStopping(brakeType::brake);        
    FrontLeft.rotateFor(-rots, vex::rotationUnits::rev, 30, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(-rots, vex::rotationUnits::rev, 30, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(rots, vex::rotationUnits::rev, 30, vex::velocityUnits::pct, false);
    BackRight.rotateFor(rots, vex::rotationUnits::rev, 30, vex::velocityUnits::pct, true);   
                FrontLeft.setStopping(brakeType::coast);
                FrontRight.setStopping(brakeType::coast);
                BackLeft.setStopping(brakeType::coast);
                BackRight.setStopping(brakeType::coast);      
}

void turnRight(double degrees)
{
    double rots = (degrees/360) * ((wheelBaseLength*PI)/(wheelDiameter*PI)) * 1.71;
                FrontLeft.setStopping(brakeType::brake);
                FrontRight.setStopping(brakeType::brake);
                BackLeft.setStopping(brakeType::brake);
                BackRight.setStopping(brakeType::brake);      
    FrontLeft.rotateFor(rots, vex::rotationUnits::rev, 30, vex::velocityUnits::pct, false);
    BackLeft.rotateFor(rots, vex::rotationUnits::rev, 30, vex::velocityUnits::pct, false);
    FrontRight.rotateFor(-rots, vex::rotationUnits::rev, 30, vex::velocityUnits::pct, false);
    BackRight.rotateFor(-rots, vex::rotationUnits::rev, 30, vex::velocityUnits::pct, true);    
                FrontLeft.setStopping(brakeType::coast);
                FrontRight.setStopping(brakeType::coast);
                BackLeft.setStopping(brakeType::coast);
                BackRight.setStopping(brakeType::coast);  
}

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


double distance(double x, double y)
{
    double W0 = 38;
    double H0 = 38;
    double dist0 = 40.5;
    double dx = (W0/x)*dist0;
    double dy = (H0/y)*dist0;
    double d = (dx+dy)/2;
    
    return d;
}

void forward(double inches)
{
    double rev = (inches/(5 * PI))*360;
    
    FrontLeft.rotateFor(rev, rotationUnits::deg, 50, velocityUnits::pct, false);
    FrontRight.rotateFor(rev, rotationUnits::deg, 50, velocityUnits::pct, false);
    BackLeft.rotateFor(rev, rotationUnits::deg, 50, velocityUnits::pct, false);
    BackRight.rotateFor(rev, rotationUnits::deg, 50, velocityUnits::pct, true);    
}

void backward(double inches)
{
    double rev = (inches/(5 * PI))*360;
    
    FrontLeft.rotateFor(-rev, rotationUnits::deg, 50, velocityUnits::pct, false);
    FrontRight.rotateFor(-rev, rotationUnits::deg, 50, velocityUnits::pct, false);
    BackLeft.rotateFor(-rev, rotationUnits::deg, 50, velocityUnits::pct, false);
    BackRight.rotateFor(-rev, rotationUnits::deg, 50, velocityUnits::pct, true);    
    
}
// Red = 0, Blue = 0
int runVision(string color)
{
    // Take Snapshot of the field, puts variables into arrays
    vector<vex::vision::object> greenFlags, redFlags, blueFlags;
    Vision.takeSnapshot(GREENFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
        greenFlags.push_back(Vision.objects[i]);
    
    Vision.takeSnapshot(BLUEFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
        blueFlags.push_back(Vision.objects[i]);
    
    Vision.takeSnapshot(REDFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
        redFlags.push_back(Vision.objects[i]);
    
    // Finds the distance from each flag
    double distAvg = 0;
    int num = 0;
    for(int i = 0; i<((color == "RED") ? redFlags.size() : blueFlags.size()); i++)
    {
        if(color == "RED" && redFlags[i].height > 12 && redFlags[i].width > 12)
        {
            distAvg += distance(redFlags[i].width, redFlags[i].height);
            num++;
        }
        else if(color == "BLUE" && blueFlags[i].height > 12 && blueFlags[i].width > 12)
        {
            distAvg += distance(blueFlags[i].width, blueFlags[i].height);
            num++;            
        }
    }

    distAvg /= num;
    
    // Puts all color into one arraylist
    vector<vex::vision::object> flagsTemp;    
    if(color == "RED")
        flagsTemp = redFlags;
    else if(color == "BLUE")
        flagsTemp = blueFlags;
    
    // Find Flags
    for(int i = 0; i<flagsTemp.size(); i++)
    {
        for(int s = 0; s<greenFlags.size(); s++)
        {
            // Make sure that the flag and green flag indices are matching
            if(abs(flagsTemp[i].centerX - flagsTemp[i].width/2 - greenFlags[s].originX) < 20
                && abs(flagsTemp[i].originY - greenFlags[s].originY) < 20)
            {
                // Strafe to the flags 
                int frontLeftValue = 0, frontRightValue = 0, backLeftValue = 0, backRightValue = 0;
                
                if((flagsTemp[i].centerX - 136) <= -5)
                { 
                    int v = -1;
                    frontLeftValue = 15*v;
                    backLeftValue = -frontLeftValue;
                    frontRightValue = -15*v;
                    backRightValue = 15*v;                         
                }
                else if((flagsTemp[i].centerX - 136) > 10)
                {
                    int v = 1;
                    double t = 0;
                    frontLeftValue = 15*v;
                    backLeftValue = -frontLeftValue;
                    frontRightValue = -15*v;
                    backRightValue = 15*v;                         
                }
                
                FrontLeft.spin(directionType::fwd, frontLeftValue, velocityUnits::pct);
                FrontRight.spin(directionType::fwd, frontRightValue, velocityUnits::pct);       
                BackRight.spin(directionType::fwd, backRightValue, velocityUnits::pct);
                BackLeft.spin(directionType::fwd,  backLeftValue, velocityUnits::pct);      
                if(frontLeftValue == 0) 
                {
                    int d = 0;
                    if(abs(gyroValue) < 22.5)
                    {
                        //turnTo(0);
                        d = 1;
                    }
                    else
                        Controller1.rumble("-.-.-.-.-");                    
                    // Go forward/backward to the flags
                    if(distAvg > 44)
                    {
                        forward(distAvg - 42);
                    }else if(distAvg < 40)
                    {
                        backward(42- distAvg);
                    }                     
                    task::sleep(200);
                    return 0;
                }
            }
        }
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
            Shooter.rotateFor(1850,rotationUnits::deg,100,velocityUnits::pct); 
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

int taskGyro()
{
    double prevVal, currVal, delta;
    while (true)
    {
        currVal = -GyroS.value(rotationUnits::deg) + GyroI.value(rotationUnits::deg);
        currVal /= 2;
        delta = currVal - prevVal;
        
        // Gyro roll-over
        if (currVal >= 0 && currVal < 90 && prevVal > 270 && prevVal < 360)
        {
            delta += 360;
        }
        else if (prevVal >= 0 && prevVal < 90 && currVal > 270 && currVal < 360)
        {
            delta -= 360;
        }
 
        if (currVal <= 0 && currVal > -90 && prevVal < -270 && prevVal > -360)
        {
            delta -= 360;
        }
        else if (prevVal <= 0 && prevVal > -90 && currVal < -270 && currVal > -360)
        {
            delta += 360;
        }
        
        gyroValue += fmod((delta * 4/3), 360);
        gyroValue = fmod(gyroValue, 360);
        //gyroValue = -GyroS.value(rotationUnits::) + GyroI.value(analogUnits::range12bit);
        //gyroValue /= 2;
        string isBraked = toString(gyroValue);
        Controller1.Screen.clearLine();
        Controller1.Screen.print(isBraked.c_str());        
        prevVal = currVal;
        task::sleep(10);
    }
    
    return -1;
}


int main() {
    int encoderPositionShooter = Shooter.rotation(rotationUnits::deg);
    //task shooterTask(taskShooter);
    vex::task(taskShooter, 1);
    vex::task(taskGyro, 1);
    //vex::task(visionTask, 1);
    double firstHundred = 0;
    double secondHundred = 0;
    double thirdHundred = 0;
    double fourthHundred = 0;
    double angleStrafe = 0;
    bool strafing = false;
    bool slow = false;

    GyroS.startCalibration(3000);
    GyroI.startCalibration(3000);    
    task::sleep(3000);
    bool braked = false;
    Controller1.Screen.clearScreen();
    //while(GyroS.isCalibrating() || GyroI.isCalibrating());
    while(true)
    {  
        int frontLeftValue = 0, frontRightValue = 0, backLeftValue = 0, backRightValue = 0;
        
        if (slow == false && abs(Controller1.Axis3.value()) > 20)
        {
            frontLeftValue = Controller1.Axis3.value();
            backLeftValue = Controller1.Axis3.value();  
            strafing = false;
        }
           
        if (slow == false && abs(Controller1.Axis2.value()) > 20)
        {
            frontRightValue = Controller1.Axis2.value();
            backRightValue = frontRightValue;
            strafing = false;
        }
        
        if (slow)
        {
            frontRightValue /= 2;
            backLeftValue /= 2;
            backRightValue /2;
            frontLeftValue /=2;
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
            t = (gyroValue - angleStrafe);
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
            int d = 0;
            if(abs(gyroValue) < 22.5)
            {
              //  turnTo(0);
                d = 1;
            }
            else
                Controller1.rumble("-.-.-.-.-");
            if(d == 1)
            {
                while(runVision("RED") == -1);
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
        
        

        
        string isSlow = "Slow: " + toString(slow);
        //Controller1.Screen.print(slow);
        
        task::sleep(20);
    }
    return -1;
}
