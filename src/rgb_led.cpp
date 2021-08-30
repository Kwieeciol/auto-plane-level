#include <Arduino.h>
#include "rgb_led.h"

void RGB_LED::setup() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

void RGB_LED::set(String color) {
    int r, g, b;

    if (color == "white") {
        r = 255;
        g = 255;
        b = 255;
    } else if (color == "red") {
        r = 255;
        g = 0;
        b = 0;
    } else if (color == "green") {
        r = 0;
        g = 255;
        b = 0;
    } else if (color == "blue") {
        r = 0;
        g = 0;
        b = 255;
    } else if (color == "yellow") {
        r = 255;
        g = 255;
        b = 0;
    } else {
        r = 0;
        g = 0;
        b = 0;
    }

    // reversing, LED's that share a common anode have
    // the values inverted, 0 -- ON, 255 -- OFF
    r = map(r, 0, 255, 255, 0);
    g = map(g, 0, 255, 255, 0);
    b = map(b, 0, 255, 255, 0);

    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);
}