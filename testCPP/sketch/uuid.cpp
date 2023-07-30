#include<string>
#include "uuid.h"
#include "definitions.h"

int uuidToInt(std::string uuid)
{
    std::string first(1, uuid[0]);
    std::string second(1, uuid[1]);

    return (std::stoi(first) * 10) + std::stoi(second);
}

std::string intToUuid(int id)
{
    switch (id)
    {
        case CHAR_A:
            return CHAR_ID_A;
        break;
        case CHAR_B:
            return CHAR_ID_B;
        break;
        case CHAR_C:
            return CHAR_ID_C;
        break;
        case CHAR_D:
            return CHAR_ID_D;
        break;
        case CHAR_E:
            return CHAR_ID_E;
        break;
        case CHAR_F:
            return CHAR_ID_F;
        break;
        case CHAR_G:
            return CHAR_ID_G;
        break;
        case CHAR_H:
            return CHAR_ID_H;
        break;
        case CHAR_I:
            return CHAR_ID_I;
        break;
        case CHAR_J:
            return CHAR_ID_J;
        break;
    }
    return "";
}