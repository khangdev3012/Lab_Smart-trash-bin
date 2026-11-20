#include "power_sleep.h"
#include "config.h"
#include "esp_sleep.h"

RTC_DATA_ATTR static uint32_t s_bootCount      = 0;
RTC_DATA_ATTR static uint8_t  s_bin1FullStreak = 0;
RTC_DATA_ATTR static uint8_t  s_bin2FullStreak = 0;

uint32_t powerGetBootCount() { return s_bootCount; }
void powerIncrementBootCount() { s_bootCount++; }

void powerPrintWakeupReason() {
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    switch (cause) {
        case ESP_SLEEP_WAKEUP_EXT0:
            Serial.println("[PWR] Thuc day tu Deep Sleep do cam bien (EXT0 GPIO 14).");
            break;
        default:
            Serial.printf("[PWR] Khoi dong moi / Reset (cause=%d).\n", (int)cause);
            break;
    }
}

bool powerUpdateBin1FullStatus(bool currentlyFull) {
    s_bin1FullStreak = currentlyFull ? min((uint8_t)255, (uint8_t)(s_bin1FullStreak + 1)) : 0;
    return (s_bin1FullStreak >= BIN_FULL_STREAK_LIMIT);
}

bool powerUpdateBin2FullStatus(bool currentlyFull) {
    s_bin2FullStreak = currentlyFull ? min((uint8_t)255, (uint8_t)(s_bin2FullStreak + 1)) : 0;
    return (s_bin2FullStreak >= BIN_FULL_STREAK_LIMIT);
}

void powerEnterDeepSleep() {
    pinMode(PIN_SENSOR_WAKE, INPUT);

    Serial.printf("[PWR] Cho cam bien GPIO %d ve trang thai nghi (khac %s)...\n",
                  PIN_SENSOR_WAKE, (WAKE_ACTIVE_LEVEL == HIGH) ? "HIGH" : "LOW");

    uint32_t startWait = millis();
    uint32_t stableSince = 0;

    // Chờ tín hiệu rời khỏi mức kích hoạt ổn định liên tục 300ms (tránh vừa sleep đã tỉnh lại)
    while (millis() - startWait < SLEEP_CLEAR_TIMEOUT_MS) {
        if (digitalRead(PIN_SENSOR_WAKE) != WAKE_ACTIVE_LEVEL) {
            if (stableSince == 0) stableSince = millis();
            if (millis() - stableSince >= 300) {
                break;
            }
        } else {
            stableSince = 0;
        }
        delay(20);
    }

    if (digitalRead(PIN_SENSOR_WAKE) == WAKE_ACTIVE_LEVEL) {
        Serial.println("[PWR] CANH BAO: Cam bien van dang bi kich (ket rac/chinh bien tro IR)!");
    }

    // Cấu hình ngắt đánh thức EXT0 theo mức kích hoạt đã chọn trong config.h
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_SENSOR_WAKE, WAKE_ACTIVE_LEVEL);

    Serial.println("[PWR] === VAO CHE DO DEEP SLEEP ===\n");
    Serial.flush();
    delay(50);

    esp_deep_sleep_start();
}