#include "../../../include/Things/Interruptors/Button.hpp"

Button::Button(String name, byte pin, byte thingPin) : Things(name), pin(pin), thingPin(thingPin)
{
    this->initialize();

    this->thing = new Led(this->name + (String)Things::things.size(), this->thingPin);
}

Button::~Button()
{
}

void Button::initialize()
{
    pinMode(this->pin, INPUT);
    this->updateState();
}

void Button::updateState()
{
    byte newReading = digitalRead(this->pin);

    if (newReading != this->oldReading)
        this->lastDebonuceTime = millis();

    if (millis() - this->lastDebonuceTime > this->debonuceDelay)
        this->state = newReading;

    this->oldReading = newReading;
}

byte Button::getState()
{
    this->updateState();
    return this->state;
}

bool Button::isPressed()
{
    return (getState() == HIGH);
}

void Button::Action()
{
    if (this->isPressed())
        this->thing->Action();
}