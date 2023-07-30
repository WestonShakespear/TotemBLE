#include<iostream>
#include<string>

#include "debugSerial.h"
#include "definitions.h"

#include <Arduino.h>


void debug(std::string message)
{
    for (int i = 0; i < message.length(); i++)
    {
        Serial.print(message[i]);
    }
    Serial.println();
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