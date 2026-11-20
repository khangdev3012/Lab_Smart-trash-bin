#pragma once
#include <Arduino.h>

enum WasteType {
    WASTE_EMPTY = 0,
    WASTE_ELECTRONIC,
    WASTE_OTHER,
    WASTE_ERROR
};

void servoInit();
void servoMoveSmooth(int targetAngle);
void servoDumpWaste(WasteType type);
void servoDetach();