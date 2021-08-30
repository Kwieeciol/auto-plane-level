#pragma once

#include <Arduino.h>

#define RED_PIN 11
#define GREEN_PIN 10
#define BLUE_PIN 9


class RGB_LED {
public:
    void setup();
    void set(String color);
};