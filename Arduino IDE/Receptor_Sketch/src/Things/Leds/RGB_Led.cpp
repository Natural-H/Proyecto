#include "../../../include/Things/Leds/RGB_Led.hpp"

RGB_Led::RGB_Led(String name, byte pin1, byte pin2, byte pin3) : Led(name)
{
    this->pin[0] = pin1;
    this->pin[1] = pin2;
    this->pin[2] = pin3;

    this->initialize();
    this->off();
}

void RGB_Led::initialize()
{
    for (auto &&pin : this->pin)
        pinMode(pin, OUTPUT);
}

void RGB_Led::Action(byte args)
{
    switch (args)
    {
    case 0:
        if (this->isOn())
            this->off();
        else
            this->on();
        break;

    case 1:
        this->on();
        break;

    case 2:
        this->off();
        break;

    case 3:
        this->readValues();

        if (this->pinValues[0] + 5 < 256)
            this->pinValues[0] += 5;

        this->on(this->pinValues[0], this->pinValues[1], this->pinValues[2]);
        break;

    case 4:
        this->readValues();

        if (this->pinValues[1] + 5 < 256)
            this->pinValues[1] += 5;

        this->on(this->pinValues[0], this->pinValues[1], this->pinValues[2]);
        break;

    case 5:
        this->readValues();

        if (this->pinValues[2] + 5 < 256)
            this->pinValues[2] += 5;

        this->on(this->pinValues[0], this->pinValues[1], this->pinValues[2]);
        break;

    case 6:
        this->readValues();

        if (this->pinValues[0] - 5 > 0)
            this->pinValues[0] -= 5;

        this->on(this->pinValues[0], this->pinValues[1], this->pinValues[2]);
        break;

    case 7:
        this->readValues();

        if (this->pinValues[1] - 5 > 0)
            this->pinValues[1] -= 5;

        this->on(this->pinValues[0], this->pinValues[1], this->pinValues[2]);
        break;

    case 8:
        this->readValues();

        if (this->pinValues[2] - 5 > 0)
            this->pinValues[2] -= 5;

        this->on(this->pinValues[0], this->pinValues[1], this->pinValues[2]);
        break;
    default:
        break;
    }
}

bool RGB_Led::isOn()
{
    for (auto pin : this->pin)
    {
        if (analogRead(pin) > 0)
            return true;
    }

    return false;
}

void RGB_Led::on()
{
    for (auto pin : this->pin)
        digitalWrite(pin, HIGH);
}

void RGB_Led::on(byte color)
{
    for (auto pin : this->pin)
        analogWrite(pin, color);
}

void RGB_Led::on(byte first, byte second, byte third)
{
    analogWrite(this->pin[0], first);
    analogWrite(this->pin[1], second);
    analogWrite(this->pin[2], third);
}

void RGB_Led::off()
{
    for (auto pin : this->pin)
        digitalWrite(pin, LOW);
}

void RGB_Led::readValues()
{
    for (auto i = 0; i < 3; i++)
        this->pinValues[i] = analogRead(this->pin[i]) / 4;
}
