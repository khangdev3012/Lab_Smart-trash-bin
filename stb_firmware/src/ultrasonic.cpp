#include "ultrasonic.h"
#include "config.h"

static float measureSingleCm(uint8_t trigPin, uint8_t echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned long duration = pulseIn(echoPin, HIGH, US_TIMEOUT_US);
    if (duration == 0) {
        return -1.0f; // Timeout / cảm biến không phản hồi
    }

    float dist = (float)duration / 58.0f;
    if (dist < 2.0f || dist > 400.0f) {
        return -1.0f;
    }
    return dist;
}

static void sortAscending(float *arr, uint8_t count) {
    for (uint8_t i = 1; i < count; i++) {
        float key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void ultrasonicInit() {
    pinMode(PIN_TRIG_1, OUTPUT);
    pinMode(PIN_ECHO_1, INPUT);
    digitalWrite(PIN_TRIG_1, LOW);

    pinMode(PIN_TRIG_2, OUTPUT);
    pinMode(PIN_ECHO_2, INPUT);
    digitalWrite(PIN_TRIG_2, LOW);
}

BinLevel ultrasonicReadBin(uint8_t trigPin, uint8_t echoPin) {
    float samples[US_NUM_SAMPLES];
    uint8_t validCount = 0;

    for (uint8_t i = 0; i < US_NUM_SAMPLES; i++) {
        float d = measureSingleCm(trigPin, echoPin);
        if (d > 0.0f) {
            samples[validCount++] = d;
        }
        delay(US_SAMPLE_DELAY_MS);
    }

    BinLevel result = { -1.0f, 0.0f, false };
    if (validCount == 0) {
        return result;
    }

    sortAscending(samples, validCount);
    float medianDist = samples[validCount / 2];

    float span = BIN_EMPTY_DIST_CM - BIN_FULL_DIST_CM;
    float pct = (span > 0.0f) ? ((BIN_EMPTY_DIST_CM - medianDist) / span) * 100.0f : 0.0f;
    pct = constrain(pct, 0.0f, 100.0f);

    result.distanceCm  = medianDist;
    result.fillPercent = pct;
    result.isFull      = (medianDist <= BIN_FULL_DIST_CM) || (pct >= BIN_FULL_PERCENT_TH);
    return result;
}

void ultrasonicReadAll(BinLevel &binEwaste, BinLevel &binOther) {
    binEwaste = ultrasonicReadBin(PIN_TRIG_1, PIN_ECHO_1);
    delay(US_CROSSTALK_DELAY_MS); // Chống nhiễu sóng siêu âm chéo giữa 2 ngăn
    binOther  = ultrasonicReadBin(PIN_TRIG_2, PIN_ECHO_2);
}