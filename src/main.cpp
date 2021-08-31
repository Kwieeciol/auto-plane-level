#include <Arduino.h>
#include <Wire.h>
#include "gyroscope.h"
#include "receiver.h"
#include "rgb_led.h"

#define MAX_ANGLE 15

Gyroscope mpu;
Receiver receiver;
RGB_LED rgb_led;

int before = 0;

void setup() {
    Wire.begin();
    Serial.begin(9600);

    rgb_led.setup();
    rgb_led.set("yellow");

    mpu.setup();
    mpu.calibrate();

    receiver.setup();
}

void loop() {
    /* 
    If the channels are in rest (not moving), use the 
    gyroscope (and some arithmetic equation) to level-up the plane.
    */
    if (receiver.should_calibrate()) {
        // calibrate MPU and RC pilot sticks
        rgb_led.set("yellow");
        mpu.fast_calibrate();
        receiver.calibrate();
    }

    if (receiver.is_enabled()) {
        Angles angles = mpu.get_angles();

        // checking if the plane is not too inclided (kinda off-topic to the project tbh)
        if (angles.x > MAX_ANGLE && angles.x < 360 - MAX_ANGLE) {
            rgb_led.set("red");
        } else if (angles.y > MAX_ANGLE && angles.y < 360 - MAX_ANGLE) {
            rgb_led.set("red");
        } else {
            rgb_led.set("green");
        }

        // check if the RC pilot is at rest
        // if it is -- level the plane
    } else {
        // redirect the input as-is
        rgb_led.set("blue");
    }
    delay(10);
}
