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
vex::vision::signature GREENFLAG (1, -3117, -2419, -2768, -5135, -4253, -4694, 5.8, 0);
vex::vision::signature REDFLAG (2, 8415, 8889, 8652, -911, -697, -804, 5.900000095367432, 0);
vex::vision::signature BLUEFLAG (3, -3709, -2777, -3243, 8191, 11565, 9878, 4.5, 0);*/
vex::controller Controller1;
vex::vision::signature GREENFLAG (1, -2919, -2505, -2712, -5069, -4731, -4900, 11, 0);
vex::vision::signature REDFLAG (2, 7281, 7735, 7508, -1115, -679, -896, 5, 0);
vex::vision::signature BLUEFLAG (3, -3745, -3319, -3532, 10485, 11833, 11159, 7.3, 0);
vex::vision::signature SIG_4 (4, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_5 (5, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3, 0);
//vex::vision::signature GREENFLAG (1, -1411, -153, -782, -6471, -5621, -6046, 5.5, 0);
//vex::vision::signature REDFLAG (2, 1169, 9191, 5180, -1617, -621, -1120, 1.100000023841858, 0);

vex::vision Vision (vex::PORT16, 61, GREENFLAG, REDFLAG, BLUEFLAG, SIG_4, SIG_5, SIG_6, SIG_7);
