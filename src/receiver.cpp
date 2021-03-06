#include <Arduino.h>
#include "receiver.h"

void Receiver::setup() {
    this->rest_channel_1 = read_channel(CHANNEL_1);
    this->rest_channel_2 = read_channel(CHANNEL_2);
}

void Receiver::calibrate() {
    setup();
}

int Receiver::read_channel(int pin) {
    int value = pulseIn(pin, HIGH, 25000);
    if (value == 0) { // retry without timeout
        value = pulseIn(pin, HIGH);
    }
    return value;
}

bool Receiver::is_enabled() {
    // switch up (enabled)    -- 900 < value < 1100
    // switch down (disabled) -- 1900 < value < 2000
    int value = read_channel(CHANNEL_5);
    bool enabled;

    if (900 < value && value < 1100) {
        enabled = true;
    } else {
        enabled = false;
    }
    return enabled;
}

bool Receiver::is_at_rest(int channel) {
    int before;
    if (channel == CHANNEL_1) {
        before = rest_channel_1;
    } else {
        before = rest_channel_2;
    }

    int value = read_channel(channel);
    return ((before - REST_RANGE) <= value && value <= (before + REST_RANGE));
}

bool Receiver::should_calibrate() {
    // switch up (disabled)  -- 900 < value < 1100
    // switch down (enabled) -- 1900 < value < 2000
    int value = read_channel(CHANNEL_6);
    bool enabled;

    if (1900 < value && value < 2000) {
        enabled = true;
    } else {
        enabled = false;
    }
    return enabled;
}