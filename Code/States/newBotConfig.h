vex::brain Brain;
vex::controller Controller1 = vex::controller();
vex::competition Competition = vex::competition();

// Drive Motors
vex::motor BackLeft = vex::motor(vex::PORT2, vex::gearSetting::ratio18_1);
vex::motor FrontLeft = vex::motor(vex::PORT7, vex::gearSetting::ratio18_1);
vex::motor BackRight = vex::motor(vex::PORT1, vex::gearSetting::ratio18_1, true);
vex::motor FrontRight = vex::motor(vex::PORT8, vex::gearSetting::ratio18_1, true);

// Accessory Motors
vex::motor Intake = vex::motor(vex::PORT6, vex::gearSetting::ratio6_1);
vex::motor Shooter = vex::motor(vex::PORT9, vex::gearSetting::ratio36_1, true);
vex::motor ShooterAux = vex::motor(vex::PORT5, vex::gearSetting::ratio36_1, false);
vex::motor Scraper = vex::motor(vex::PORT4, vex::gearSetting::ratio36_1, true);

// Sensors
vex::bumper Limit1 = vex::bumper(Brain.ThreeWirePort.A);
vex::gyro gyroscope = vex::gyro(Brain.ThreeWirePort.H);
vex::gyro invertedGyro = vex::gyro(Brain.ThreeWirePort.B);
