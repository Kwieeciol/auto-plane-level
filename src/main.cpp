#include <Arduino.h>
#include <Wire.h>
#include "gyroscope.h"
#include "receiver.h"
#include "rgb_led.h"

Gyroscope mpu;
Receiver receiver;
RGB_LED rgb_led;

int before = 0;

void setup() {
    Wire.begin();
    Serial.begin(9600);

    rgb_led.setup();

    mpu.setup();
    mpu.calibrate();

    receiver.setup();
}

void loop() {
    /* 
    If the channels are in rest, use the gyroscope (and some arithmetic equation) to level-up the plane.
    */
    delay(100);
}
