vex::brain Brain;
vex::controller Controller;
vex::competition Competition;

// Drive Motors
vex::motor FrontRight = vex::motor(vex::PORT1, vex::gearSetting::ratio18_1, true);
vex::motor BackRight = vex::motor(vex::PORT10, vex::gearSetting::ratio18_1, true);
vex::motor FrontLeft = vex::motor(vex::PORT15, vex::gearSetting::ratio18_1, false);
vex::motor BackLeft = vex::motor(vex::PORT16, vex::gearSetting::ratio18_1, false);

// Auxiliary Motors
vex::motor Intake = vex::motor(vex::PORT2, vex::gearSetting::ratio18_1, true);
vex::motor Angler = vex::motor(vex::PORT3, vex::gearSetting::ratio36_1, true);
vex::motor Puncher = vex::motor(vex::PORT20, vex::gearSetting::ratio36_1, false);
vex::motor Scraper = vex::motor(vex::PORT9, vex::gearSetting::ratio36_1, false);

// Sensors
vex::pot Poten = vex::pot(Brain.ThreeWirePort.A);
vex::line puncherLine = vex::line(Brain.ThreeWirePort.B);
vex::line intakeLine = vex::line(Brain.ThreeWirePort.C);
