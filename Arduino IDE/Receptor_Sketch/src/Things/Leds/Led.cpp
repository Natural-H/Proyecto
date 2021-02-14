#include "../../../include/Things/Leds/Led.hpp"

// Constructors
// Protected Constructor
Led::Led(String name) : Things(name) {}

// Public constructor
Led::Led(String name, byte pin) : Things(name), pin(pin)
{
    this->initialize();
    this->off();
}

byte Led::getPin() { return this->pin; }

void Led::setPin(byte newPin) { this->pin = newPin; }

void Led::initialize() { pinMode(pin, OUTPUT); }

void Led::on() { digitalWrite(this->pin, HIGH); }

void Led::off() { digitalWrite(this->pin, LOW); }

bool Led::isOn()
{
    if (digitalRead(this->pin) == HIGH)
        return true;
    else
        return false;
}

void Led::Action()
{
    if (this->isOn())
        this->off();
    else
        this->on();
}

void Led::Action(byte args)
{
    switch (args)
    {
    case 1:
        this->on();
        break;

    case 2:
        this->off();
        break;
        
    default:
        break;
    }
}