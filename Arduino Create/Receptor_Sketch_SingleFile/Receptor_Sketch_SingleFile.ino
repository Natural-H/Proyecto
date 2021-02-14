#include <SoftwareSerial.h>
#include <nRF24L01.h>
#include <Vector.h>
#include <string.h>
#include <RF24.h>
#include <SPI.h>

class Things
{
protected:
    String name;

public:
    Things(String name) : name(name) {}
    ~Things() {}

    static Vector<Things *> things;

    String getName() { return this->name; }

    virtual void Action() {}
    virtual void Action(byte Args) {}
};
Vector<Things *> Things::things;

class Led : public Things
{
protected:
    Led(String name) : Things(name) {}

private:
    byte pin;

public:
    Led(String name, byte pin) : Things(name), pin(pin)
    {
        this->initialize();
        this->off();
    }
    ~Led() {}

    byte getPin() { return this->pin; }
    virtual void setPin(byte newPin) { this->pin = newPin; }

    virtual void initialize() { pinMode(this->pin, OUTPUT); }

    virtual void on() { digitalWrite(this->pin, HIGH); }

    virtual void off() { digitalWrite(this->pin, LOW); }

    virtual bool isOn()
    {
        if (digitalRead(this->pin) == HIGH)
            return true;
        else
            return false;
    }

    void Action() override
    {
        if (this->isOn())
            this->off();
        else
            this->on();
    }

    void Action(byte args) override
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
};

class RGB_Led : public Led
{
private:
    byte pin[3];
    byte pinValues[3];

public:
    RGB_Led(String name, byte pin1, byte pin2, byte pin3) : Led(name)
    {
        this->pin[0] = pin1;
        this->pin[1] = pin2;
        this->pin[2] = pin3;
    }
    ~RGB_Led() {}

    void initialize() override
    {
        for (auto pin : this->pin)
            pinMode(pin, OUTPUT);
    }

    void on() override
    {
        for (auto pin : this->pin)
            digitalWrite(pin, HIGH);
    }

    void on(byte color)
    {
        for (auto pin : this->pin)
            analogWrite(pin, color);
    }

    void on(byte first, byte second, byte third)
    {
        analogWrite(first, this->pin[0]);
        analogWrite(second, this->pin[1]);
        analogWrite(third, this->pin[2]);
    }

    void off() override
    {
        for (auto pin : this->pin)
            digitalWrite(pin, LOW);
    }

    void readValues()
    {
        for (auto i = 0; i < 3; i++)
            this->pinValues[i] = analogRead(this->pin[i]) / 4;
    }

    void Action() override
    {
        if (this->isOn())
            this->off();
        else
            this->on();
    }

    void Action(byte args) override
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

            if (this->pinValues[2] - 5 > 0)
                this->pinValues[2] -= 5;

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
};

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
    Button(String name, byte pin, byte thingPin) : Things(name), pin(pin), thingPin(thingPin)
    {
        this->thing = new Led(this->name + "forButton", this->thingPin);
        this->initialize();
    }

    ~Button() {}

    void initialize()
    {
        pinMode(this->pin, INPUT);
        this->updateState();
    }

    void updateState()
    {
        byte newReading = digitalRead(this->pin);

        if (newReading != this->oldReading)
            this->lastDebonuceTime = millis();

        if (millis() - this->lastDebonuceTime > this->debonuceDelay)
            this->state = newReading;

        this->oldReading = newReading;
    }

    void Action() override
    {
        if (this->isPressed())
            this->thing->Action();
    }

    byte getState()
    {
        this->updateState();
        return this->state;
    }

    bool isPressed() { return (getState() == HIGH); }
};

class Function
{
private:
    String nameToDisplay;
    String action;
    String name;
    String default_args;

    int x;
    int y;
    int z;
    int range;

    struct Action_List
    {
        String light = "light";
        String rgbLight = "rgb_light";
    };

    Action_List _action;

    Things *thing;
    Vector<byte> pins;

    Function(String function)
    {
        takeValues(function);

        if (this->action == _action.light)
        {
            this->name = "led" + (String)(Things::things.size() + 1);
            this->thing = new Led(this->name, this->pins[0]);
        }
        else if (this->action == _action.rgbLight)
        {
            this->name = "rgb_led" + (String)(Things::things.size() + 1);
            this->thing = new RGB_Led(this->name, this->pins[0], this->pins[1], this->pins[2]);
        }

        Things::things.push_back(this->thing);
    }
    ~Function() {}

public:
    static Vector<Function> functions;

