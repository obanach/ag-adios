#include "AGMultiplexer.h"

AGMultiplexer::AGMultiplexer() {
    begin();
}

void AGMultiplexer::begin() {
    pinMode(MUX_A, OUTPUT);
    pinMode(MUX_B, OUTPUT);
    pinMode(MUX_C, OUTPUT);
}

void AGMultiplexer::write(int c, int b, int a) {
    digitalWrite(MUX_A, a);
    digitalWrite(MUX_B, b);
    digitalWrite(MUX_B, c);
}

void AGMultiplexer::setForDHT() { // Channel 1
    write(LOW, LOW, HIGH);
}

void AGMultiplexer::setForVoltage() { // Channel 5
    write(HIGH, LOW, HIGH);
}