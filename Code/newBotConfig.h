vex::brain Brain;
vex::motor BackLeft = vex::motor(vex::PORT13, vex::gearSetting::ratio18_1);
vex::motor FrontLeft = vex::motor(vex::PORT11, vex::gearSetting::ratio18_1);
vex::motor BackRight = vex::motor(vex::PORT12, vex::gearSetting::ratio18_1, true);
vex::motor FrontRight = vex::motor(vex::PORT14, vex::gearSetting::ratio18_1, true);
vex::motor Intake = vex::motor(vex::PORT15, vex::gearSetting::ratio36_1);
vex::motor Shooter = vex::motor(vex::PORT16, vex::gearSetting::ratio6_1, true);

vex::controller Controller1 = vex::controller();
