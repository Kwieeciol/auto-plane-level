#pragma once

#include <Arduino.h>

#define CHANNEL_1 A3
#define CHANNEL_2 A2
#define CHANNEL_5 A1
#define CHANNEL_6 A0
#define REST_RANGE 30

class Receiver {
private:
    int rest_channel_1, rest_channel_2;
public:
    void setup();
    void calibrate();
    bool should_calibrate();
    bool is_enabled();
    bool is_at_rest(int channel);
    int read_channel(int pin);
};
