#include <Arduino.h>
#include <Wire.h>
#include "gyroscope.h"

Gyroscope mpu;

void setup() {
    Wire.begin();
    Serial.begin(9600);

    mpu.setup();
    mpu.calibrate();
}

void loop() {
    Angles angles = mpu.get_angles();
    Serial.println(angles.y);
    delay(100);
}
