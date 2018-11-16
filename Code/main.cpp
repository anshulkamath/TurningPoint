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

string intToString(double val)
{
    ostringstream ss;
    ss<<val;
    return ss.str();
}
;

double distance(double x, double y)
{
    double W0 = 38;
    double H0 = 40;
    double dist0 = 36.5;
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

int runVision()
{
    vector<vex::vision::object> greenFlags, redFlags, blueFlags;
    Vision.takeSnapshot(GREENFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
    {
        //if(Vision.objects[i].id == 1)
        {
            greenFlags.push_back(Vision.objects[i]);
        }
    }
    Vision.takeSnapshot(BLUEFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
    {
        //if(Vision.objects[i].id == 2)
        {
            blueFlags.push_back(Vision.objects[i]);
        }
    }
    Vision.takeSnapshot(REDFLAG);
    for(int i = 0; i<Vision.objectCount; i++)
    {
        //if(Vision.objects[i].id == 3)
        {
            redFlags.push_back(Vision.objects[i]);
        }
    } 
    int redFlagsF1 = 0;
    string diff = "";
    double distAvg = 0;
    int num = 0;
    for(int i = 0; i<redFlags.size(); i++)
    {
        if(redFlags[i].height > 12 && redFlags[i].width > 12)
        {
            //if(abs(redFlags[i].centerX - redFlags[i].width/2 - greenFlags[s].originX) < 20)
            {
                distAvg += distance(redFlags[i].width, redFlags[i].height);
                num++;
            }
        }
    }
    distAvg /= num;
    for(int i = 0; i<redFlags.size(); i++)
    {
       // diff += "R:" + intToString(redFlags[i].originX) +"x" + intToString(redFlags[i].width);
        for(int s = 0; s<greenFlags.size(); s++)
        {
            //diff += "G:"+intToString(greenFlags[s].originY);
            if(abs(redFlags[i].centerX - redFlags[i].width/2 - greenFlags[s].originX) < 20)
            {

              
                
                if(abs(redFlags[i].originY - greenFlags[s].originY) < 20)
                {
                                        diff = intToString(distAvg);
                    Brain.Screen.clearLine();
                    Brain.Screen.print(diff.c_str());

                    
                    int frontLeftValue = 0, frontRightValue = 0, backLeftValue = 0, backRightValue = 0;
                    //diff = intToString(redFlags[i].centerX);
                    redFlagsF1++;
                   // diff = distAvg;//intToString(redFlags[i].angle);
                    Controller1.Screen.clearScreen();
                    Controller1.Screen.clearLine();    
                    Controller1.Screen.print(diff.c_str());                    
                    if((redFlags[i].centerX-148) < -5)
                    { 
                        int v = -1;
                        frontLeftValue = 20*v;
                        backLeftValue = -frontLeftValue;
                        frontRightValue = -20*v;
                        backRightValue = 20*v;                         
                    }else if((redFlags[i].centerX - 148) > 10)
                    {
                        int v = 1;
                        double t = 0;
                        frontLeftValue = 20*v;
                        backLeftValue = -frontLeftValue;
                        frontRightValue = -20*v;
                        backRightValue = 20*v;                         
                    }else
                    {
                        
                    }
                    FrontLeft.spin(directionType::fwd, frontLeftValue, velocityUnits::pct);
                    FrontRight.spin(directionType::fwd, frontRightValue, velocityUnits::pct);       
                    BackRight.spin(directionType::fwd, backRightValue, velocityUnits::pct);
                    BackLeft.spin(directionType::fwd,  backLeftValue, velocityUnits::pct);      
                    if(frontLeftValue == 0) 
                    {
                        if(distAvg > 44)
                        {
                            forward(distAvg - 42);
                        }else if(distAvg < 40)
                        {
                            backward(42- distAvg);
                        }                        
                        return 0;
                    }
                    //runVision();
                    // Red Flag Found
                }
            }
        }
    }
    int blueFlagsF1 = 0;
    for(int i = 0; i<blueFlags.size(); i++)
    {
        for(int s = 0; s<greenFlags.size(); s++)
        {
            if(abs(blueFlags[i].originX - (greenFlags[s].originX+greenFlags[s].width)) < 10)
            {
                if(abs(blueFlags[i].originY - greenFlags[s].originY) < 10)
                {
                    blueFlagsF1++;
                    // Blue Flag Found
                }
            }
        }
    }
    
    string line = intToString(blueFlags.size()) + " " + intToString(redFlags.size()) + " " + intToString(greenFlags.size()) + " " + intToString(redFlagsF1);
    Controller1.Screen.clearScreen();
    Controller1.Screen.clearLine();    
    Controller1.Screen.print(diff.c_str());
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
        currVal = GyroS.value(rotationUnits::deg) - GyroI.value(rotationUnits::deg);
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
        prevVal = currVal;
        task::sleep(50);
    }
    
    return -1;
}

/*
double prev1 = 0;
double offset = 0;
bool first = true;
double getAngle()
{
    double gyroValue = -GyroS.value(rotationUnits::deg) 
        + GyroI.value(rotationUnits::deg);
    gyroValue /= 2;
    double val = -fmod(GyroS.value(rotationUnits::deg), 270) 
        + fmod(GyroI.value(rotationUnits::deg), 270);
    const double gyroScale = 4.0/3.0;
    

    
    if(gyroValue >= 0 && gyroValue < 90 && prev1 > 270 && prev1 < 360)
    {
        if(!first)
        {
            offset += 90;
            Controller1.rumble("-.-.-.-");            
        }else
        {
            first = false;
        }
    }else if(gyroValue < 360 && gyroValue >= 270 && prev1 >= 0 && prev1 < 90)
    {
        if(!first)
        {
            offset -= 90;
            Controller1.rumble("-.-.-.-");
        }else
        {
            first = false;
        }
    }
    
    prev1 = gyroValue;
    double curr = fmod(((val/2 + offset) * gyroScale), 360);
    return curr;
}
*/

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

    GyroS.startCalibration(3000);
    GyroI.startCalibration(3000);    
    task::sleep(3000);
    bool braked = false;
    Controller1.Screen.clearScreen();
    //while(GyroS.isCalibrating() || GyroI.isCalibrating());
    while(true)
    {  
        int frontLeftValue = 0, frontRightValue = 0, backLeftValue = 0, backRightValue = 0;
        if(abs(Controller1.Axis3.value()) > 20)
        {
            frontLeftValue = Controller1.Axis3.value();
            backLeftValue = Controller1.Axis3.value();  
            strafing = false;
        }
           
        if(abs(Controller1.Axis2.value()) > 20)
        {
            frontRightValue = Controller1.Axis2.value();
            backRightValue = frontRightValue;
            strafing = false;
        }
        // 100.65 -455.6
        // 199.9  -914
       
       if(Controller1.ButtonRight.pressing() || Controller1.ButtonLeft.pressing())
        {
            if(!strafing) angleStrafe = gyroValue(); 
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
        
        if(Controller1.ButtonR1.pressing() && Lift.rotation(rotationUnits::rev) < 2.42)
        {
            Lift.spin(directionType::fwd, 100, velocityUnits::pct);
        }else if(Controller1.ButtonR2.pressing() && Lift.rotation(rotationUnits::deg) > 20)
        {
            Lift.spin(directionType::rev, 100, velocityUnits::pct);
        }else
        {
            Lift.stop(brakeType::hold);
        }
        
        if(Controller1.ButtonDown.pressing())
        {
            if(!braked) {
                FrontLeft.stop(brakeType::hold);
                FrontRight.stop(brakeType::hold);
                BackLeft.stop(brakeType::hold);
                BackRight.stop(brakeType::hold);    
            }else
            {
                FrontLeft.setStopping(brakeType::coast);
                FrontRight.setStopping(brakeType::coast);
                BackLeft.setStopping(brakeType::coast);
                BackRight.setStopping(brakeType::coast);                 
            }
            braked = !braked;
        }
        
        if(Controller1.ButtonY.pressing())
        {
            Shooter.spin(directionType::fwd, 60, velocityUnits::pct);
        }else if(!inUse)
        {
            Shooter.stop();
        }
        
        if(Controller1.ButtonB.pressing())
        {
            /*if(abs(getAngle()) < 22.5)
            {
                turnTo(0);
            }else if(abs(abs(getAngle())-45) < 22.5)
            {
                turnTo(45);
            }else
            {
                Controller1.rumble("-.-.-.-.-");
            }*/
            //if(n)
            {
                while(runVision() == -1);
                fire = true;
            }
        }
        
        if(Controller1.ButtonL1.pressing())
        {
            Intake.spin(directionType::fwd, 100, velocityUnits::pct);
        }else if(Controller1.ButtonL2.pressing())
        {
            Intake.spin(directionType::rev, 100, velocityUnits::pct);
        }else if(!inTakeInUse)
        {
            Intake.stop(brakeType::hold);
        }   
        
        if(Controller1.ButtonUp.pressing())
        {
            SpinnyThingy.spin(directionType::fwd, 50,  velocityUnits::pct);
        }else if(Controller1.ButtonDown.pressing())
        {
            SpinnyThingy.spin(directionType::rev, 50,  velocityUnits::pct);            
        }else
        {
            SpinnyThingy.stop(brakeType::hold);            
        }
        
        // 1924.8 1788 2008.4
        FrontLeft.spin(directionType::fwd, frontLeftValue, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, frontRightValue, velocityUnits::pct);       
        BackRight.spin(directionType::fwd, backRightValue, velocityUnits::pct);
        BackLeft.spin(directionType::fwd,  backLeftValue, velocityUnits::pct);     
        //string val = "Value: " + intToString(Shooter.rotation(rotationUnits::deg) - encoderPositionShooter) + " " + intToString(Shooter.power(powerUnits::watt)) + " " + intToString(Shooter.rotation(rotationUnits::deg));
        string toque = intToString(gyroValue) + " " + intToString(GyroS.value(rotationUnits::rev)) + " "
            + intToString(GyroI.value(rotationUnits::rev));
        Controller1.Screen.clearLine();
        Controller1.Screen.print(toque.c_str());
       // Brain.Screen.
        //string eff = intToString(Intake.efficiency(percentUnits::pct)) + " " + intToString(Intake.temperature(percentUnits::pct));
        
        //Brain.Screen.print(eff.c_str());
        task::sleep(100);
    }
    return -1;
}
