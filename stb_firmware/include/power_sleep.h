#pragma once
#include <Arduino.h>

uint32_t powerGetBootCount();
void powerIncrementBootCount();
void powerPrintWakeupReason();

// Cập nhật số lần đầy liên tiếp (lưu trên RTC Memory không mất khi Deep Sleep)
bool powerUpdateBin1FullStatus(bool currentlyFull);
bool powerUpdateBin2FullStatus(bool currentlyFull);

// Chờ cảm biến hết tín hiệu kích hoạt rồi vào Deep Sleep
void powerEnterDeepSleep();