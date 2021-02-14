#pragma once

#include <Arduino.h>
#include <Vector.h>
#include <string.h>

class Things
{
protected:
    String name;

public:
    static Vector<Things *> things;
    Things(String Name);
    ~Things();
    
    String getName();

    virtual void Action();
    virtual void Action(byte agrs);
};