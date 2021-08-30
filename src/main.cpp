#include <Arduino.h>
#include <Wire.h>

#define MPU_ADDR 0x68
#define MAX_ANGLE 30

const int min_val = 265, max_val = 402;
double offset_x = 0, offset_y = 0, offset_z = 0;

struct Angles {double x; double y; double z;};

// prototyping functions
Angles get_raw_angles();
Angles get_angles();
void setup_mpu();
void calibrate_mpu();
double get_temperature();

void setup() {
    Wire.begin();
    Serial.begin(9600);
    setup_mpu();
    calibrate_mpu();
}

void loop() {
    Angles angles = get_angles();
    Serial.println(angles.y);
    delay(100);
}

void setup_mpu() {
    Serial.println("Setting up MPU6050...");
    
    Wire.beginTransmission(MPU_ADDR); // start transmission with MPU
    Wire.write(0x6B); // power management register
    Wire.write(0); // disabling sleep mode (IMPORTANT!)
    Wire.endTransmission(true);

    // Enable to +/- 2g
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x1C); // accelerometer config register
    Wire.write(0); // set sensitivity to +/- 2g
    Wire.endTransmission();

    // Enable +/- 250°/s
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x1B); // gyroscope config register
    Wire.write(0); // smallest sensitivity
    Wire.endTransmission();

    Serial.println("Done setting up.");
}

void calibrate_mpu() {
    Serial.println("Calculating offsets, please stay stil...");
    double av_x = 0.0, av_y = 0.0, av_z = 0.0;
    int total_runs = 100;

    for (int i = 0; i < total_runs; i++) {
        Angles angles = get_raw_angles();
        av_x += angles.x;
        av_y += angles.y;
        av_z += angles.z;

        delay(25); // 25ms delay between readings
    }
    offset_x = av_x / total_runs;
    offset_y = av_y / total_runs;
    offset_z = av_z / total_runs;

    Serial.println("Done calculating offsets.");
}

Angles get_raw_angles() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B); // access register 3B
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14, true);

    double ac_x = Wire.read() << 8 | Wire.read();
    double ac_y = Wire.read() << 8 | Wire.read();
    double ac_z = Wire.read() << 8 | Wire.read(); 

    int x_angle = map(ac_x, min_val, max_val, -90, 90);
    int y_angle = map(ac_y, min_val, max_val, -90, 90);
    int z_angle = map(ac_z, min_val, max_val, -90, 90);

    double x = RAD_TO_DEG * (atan2(-y_angle, -z_angle) + PI);
    double y = RAD_TO_DEG * (atan2(-x_angle, -z_angle) + PI);
    double z = RAD_TO_DEG * (atan2(-y_angle, -x_angle) + PI);

    Angles angles = {abs(x), abs(y), abs(z)};
    return angles;
}

Angles get_angles() {
    Angles raw_angles = get_raw_angles();
    Angles angles = {
        abs(raw_angles.x - offset_x), 
        abs(raw_angles.y - offset_y), 
        abs(raw_angles.z - offset_z)
    };
    return angles;
}

double get_temperature() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x41); // register 41, temperature
    Wire.endTransmission();
    Wire.requestFrom(MPU_ADDR, 2);

    double temp = Wire.read() << 8 | Wire.read();
    return temp / 340 + 36.53;
}
