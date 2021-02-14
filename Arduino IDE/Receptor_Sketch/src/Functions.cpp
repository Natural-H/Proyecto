#include "../include/Functions.hpp"

/**
 * Function Struc:
 * 0:   Execute Func
 * 
 * "N:NameToDisplay"
 * "N:NameToDisplay A:Args"
 * 
 * 1:   Create Func
 * 
 * "N:NameToDisplay X:{Value} Y:{Value} R:{RangeValue} F:{Action}"
 * "N:Name X:{Value} Y:{Value} R:{RangeValue} F:{Action} A:{Args}"
 * 
 * {Action} Values
 * 
 * Released Actions:
 * 
 * None.
 * 
 * -------------------------------------------------------------
 * 
 * Beta Actions:
 * 
 * light - Takes one argument {byte state} 1 = toggle, 2 = on, 3 = off.
 * rgblight - Can take from one to three arguments {byte color} {color, color, color} color is a byte value, from 0 to 255.
**/

Vector<Function> Function::functions;

Function::Function(String function)
{
    Function::takeValues(function);

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

Function::~Function() {}

// Getters and Setters

String Function::getName() { return this->name; }

String Function::getNameToDisplay() { return this->nameToDisplay; }

String Function::getDefaultArgs() { return this->default_args; }

int Function::getX() { return this->x; }

int Function::getY() { return this->y; }

int Function::getZ() { return this->z; }

int Function::getR() { return this->range; }

void Function::createFunc(String msg) { functions.push_back(Function(msg)); }

void Function::Execute()
{
    if (this->default_args)
        this->thing->Action(this->default_args.toInt());
    else
        this->thing->Action();
}

void Function::Execute(int args) { this->thing->Action(args); }

void Function::Execute(String args) { this->thing->Action(args.toInt()); }

void Function::takeValues(String message)
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
