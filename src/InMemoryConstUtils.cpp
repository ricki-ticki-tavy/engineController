#include<InMemoryConstUtils.h>
#include <Arduino.h>
#include <pgmspace.h>
#include "Logger.h"

namespace tankController {

    InMemoryConstUtils::InMemoryConstUtils() {
    }

    const static char MSG_CONVERT__HEX[] PROGMEM = "0123456789ABCDEF";

    String InMemoryConstUtils::getByteAsHEX(unsigned char data) {
        return "0x" + pgm_read_byte_near(MSG_CONVERT__HEX + ((data & 0xF0) >> 4)) +
               pgm_read_byte_near(MSG_CONVERT__HEX + (data & 0x0F));
    }
//-------------------------------------------------------------------------------------


/**
   Вывод сообщения, расположенного в прграммной памяти
*/
    void InMemoryConstUtils::readConstMessage(const char data[], char *buffer) {

        char anChar;
        int len = strlen_P(data);

        ((char *) (((unsigned long) buffer) + len))[0] = 0;
        for (int i = 0; i < len; i++) {
            ((char *) ((unsigned long) buffer++))[0] = pgm_read_byte_near(data + i);
//    buffer[i] = pgm_read_byte_near(data + i);
        }
    }


    /**
   Вывод сообщения, расположенного в прграммной памяти
*/
    String*  InMemoryConstUtils::readConstMessage(const char data[]) {
        char anChar;
        int len = strlen_P(data);

        String* strBuffer = new String(len + 1);

        for (int i = 0; i < len; i++) {
            strBuffer += pgm_read_byte_near(data + i);
        }
        strBuffer += 0;

        return strBuffer;
    }

    InMemoryConstUtils IN_MEMORY_CONST_UTILS;

}
