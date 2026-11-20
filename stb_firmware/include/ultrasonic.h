#pragma once
#include <Arduino.h>

struct BinLevel {
    float distanceCm;   // -1.0 nếu lỗi cảm biến / quá tầm đo
    float fillPercent;  // 0.0 -> 100.0%
    bool  isFull;
};

void ultrasonicInit();
BinLevel ultrasonicReadBin(uint8_t trigPin, uint8_t echoPin);
void ultrasonicReadAll(BinLevel &binEwaste, BinLevel &binOther);