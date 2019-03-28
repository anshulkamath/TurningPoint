#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"
//
vex::brain Brain;
vex::motor LeftMotor = vex::motor(vex::PORT1, false);
vex::motor RightMotor = vex::motor(vex::PORT10, true);