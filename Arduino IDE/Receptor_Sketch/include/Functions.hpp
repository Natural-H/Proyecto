#pragma once

#include <Arduino.h>
#include <string.h>
#include <Vector.h>

#include "Things/Things.hpp"
#include "Things/Leds/Led.hpp"
#include "Things/Leds/RGB_Led.hpp"

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

    Things *thing;
    Vector<byte> pins;

    Function(String function);
    ~Function();

    struct Action_List
    {
        String light = "light";
        String rgbLight = "rgb_light";
    };

    Action_List _action;
    
public:
    String getName();
    String getNameToDisplay();

    static Vector<Function> functions;
    static void createFunc(String msg);

    String getDefaultArgs();

    int getX();
    int getY();
    int getZ();
    int getR();

    void takeValues(String function);
    void setFunction(String function);
    void Execute();
    void Execute(int args);
    void Execute(String args);
};