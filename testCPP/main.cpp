#include<iostream>
#include<cstdlib>
#include<string>


// default settings
#define SA 0
#define SB 0
#define SC 0
#define SD 0
#define SE 0
#define SF 0
#define SG 0
#define SH 0
#define SI 0
#define SJ 0


// settings
// brightness
// mode
// sound
// sensitivity
// lowpower

// uuid "hashmap"
#define CHAR_ID_A "00947533-3143-470c-9c12-0816383efdd8"
#define CHAR_A      0
#define CHAR_ID_B "017a77ec-5a68-45c8-972e-8442cac37928"
#define CHAR_B      1
#define CHAR_ID_C "024ea33f-43da-4e62-9333-baf75af57c12"
#define CHAR_C      2
#define CHAR_ID_D "03097d35-7f88-4bbb-9573-3b5f60f104b6"
#define CHAR_D      3
#define CHAR_ID_E "047883e9-f159-4f98-938e-254691847ef8"
#define CHAR_E      4
#define CHAR_ID_F "050fd81c-38b8-4251-9ad9-67ad3363791e"
#define CHAR_F      5
#define CHAR_ID_G "06b19edf-25eb-4b41-8195-25bb1e6a41b9"
#define CHAR_G      6
#define CHAR_ID_H "0769e5df-633d-4943-9b1a-f49a72992d6c"
#define CHAR_H      7
#define CHAR_ID_I "0865962a-0f0a-4aa5-a38f-5c9ce8852f11"
#define CHAR_I      8
#define CHAR_ID_J "09929676-371d-4abb-ae50-8f14f2541656"
#define CHAR_J      9

// how many previous entries there are
#define CHAR_LEN 10


int lastMessage[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int settings[] = {SA, SB, SC, SD, SE, SF, SG, SH, SI, SJ};


int uuidToInt(std::string uuid);
std::string intToUuid(int id);

void debug(std::string message);
void outputSettings();
void outputMessages();

void testUtilityA();


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



int main()
{
    
    // testUtilityA();
    debug("test write");
    lWrite(CHAR_ID_A, 1);
    lWrite(CHAR_ID_B, 2);
    lWrite(CHAR_ID_C, 3);
    lWrite(CHAR_ID_D, 4);

    outputMessages();
    outputSettings();

    debug("read all");
    lRead(CHAR_ID_A);
    lRead(CHAR_ID_B);
    lRead(CHAR_ID_C);
    lRead(CHAR_ID_D);

    outputMessages();

    debug("writeall");
    lWrite(CHAR_ID_A, 1);
    lWrite(CHAR_ID_B, 2);
    lWrite(CHAR_ID_C, 3);
    lWrite(CHAR_ID_D, 4);


    debug("try to write again");
    lWrite(CHAR_ID_A, 6);
    lWrite(CHAR_ID_B, 6);
    lWrite(CHAR_ID_C, 6);
    lWrite(CHAR_ID_D, 6);

    outputMessages();

    debug("try to flush");
    lWrite(CHAR_ID_A, -1);

    outputMessages();


    system("pause");
    return 1;
}


void debug(std::string message)
{
    for (int i = 0; i < message.length(); i++)
    {
        std::cout << message[i];
    }
    std::cout << std::endl;
}

void testUtilityA()
{

    std::cout << "value of charid A: " << uuidToInt(CHAR_ID_A) << std::endl;
    std::cout << "value of char A int: " << intToUuid(CHAR_A) << std::endl;

    std::cout << "value of charid B: " << uuidToInt(CHAR_ID_B) << std::endl;
    std::cout << "value of char A int: " << intToUuid(CHAR_B) << std::endl;

    std::cout << "value of charid C: " << uuidToInt(CHAR_ID_C) << std::endl;
    std::cout << "value of char A int: " << intToUuid(CHAR_C) << std::endl;

    std::cout << "value of charid D: " << uuidToInt(CHAR_ID_D) << std::endl;
    std::cout << "value of char A int: " << intToUuid(CHAR_D) << std::endl;

    std::cout << "value of charid E: " << uuidToInt(CHAR_ID_E) << std::endl;
    std::cout << "value of char A int: " << intToUuid(CHAR_E) << std::endl;

    std::cout << "value of charid F: " << uuidToInt(CHAR_ID_F) << std::endl;
    std::cout << "value of char A int: " << intToUuid(CHAR_F) << std::endl;

    std::cout << "value of charid G: " << uuidToInt(CHAR_ID_G) << std::endl;
    std::cout << "value of char A int: " << intToUuid(CHAR_G) << std::endl;

    std::cout << "value of charid H: " << uuidToInt(CHAR_ID_H) << std::endl;
    std::cout << "value of char A int: " << intToUuid(CHAR_H) << std::endl;

    std::cout << "value of charid I: " << uuidToInt(CHAR_ID_I) << std::endl;
    std::cout << "value of char A int: " << intToUuid(CHAR_I) << std::endl;

    std::cout << "value of charid J: " << uuidToInt(CHAR_ID_J) << std::endl;
    std::cout << "value of char A int: " << intToUuid(CHAR_J) << std::endl;
}












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


