#include<string>

#include "message.h"
#include "uuid.h"
#include "definitions.h"


int lastMessage[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int settings[] = {SA, SB, SC, SD, SE, SF, SG, SH, SI, SJ};


void flushMessages()
{
    // reset the message array
    for (int i = 0; i < CHAR_LEN; i++)
    {
        lastMessage[i] =  0;
    }
}

void lRead(std::string uuid)
{
    lastMessage[uuidToInt(uuid)] = 0;
}

void lWrite(std::string uuid, int value)
{
    if (value == -1)
    {
        flushMessages();
        return;
    }

    int uuidInt = uuidToInt(uuid);
    if (lastMessage[uuidInt] == 0)
    {
        lastMessage[uuidInt] = value;
        settings[uuidInt] = value;
    }
}