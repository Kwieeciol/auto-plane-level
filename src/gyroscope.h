#pragma once

#include <Arduino.h>

#define MPU_ADDR 0x68
#define MIN_VAL 265
#define MAX_VAL 402

struct Angles {double x; double y; double z;};

class Gyroscope {
private:
    int offset_x, offset_y, offset_z;
public:
    Gyroscope(); // constructor
    void setup();
    void calibrate();
    double get_temperature();
    Angles get_angles();
    Angles get_raw_angles();
};
