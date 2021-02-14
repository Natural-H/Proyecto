#include "../include/Listener.hpp"

Listener::Listener()
{
    this->bPins = new byte[2]{1, 2};
    this->rfPins = new byte[2]{10, 9};

    this->bluetooth = nullptr;
    this->bluetooth = new SoftwareSerial(this->bPins[0], this->bPins[1]);

    this->radio = nullptr;
    this->radio = new RF24(this->rfPins[0], this->rfPins[1]);
}

void Listener::Initialize()
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

bool Listener::bluetoothIsAviable() { return this->bluetooth->available(); }

bool Listener::RF24IsAviable() { return this->radio->available(); }

String Listener::bluetooth_Read() { return this->bluetooth->readString(); }

void Listener::interpret_Message(String message)
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
                {
                    name += _char;
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
                    args += _char;
                    continue;
                }
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
                if (func.getNameToDisplay() == name)
                    func.Execute();
            }
        }

        for (auto &&func : Function::functions)
        {
            if (func.getNameToDisplay() == name)
                func.Execute();
        }
    }
    //  If the first char is 1, this will create a new Function
    else if (message.startsWith("1"))
        Function::createFunc(message);
}

void Listener::rf24_Read() { this->radio->read(&this->take_data, sizeof(data)); }