    static void createFunc(String msg) { functions.push_back(Function(msg)); }

    String getName() { return this->name; }
    String getNameToDisplay() { return this->nameToDisplay; }
    String getDefaultArgs() { return this->default_args; }

    int getX() { return this->x; }
    int getY() { return this->y; }
    int getZ() { return this->z; }
    int getR() { return this->range; }

    void setFunction(String function) { this->takeValues(function); }

    void Execute()
    {
        if (this->default_args)
            this->thing->Action(this->default_args.toInt());
        else
            this->thing->Action();
    }
    
    void Execute()
    {
        if (this->default_args)
            this->thing->Action(this->default_args.toInt());
        else
            this->thing->Action();
    }

    void Execute(int args) { this->thing->Action(args); }

    void Execute(String args) { this->thing->Action(args.toInt()); }

    void takeValues(String message)
    {
        bool n_found = false; //  Name
        bool x_found = false; //  X value
        bool y_found = false; //  Y value
        bool z_found = false; //  Z value
        bool r_found = false; //  Range value
        bool f_found = false; //  Action value
        bool a_found = false; //  Arguments value
        bool p_found = false; //  Pins value

        bool n_exists = false;
        bool x_exists = false;
        bool y_exists = false;
        bool z_exists = false;
        bool r_exists = false;
        bool f_exists = false;
        bool a_exists = false;
        bool p_exists = false;

        String x;
        String y;
        String z;
        String range;
        String pin;

        for (auto &&_char : message)
        {
            if (_char == 'N' && !n_found)
            {
                n_found = true;
                continue;
            }

            if (n_found && !n_exists)
            {
                if (_char == ' ')
                {
                    n_exists = true;
                    continue;
                }

                if (_char >= '0' && _char <= 'z' && _char != ':')
                {
                    this->nameToDisplay += _char;
                    continue;
                }
            }

            if (_char == 'X' && !x_found)
            {
                x_found = true;
                continue;
            }

            if (x_found && !x_exists)
            {
                if (_char == ' ')
                {
                    x_exists = true;
                    continue;
                }

                if (_char >= '0' && _char <= '9')
                {
                    x += _char;
                    continue;
                }
            }

            if (_char == 'Y' && !y_found)
            {
                y_found = true;
                continue;
            }

            if (y_found && !y_exists)
            {
                if (_char == ' ')
                {
                    y_exists = true;
                    continue;
                }

                if (_char >= '0' && _char <= '9')
                {
                    y += _char;
                    continue;
                }
            }

            if (_char == 'Z' && !z_found)
            {
                z_found = true;
                continue;
            }

            if (z_found && !z_exists)
            {
                if (_char == ' ')
                {
                    z_exists = true;
                    continue;
                }

                if (_char >= '0' && _char <= '9')
                {
                    z += _char;
                    continue;
                }
            }

            if (_char == 'R' && !r_found)
            {
                r_found = true;
                continue;
            }

            if (r_found && !r_exists)
            {
                if (_char == ' ')
                {
                    r_exists = true;
                    continue;
                }

                if (_char >= '0' && _char <= '9')
                {
                    range += _char;
                    continue;
                }
            }

            if (_char == 'F' && !f_found)
            {
                f_found = true;
                continue;
            }

            if (f_found && !f_exists)
            {
                if (_char == ' ')
                {
                    f_exists = true;
                    continue;
                }

                if (_char >= 'A' && _char <= 'z')
                {
                    this->action += _char;
                    continue;
                }
            }

            if (_char == 'A' && !a_found)
            {
                a_found = true;
                continue;
            }

            if (a_found && !a_exists)
            {
                if (_char == ' ')
                {
                    a_exists = true;
                    continue;
                }

                if (_char >= '0' && _char <= 'z' && _char != ':')
                {
                    this->default_args += _char;
                    continue;
                }
            }

            if (_char == 'P' && !p_found)
            {
                p_found = true;
                continue;
            }

            if (p_found && !p_exists)
            {
                if (_char == ' ')
                {
                    p_exists = true;
                    continue;
                }

                if (_char == ',')
                {
                    this->pins.push_back(pin.toInt());
                    pin = "";
                    continue;
                }

                if (_char >= '0' && _char <= '9')
                {
                    pin += _char;
                    continue;
                }
            }
        }

        if (!this->range)
            this->range = 15;

        this->x = x.toInt();
        this->y = y.toInt();
        this->z = z.toInt();

        if (range)
            this->range = range.toInt();
    }
};
Vector<Function> Function::functions;

