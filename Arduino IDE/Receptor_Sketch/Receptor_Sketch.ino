#include <Vector.h>

#include "include/Listener.hpp"
#include "include/Functions.hpp"
#include "include/Things/Things.hpp"
#include "include/Things/Leds/Led.hpp"

Listener listener;
Led verifyLed("Verify Led", 2);

void setup()
{
    listener.Initialize();

    //  Tests for funcs
    Function::createFunc("N:myLed X:300 Y:450 Z:500 R:30 F:light A:0 P:4,");
    Function::createFunc("N:myRGBLed X:200 Y:200 Z:500 R:35 F:rgb_light P:6,5,3,");
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
