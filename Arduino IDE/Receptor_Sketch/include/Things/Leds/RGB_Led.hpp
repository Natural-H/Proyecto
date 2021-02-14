#pragma once

#include <Arduino.h>
#include "Led.hpp"

class RGB_Led : public Led
{
private:
    byte pin[3];
    byte pinValues[3];

public:
    RGB_Led(String name, byte pin1, byte pin2, byte pin3);

    void initialize() override;

    void Action(byte args) override;

    // Takes 255 for all pins.
    void on();
    // Takes a value for all pins.
    void on(byte color);
    // Takes custom values for each pin.
    void on(byte first, byte second, byte third);

    void off() override;
    void readValues();

    bool isOn() override;

    /**
     * TODO:
     * Maybe will be necessary finish these functions.
    **/

    // void blink();
    // void blink(int times);
    // void blink(int times, long delay);
};
