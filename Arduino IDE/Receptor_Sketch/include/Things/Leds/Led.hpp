#pragma once

#include <Arduino.h>
#include "../Things.hpp"

class Led : public Things
{
protected:
    Led(String name);

private:
    //  This variable allows use blink function without use delay function
    byte pin;

public:
    // Led(String name);
    Led(String name, byte pin);

    //  Accessors
    byte getPin();
    virtual void setPin(byte newPin);

    //  Functions
    virtual void initialize();
    virtual void on();
    virtual void off();
    virtual bool isOn();

    void Action() override;
    void Action(byte args) override;
};