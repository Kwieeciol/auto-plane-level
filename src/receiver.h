#pragma once

#include <Arduino.h>

#define CHANNEL_1 A7
#define CHANNEL_2 A6
#define CHANNEL_3 A3
#define CHANNEL_4 A2
#define CHANNEL_5 A1
#define CHANNEL_6 A0

class Receiver {
public:
    bool should_calibrate();
    bool is_enabled();
    int read_channel(int pin);
};
