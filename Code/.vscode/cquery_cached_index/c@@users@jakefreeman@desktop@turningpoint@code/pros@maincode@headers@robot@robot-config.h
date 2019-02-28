vex::brain Brain;

// Drive motors
vex::motor frontRight = vex::motor(vex::PORT1, vex::gearSetting::ratio18_1, false);
vex::motor backRight = vex::motor(vex::PORT2, vex::gearSetting::ratio18_1, false);
vex::motor frontLeft = vex::motor(vex::PORT3, vex::gearSetting::ratio18_1, true);
vex::motor backLeft = vex::motor(vex::PORT4, vex::gearSetting::ratio18_1, true);

// Auxiliary
vex::motor Puncher = vex::motor(vex::PORT5, vex::gearSetting::ratio18_1, true);
vex::motor Intake = vex::motor(vex::PORT6, vex::gearSetting::ratio18_1, true);
vex::motor Index = vex::motor(vex::PORT7, vex::gearSetting::ratio18_1, true);
vex::motor Descore = vex::motor(vex::PORT8, vex::gearSetting::ratio18_1, true);

// Sensors
vex::gyro gyroscope = vex::gyro(Brain.ThreeWirePort.A);
vex::gyro invertedGyro = vex::gyro(Brain.ThreeWirePort.B);
vex::accelerometer accelo = vex::accelerometer(Brain.ThreeWirePort.C);
