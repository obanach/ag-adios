#ifndef AG_MULTIPLEXER_H
#define AG_MULTIPLEXER_H

#include <Arduino.h>

class AGMultiplexer {
public:
    AGMultiplexer();
    void begin();
    void write(int c, int b, int a);
    void setForDirt();       // Channel 1
    void setForVoltage();    // Channel 5
};

#endif // AG_MULTIPLEXER_H
