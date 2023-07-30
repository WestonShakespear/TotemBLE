#include "definitions.h"
#include "message.h"
#include "uuid.h"


#include "debugSerial.h"

void testUtilityA()
{
    debug("value of charid A: " + std::to_string(uuidToInt(CHAR_ID_A)));
    debug("value of char A int: " + intToUuid(CHAR_A));

    debug("value of charid B: " + std::to_string(uuidToInt(CHAR_ID_B)));
    debug("value of char A int: " + intToUuid(CHAR_B));

    debug("value of charid C: " + std::to_string(uuidToInt(CHAR_ID_C)));
    debug("value of char A int: " + intToUuid(CHAR_C));

    debug("value of charid D: " + std::to_string(uuidToInt(CHAR_ID_D)));
    debug("value of char A int: " + intToUuid(CHAR_D));

    debug("value of charid E: " + std::to_string(uuidToInt(CHAR_ID_E)));
    debug("value of char A int: " + intToUuid(CHAR_E));

    debug("value of charid F: " + std::to_string(uuidToInt(CHAR_ID_F)));
    debug("value of char A int: " + intToUuid(CHAR_F));

    debug("value of charid G: " + std::to_string(uuidToInt(CHAR_ID_G)));
    debug("value of char A int: " + intToUuid(CHAR_G));

    debug("value of charid H: " + std::to_string(uuidToInt(CHAR_ID_H)));
    debug("value of char A int: " + intToUuid(CHAR_H));

    debug("value of charid I: " + std::to_string(uuidToInt(CHAR_ID_I)));
    debug("value of char A int: " + intToUuid(CHAR_I));

    debug("value of charid J: " + std::to_string(uuidToInt(CHAR_ID_J)));
    debug("value of char A int: " + intToUuid(CHAR_J));
}

void testUtilityB()
{
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
}