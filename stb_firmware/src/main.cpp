#include <Arduino.h>
#include "config.h"
#include "ultrasonic.h"
#include "servo.h"
#include "classifier.h"
#include "power_sleep.h"

static const char* wasteTypeToStr(WasteType t) {
    switch (t) {
        case WASTE_ELECTRONIC: return "RAC LINH KIEN";
        case WASTE_OTHER:      return "RAC KHAC";
        case WASTE_EMPTY:      return "KHAY TRONG";
        default:               return "LOI";
    }
}

static void runSortingCycle() {
    powerIncrementBootCount();
    uint32_t cycle = powerGetBootCount();
    Serial.printf("\n========== BAT DAU CHU TRINH #%lu ==========\n", (unsigned long)cycle);

    // Bước 1: Chờ người dùng rút tay khỏi khay
    Serial.printf("[FSM] Cho on dinh khay %d ms...\n", SETTLE_DELAY_MS);
    delay(SETTLE_DELAY_MS);

    // Bước 2: Chiếu sáng & Suy luận phân loại (đang chạy Mock)
    ClassifyResult ai = classifierRun(cycle);
    Serial.printf("[FSM] Ket qua: %s (%.1f%%) trong %lu ms\n",
                  wasteTypeToStr(ai.type), ai.confidence * 100.0f, (unsigned long)ai.inferenceMs);

    // Bước 3: Xoay mượt Servo TD-8120MG lật bản lật
    servoDumpWaste(ai.type);

    // Bước 4: Đo 2 cảm biến siêu âm HC-SR04 (lọc trung vị)
    BinLevel bin1, bin2;
    ultrasonicReadAll(bin1, bin2);

    Serial.printf("[US1 - Linh kien] KC: %.1f cm | Day: %.1f%%\n",
                  bin1.distanceCm, bin1.fillPercent);
    Serial.printf("[US2 - Rac khac ] KC: %.1f cm | Day: %.1f%%\n",
                  bin2.distanceCm, bin2.fillPercent);

    bool alertBin1 = (bin1.distanceCm > 0) && powerUpdateBin1FullStatus(bin1.isFull);
    bool alertBin2 = (bin2.distanceCm > 0) && powerUpdateBin2FullStatus(bin2.isFull);

    if (alertBin1) Serial.println("[CANH BAO] >>> NGAN RAC LINH KIEN DA DAY! <<<");
    if (alertBin2) Serial.println("[CANH BAO] >>> NGAN RAC KHAC DA DAY! <<<");

    Serial.println("========== KET THUC CHU TRINH ==========");
}

void setup() {
    Serial.begin(115200);
    delay(300); // Đợi cổng Serial ổn định sau khi wake-up

    pinMode(PIN_SENSOR_WAKE, INPUT);
    powerPrintWakeupReason();

    // Khởi tạo toàn bộ ngoại vi
    ultrasonicInit();
    servoInit();
    classifierInit();

#if ENABLE_DEEP_SLEEP
    // Chạy 1 chu trình trọn vẹn sau khi thức dậy, sau đó tắt ngoại vi và ngủ sâu
    runSortingCycle();

    classifierDeinit();
    servoDetach();
    powerEnterDeepSleep();
#else
    Serial.println("[DEBUG] Che do KHONG NGU SAU: Dua vat can vao cam bien IR de test.");
#endif
}

void loop() {
#if !ENABLE_DEEP_SLEEP
    // Chờ có vật cản kích hoạt trên GPIO 14
    if (digitalRead(PIN_SENSOR_WAKE) == WAKE_ACTIVE_LEVEL) {
        runSortingCycle();
        // Chờ rút vật cản khỏi cảm biến trước khi cho phép kích hoạt đợt kế tiếp
        while (digitalRead(PIN_SENSOR_WAKE) == WAKE_ACTIVE_LEVEL) {
            delay(50);
        }
        delay(500);
    }
    delay(20);
#endif
}