#include <Arduino.h>
#include <Wire.h>
#include "gyroscope.h"

Gyroscope::Gyroscope() {
    this->offset_x = 0.0;
    this->offset_y = 0.0;
    this->offset_z = 0.0;
}

void Gyroscope::setup() {
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

void Gyroscope::calibrate() {
    Serial.println("Calculating offsets, please stay still...");

    double av_x = 0.0, av_y = 0.0, av_z = 0.0;
    int total_runs = 100; // lowered for testing (should be 150+)

    for (int i = 0; i < total_runs; i++) {
        Angles angles = get_raw_angles();

        av_x += angles.x;
        av_y += angles.y;
        av_z += angles.z;

        delay(25); // 25ms delay between readings
    }

    this->offset_x = av_x / total_runs;
    this->offset_y = av_y / total_runs;
    this->offset_z = av_z / total_runs;

    Serial.println("Done calculating offsets.");
}

Angles Gyroscope::get_raw_angles() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B); // access register 3B
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14, true);

    double ac_x = Wire.read() << 8 | Wire.read();
    double ac_y = Wire.read() << 8 | Wire.read();
    double ac_z = Wire.read() << 8 | Wire.read(); 

    int x_angle = map(ac_x, MIN_VAL, MAX_VAL, -90, 90);
    int y_angle = map(ac_y, MIN_VAL, MAX_VAL, -90, 90);
    int z_angle = map(ac_z, MIN_VAL, MAX_VAL, -90, 90);

    double x = RAD_TO_DEG * (atan2(-y_angle, -z_angle) + PI);
    double y = RAD_TO_DEG * (atan2(-x_angle, -z_angle) + PI);
    double z = RAD_TO_DEG * (atan2(-y_angle, -x_angle) + PI);

    Angles angles = {abs(x), abs(y), abs(z)};
    return angles;
}

Angles Gyroscope::get_angles() {
    Angles raw_angles = get_raw_angles();
    Angles angles = {
        abs(raw_angles.x - offset_x), 
        abs(raw_angles.y - offset_y), 
        abs(raw_angles.z - offset_z)
    };
    return angles;
}

double Gyroscope::get_temperature() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x41); // register 41, temperature
    Wire.endTransmission();
    Wire.requestFrom(MPU_ADDR, 2);

    double temp = Wire.read() << 8 | Wire.read();
    return temp / 340 + 36.53;
}