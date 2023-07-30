#ifndef DEBUG_SERIAL_H
#define DEBUG_SERIAL_H

#include<string>

extern int lastMessage[];
extern int settings[];

void debug(std::string message);
void outputSettings();
void outputMessages();

#endif