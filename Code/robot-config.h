vex::brain Brain;

vex::motor BackRight(vex::PORT8, vex::gearSetting::ratio18_1, true);
vex::motor FrontRight(vex::PORT9, vex::gearSetting::ratio18_1, true);
vex::motor BackLeft(vex::PORT7, vex::gearSetting::ratio18_1, false);
vex::motor FrontLeft(vex::PORT10, vex::gearSetting::ratio18_1, false);
vex::motor Shooter(vex::PORT6, vex::gearSetting::ratio18_1, true);
vex::motor Intake(vex::PORT5, vex::gearSetting::ratio18_1, true);
vex::motor Lift(vex::PORT4, vex::gearSetting::ratio18_1, true);
vex::motor SpinnyThingy(vex::PORT3, vex::gearSetting::ratio18_1, false);

vex::limit Limit1(Brain.ThreeWirePort.H);

vex::limit Limit2(Brain.ThreeWirePort.Port[6]);

vex::gyro GyroS(Brain.ThreeWirePort.F);
vex::gyro GyroI(Brain.ThreeWirePort.E);

vex::controller Controller1;
vex::vision::signature GREENFLAG (1, -2719, -2015, -2367, -5071, -4169, -4620, 3.799999952316284, 0);
vex::vision::signature REDFLAG (2, 8117, 9023, 8570, -831, -431, -631, 5.9, 0);
vex::vision::signature BLUEFLAG (3, -4337, -3475, -3906, 11999, 13213, 12606, 6.3, 0);
vex::vision::signature SIG_4 (4, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_5 (5, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3, 0);
//vex::vision::signature GREENFLAG (1, -1411, -153, -782, -6471, -5621, -6046, 5.5, 0);
//vex::vision::signature REDFLAG (2, 1169, 9191, 5180, -1617, -621, -1120, 1.100000023841858, 0);

vex::vision Vision (vex::PORT16, 61, GREENFLAG, REDFLAG, BLUEFLAG, SIG_4, SIG_5, SIG_6, SIG_7);
