#ifndef AG_MULTIPLEXER_H
#define AG_MULTIPLEXER_H
#define MUX_A D5
#define MUX_B D6
#define MUX_C D7

#include <Arduino.h>

class AGMultiplexer {
public:
    AGMultiplexer();
    void begin();
    void write(int c, int b, int a);
    void setForDHT();       // Channel 1
    void setForVoltage();   // Channel 5
};

#endif // AG_MULTIPLEXER_H
