#include "classifier.h"
#include "config.h"

void classifierInit() {
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // TODO (Sau khi train xong Edge Impulse):
    // 1. Gọi esp_camera_init(&config) cho OV5640
    // 2. Xả 2 frame đầu để cân bằng phơi sáng (AEC/AWB)
    Serial.println("[AI] Khoi tao module AI (Che do GIA LAP - Mock).");
}

ClassifyResult classifierRun(uint32_t cycleIndex) {
    ClassifyResult res = { WASTE_OTHER, 0.0f, 0 };
    uint32_t t0 = millis();

    // Bật LED trợ sáng mô phỏng đúng pha chụp ảnh thật
    Serial.println("[AI] Bat LED tro sang -> Chup anh (gia lap)...");
    digitalWrite(PIN_LED, HIGH);
    delay(300);

    // =========================================================================
    // TODO (Sau khi train xong Edge Impulse):
    // Thay khối code giả lập bên dưới bằng:
    //   1. esp_camera_fb_get() -> fmt2rgb888() -> crop_and_interpolate_rgb888()
    //   2. run_classifier(&signal, &ei_result, false)
    //   3. Gán res.type và res.confidence từ nhãn có điểm cao nhất
    // =========================================================================
    uint8_t mockCase = cycleIndex % 3;
    if (mockCase == 1) {
        res.type = WASTE_ELECTRONIC;
        res.confidence = 0.92f;
    } else if (mockCase == 2) {
        res.type = WASTE_OTHER;
        res.confidence = 0.88f;
    } else {
        res.type = WASTE_EMPTY;
        res.confidence = 0.95f;
    }

    digitalWrite(PIN_LED, LOW);
    res.inferenceMs = millis() - t0;

    // Quy tắc Fail-safe: Độ tin cậy thấp hơn ngưỡng -> đẩy về ngăn rác thường
    if (res.type == WASTE_ELECTRONIC && res.confidence < AI_CONFIDENCE_TH) {
        Serial.println("[AI] Do tin cay thap -> Fail-safe chuyen sang RAC KHAC.");
        res.type = WASTE_OTHER;
    }

    return res;
}

void classifierDeinit() {
    digitalWrite(PIN_LED, LOW);
    // TODO (Sau khi gắn OV5640): Gọi esp_camera_deinit();
}