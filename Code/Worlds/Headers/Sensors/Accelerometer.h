#include <vector>
#include <iostream>

using namespace std;

class Accelerometer
{
    accelerometer accelX, accelY, accelZ;
    vector<double> accelXData, accelYData, accelZData;
  public:
    
    int accelFilterThread();
    
    Accelerometer(accelerometer accelX1, accelerometer accelY1, accelerometer accelZ1)
    {
      accelX = accelX1;
      accelY = accelY1;
      accelZ = accelZ1;
    }
    
    void getXAccel();
    void getYAccel();
    void getZAccel();
};
