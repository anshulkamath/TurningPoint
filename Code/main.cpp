#include "robot-config.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <vector>
#include <thread>
#include <fstream>
using namespace std;

void runVision()
{
    vector<vex::vision::object> greenFlags, redFlags, blueFlags;
    Vision.takeSnapshot(1);
    for(int i = 0; i<Vision.objectCount; i++)
    {
        if(Vision.objects[i].id == 1)
        {
            greenFlags.push_back(Vision.objects[i]);
        }
    }
    Vision.takeSnapshot(2);
    for(int i = 0; i<Vision.objectCount; i++)
    {
        if(Vision.objects[i].id == 2)
        {
            blueFlags.push_back(Vision.objects[i]);
        }
    }
    Vision.takeSnapshot(3);
    for(int i = 0; i<Vision.objectCount; i++)
    {
        if(Vision.objects[i].id == 3)
        {
            redFlags.push_back(Vision.objects[i]);
        }
    } 
    
    for(int i = 0; i<redFlags.size(); i++)
    {
        for(int s = 0; s<greenFlags.size(); s++)
        {
            if(abs(redFlags[i].originX + redFlags[i].width - greenFlags[s].originX) < 10)
            {
                if(abs(redFlags[i].originY - greenFlags[s].originY) < 10)
                {
                    // Red Flag Found
                }
            }
        }
    }
    for(int i = 0; i<blueFlags.size(); i++)
    {
        for(int s = 0; s<greenFlags.size(); s++)
        {
            if(abs(blueFlags[i].originX - (greenFlags[s].originX+greenFlags[s].width)) < 10)
            {
                if(abs(blueFlags[i].originY - greenFlags[s].originY) < 10)
                {
                    // Blue Flag Found
                }
            }
        }
    }    
}

void targetFlag(int offset)
{
    
}

string intToString(double val)
{
    ostringstream ss;
    ss<<val;
    return ss.str();
}
bool inUse = false;
bool inTakeInUse = false;
int taskShooter()
{
    while(true)
    {
        if(Controller1.ButtonR1.pressing())
        {
            inUse = true;
            Shooter.spin(directionType::fwd, 100, velocityUnits::pct);
            while(!Limit1.pressing())
            {
           
            }
            Shooter.stop();
            Shooter.spin(directionType::fwd, 100, velocityUnits::pct);
            while(Limit1.pressing())
            {
                
            }
            Shooter.stop();
            Shooter.rotateFor(1500,rotationUnits::deg,100,velocityUnits::pct); 
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

int runY()
{
            Shooter.spin(directionType::fwd, 100, velocityUnits::pct);
            while(!Limit1.pressing())
            {
           
            }
            Shooter.stop();
            Shooter.spin(directionType::fwd, 100, velocityUnits::pct);
            while(Limit1.pressing())
            {
                
            }
            Shooter.stop();
            task::sleep(100);
            Shooter.resetRotation();
            Shooter.rotateTo(1200, rotationUnits::deg, 100, velocityUnits::pct);
            return 0;
}

int main() {
    int encoderPositionShooter = Shooter.rotation(rotationUnits::deg);
    //task shooterTask(taskShooter);
   vex::task(taskShooter, 1);
    
    double firstHundred = 0;
    double secondHundred = 0;
    double thirdHundred = 0;
    double fourthHundred = 0;

    while(true)
    {  
        int frontLeftValue = 0, frontRightValue = 0, backLeftValue = 0, backRightValue = 0;
        if(abs(Controller1.Axis3.value()) > 20)
        {
            frontLeftValue = Controller1.Axis3.value();
            backLeftValue = Controller1.Axis3.value();           
        }
           
        if(abs(Controller1.Axis2.value()) > 20)
        {
            frontRightValue = Controller1.Axis2.value();
            backRightValue = frontRightValue;
        }
        
        /*if(abs(Controller1.Axis4.value()) > 50 && Controller1.Axis3.value() < 20)
        {
            frontLeftValue = Controller1.Axis4.value();
            backLeftValue = -frontLeftValue;
            frontRightValue = -Controller1.Axis4.value();
            backRightValue = Controller1.Axis4.value();            
        }*/
 
        
        
        if(Controller1.ButtonR2.pressing())
        {
            Shooter.spin(directionType::fwd, 60, velocityUnits::pct);
        }else if(!inUse)
        {
            Shooter.stop();
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

        // 1924.8 1788 2008.4
        FrontLeft.spin(directionType::fwd, frontLeftValue, velocityUnits::pct);
        FrontRight.spin(directionType::fwd, frontRightValue, velocityUnits::pct);       
        BackRight.spin(directionType::fwd, backRightValue, velocityUnits::pct);
        BackLeft.spin(directionType::fwd,  backLeftValue, velocityUnits::pct);     
        string val = "Value: " + intToString(Shooter.rotation(rotationUnits::deg) - encoderPositionShooter) + " " + intToString(Shooter.power(powerUnits::watt)) + " " + intToString(Shooter.rotation(rotationUnits::deg));
        string toque = intToString(Shooter.torque(torqueUnits::Nm)) + " Rotation: " + intToString(Shooter.rotation(rotationUnits::deg));
        Brain.Screen.clearLine();
        Brain.Screen.print(toque.c_str());
       // Brain.Screen.
        //string eff = intToString(Intake.efficiency(percentUnits::pct)) + " " + intToString(Intake.temperature(percentUnits::pct));
        
        //Brain.Screen.print(eff.c_str());
        task::sleep(100);
    }
}
