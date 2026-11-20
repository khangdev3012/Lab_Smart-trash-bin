#pragma once
#include <Arduino.h>
#include "servo.h"

struct ClassifyResult {
    WasteType type;
    float     confidence;
    uint32_t  inferenceMs;
};

void classifierInit();
ClassifyResult classifierRun(uint32_t cycleIndex);
void classifierDeinit();