class Listener
{
private:
    byte *bPins;
    byte *rfPins;

    SoftwareSerial *bluetooth;
    RF24 *radio;

    struct data
    {
        int x;
        int y;
        int z;
    };

public:
    data take_data;

    Listener()
    {
        this->bPins = new byte[2]{1, 2};
        this->rfPins = new byte[2]{10, 9};

        this->bluetooth = nullptr;
        this->bluetooth = new SoftwareSerial(this->bPins[0], this->bPins[1]);

        this->radio = nullptr;
        this->radio = new RF24(this->rfPins[0], this->rfPins[1]);
    }
    ~Listener() {}

    void Initialize()
    {
        const byte address[6] = "00001";

        Serial.begin(9600);
        this->bluetooth->begin(9600);

        this->radio->begin();
        this->radio->openReadingPipe(0, address);
        this->radio->setPALevel(RF24_PA_MIN);
        this->radio->setDataRate(RF24_250KBPS);
        this->radio->startListening();
    }

    bool bluetoothIsAviable() { return this->bluetooth->available(); }
    bool RF24IsAviable() { return this->radio->available(); }
    String bluetooth_Read() { return this->bluetooth->readString(); }
    void rf24_Read() { this->radio->read(&this->take_data, sizeof(data)); }

    void interpret_Message(String message)
    {
        if (message.startsWith("0"))
        {
            String name;
            String args;

            bool n_found = false;
            bool a_found = false;

            bool n_exists = false;
            bool a_exists = false;

            for (auto &&_char : message)
            {
                if (_char == 'N' && !n_found)
                {
                    n_found = true;
                    continue;
                }

                if (n_found && !n_exists)
                {
                    if (_char == ' ')
                    {
                        n_exists = true;
                        continue;
                    }

                    if (_char >= '0' && _char <= 'z' && _char != ':')
                        name += _char;
                }

                if (_char == 'A' && !a_found)
                {
                    a_found = true;
                    continue;
                }

                if (a_found && !a_exists)
                {
                    if (_char == ' ')
                    {
                        a_exists = true;
                        continue;
                    }

                    if (_char >= '0' && _char <= 'z' && _char != ':')
                        args += _char;
                }
            }

            if (args)
            {
                for (auto &&func : Function::functions)
                {
                    if (func.getNameToDisplay() == name)
                    {
                        if (args)
                            func.Execute(args.toInt());
                        else
                            func.Execute();
                    }
                }
            }
            else
            {
                for (auto &&func : Function::functions)
                {
                    if (func.getName() == name)
                        func.Execute();
                }
            }

            for (auto &&func : Function::functions)
            {
                if (func.getName() == name)
                {
                    func.Execute();
                }
            }
        }
        //  If the first char is 1, this will create a new Function
        else if (message.startsWith("1"))
            Function::createFunc(message);
    }
};

Listener listener;
Led verifyLed("Verify Led", 2);

void setup()
{
    listener.Initialize();

    Function::createFunc("N:myLed X:300 Y:450 Z:500 R:30 F:light A:0 P:4");
    Function::createFunc("N:myRGBLed X:200 Y:200 Z:500 R:35 F:rgb_light P:6,5,3");
}

void loop()
{
    verifyLed.on();

    if (listener.RF24IsAviable())
    {
        listener.rf24_Read();

        for (auto &&function : Function::functions)
        {
            if ((listener.take_data.x > function.getX() - function.getR() && listener.take_data.x < function.getX() + function.getR()) &&
                (listener.take_data.y > function.getY() - function.getR() && listener.take_data.y < function.getY() + function.getR()) &&
                (listener.take_data.z > function.getZ() - function.getR() && listener.take_data.z < function.getZ() + function.getR()))
                function.Execute();
        }
    }

    if (listener.bluetoothIsAviable())
    {
        String message = listener.bluetooth_Read();
        //  test
        message = "0: N:myLed";

        listener.interpret_Message(message);
    }
}