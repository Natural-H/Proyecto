#pragma once

#include <Arduino.h>
#include "Functions.hpp"

//  Networking stuff
//  Bluetooth
#include <SoftwareSerial.h>

//  RF24
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/**
 * This class is for listening control.
 * TODO:
**/

class Listener
{
private:
    // Default Pins for Serial
    byte *bPins;
    byte *rfPins;

    SoftwareSerial *bluetooth;

    // Radio stuff
    RF24 *radio;

    struct data
    {
        int x;
        int y;
        int z;
    };

public:
    Listener();

    data take_data;

    bool bluetoothIsAviable();
    bool RF24IsAviable();

    void interpret_Message(String message);
    String bluetooth_Read();
    void rf24_Read();
    void Initialize();
};