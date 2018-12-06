vex::brain Brain;

vex::motor BackRight(vex::PORT8, vex::gearSetting::ratio18_1, true);
vex::motor FrontRight(vex::PORT9, vex::gearSetting::ratio18_1, true);
vex::motor BackLeft(vex::PORT7, vex::gearSetting::ratio18_1, false);
vex::motor FrontLeft(vex::PORT10, vex::gearSetting::ratio18_1, false);
vex::motor Shooter(vex::PORT6, vex::gearSetting::ratio18_1, true);
vex::motor Intake(vex::PORT5, vex::gearSetting::ratio18_1, true);
vex::motor Lift(vex::PORT4, vex::gearSetting::ratio18_1, false);
vex::motor SpinnyThingy(vex::PORT3, vex::gearSetting::ratio18_1, false);

vex::limit Limit1(Brain.ThreeWirePort.H);

vex::limit Limit2(Brain.ThreeWirePort.Port[6]);

vex::gyro GyroS(Brain.ThreeWirePort.F);
vex::gyro GyroI(Brain.ThreeWirePort.E);
/*
vex::vision::signature GREENFLAG (1, -3475, -3251, -3362, -4955, -4645, -4800, 8, 0);
vex::vision::signature REDFLAG (2, 8365, 8871, 8618, -713, -311, -512, 10.300000190734863, 0);
vex::vision::signature BLUEFLAG (3, -3607, -3203, -3404, 11667, 12619, 12142, 6.900000095367432, 0);
*/vex::controller Controller1;
vex::vision::signature GREENFLAG (1, 0, 0, 0, 0, 0, 0, 10.5, 0);
vex::vision::signature REDFLAG (2, 8365, 8871, 8618, -713, -311, -512, 10.300000190734863, 0);
vex::vision::signature BLUEFLAG (3, -2521, -1431, -1976, 2615, 6087, 4350, 2, 0);
vex::vision::signature SIG_4 (4, -1393, -1009, -1201, -2859, -2521, -2690, 2.4, 0);
vex::vision::signature SIG_5 (5, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3, 0);
//vex::vision::signature GREENFLAG (1, -1411, -153, -782, -6471, -5621, -6046, 5.5, 0);
//vex::vision::signature REDFLAG (2, 1169, 9191, 5180, -1617, -621, -1120, 1.100000023841858, 0);

vex::vision Vision (vex::PORT16, 74, GREENFLAG, REDFLAG, BLUEFLAG, SIG_4, SIG_5, SIG_6, SIG_7);
