#ifndef MESSAGE_H
#define MESSAGE_H

#include<string>

void flushMessages();
void lRead(std::string uuid);
void lWrite(std::string uuid, int value);

#endif