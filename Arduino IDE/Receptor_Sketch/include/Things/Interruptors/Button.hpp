#pragma once

#include <Arduino.h>
#include "../Things.hpp"
#include "../Leds/Led.hpp"

class Button : public Things
{
private:
    Things *thing;

    byte pin;
    byte thingPin;
    byte state;
    byte oldReading;

    unsigned long lastDebonuceTime;
    unsigned long debonuceDelay;

public:
    Button(String name, byte pin, byte thingPin);
    ~Button();

    void initialize();
    void updateState();

    void Action() override;

    byte getState();
    bool isPressed();
};