#include<iostream>
#include<string>

#include "debugPC.h"
#include "definitions.h"

void debug(std::string message)
{
    for (int i = 0; i < message.length(); i++)
    {
        std::cout << message[i];
    }
    std::cout << std::endl;
}

void outputMessages()
{
    debug("Last Messages:");
    for (int i = 0; i < CHAR_LEN; i++)
    {
        debug(std::to_string(lastMessage[i]));
    }
    debug("--------------");
}

void outputSettings()
{
    debug("Current Settings:");
    for (int i = 0; i < CHAR_LEN; i++)
    {
        debug(std::to_string(settings[i]));
    }
    debug("--------------");
}