
#ifndef IN_MEMORY_CONST_UTILS_H_
#define IN_MEMORY_CONST_UTILS_H_

#include <Arduino.h>

namespace tankController {

    class InMemoryConstUtils {
    public:
        InMemoryConstUtils();

        void readConstMessage(const char data[], char *buffer);

        String* readConstMessage(const char data[]);

        String getByteAsHEX(unsigned char data);
    };

    extern InMemoryConstUtils IN_MEMORY_CONST_UTILS;

}

#endif // IN_MEMORY_CONST_UTILS_H_
