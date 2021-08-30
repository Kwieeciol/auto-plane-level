#include <Arduino.h>
#include <Wire.h>
#include "gyroscope.h"
#include "receiver.h"
#include "rgb_led.h"

Gyroscope mpu;
Receiver receiver;
RGB_LED rgb_led;

void setup() {
    Wire.begin();
    Serial.begin(9600);

    mpu.setup();
    mpu.calibrate();
    
    rgb_led.setup();
}

void loop() {
    delay(500);
}
