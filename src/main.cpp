#include <Arduino.h>
#include <Wire.h>
#include "gyroscope.h"
#include "receiver.h"

Gyroscope mpu;
Receiver receiver;

void setup() {
    Wire.begin();
    Serial.begin(9600);

    mpu.setup();
    mpu.calibrate();
}

void loop() {
    // Angles angles = mpu.get_angles();
    // Serial.println(angles.y);
    Serial.println(receiver.should_calibrate());
    delay(100);
}
