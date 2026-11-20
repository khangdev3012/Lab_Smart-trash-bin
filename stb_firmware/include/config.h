#pragma once
#include <Arduino.h>

// ================= CHẾ ĐỘ DEBUG =================
// Đặt false để chạy lặp trên bàn test qua Serial (không ngắt USB)
// Đặt true khi đã gắn pin/nguồn rời để chạy Deep Sleep thực tế
#define ENABLE_DEEP_SLEEP      true

// ================= PHÂN BỔ CHÂN GPIO =================
// Cảm biến siêu âm 1 (Ngăn Rác Linh Kiện)
#define PIN_TRIG_1             46
#define PIN_ECHO_1             3     // CẢNH BÁO: Trùng chân Camera Y4! Khi gắn AI nên đổi sang GPIO 1

// Cảm biến siêu âm 2 (Ngăn Rác Khác)
#define PIN_TRIG_2             41     // CẢNH BÁO: Trùng chân Camera Y3! Khi gắn AI nên đổi sang GPIO 2
#define PIN_ECHO_2             42    // Lưu ý: Strapping pin, đảm bảo không bị kéo HIGH lúc boot

// Cảm biến phát hiện rác / Wake-up (Bắt buộc là RTC GPIO 0..21)
#define PIN_SENSOR_WAKE        14
// Module IR tránh vật cản thường là LOW khi có vật cản (đổi thành HIGH nếu dùng PIR)
#define WAKE_ACTIVE_LEVEL      LOW

// Đèn LED trợ sáng & Servo 20KG
#define PIN_LED                38
#define PIN_SERVO              39

// ================= THÔNG SỐ SERVO TD-8120MG =================
#define SERVO_PWM_MIN_US       500   // Theo thông số 500us ~ 2500us
#define SERVO_PWM_MAX_US       2500
#define SERVO_ANGLE_CENTER     90    // Vị trí khay nằm ngang
#define SERVO_ANGLE_EWASTE     35    // Lật trái: Ngăn rác linh kiện
#define SERVO_ANGLE_OTHER      145   // Lật phải: Ngăn rác khác
#define SERVO_STEP_DEG         2     // Bước xoay mỗi lần (độ) để lật mượt
#define SERVO_STEP_DELAY_MS    15    // Thời gian nghỉ giữa mỗi bước (ms)
#define SERVO_HOLD_DROP_MS     1500  // Thời gian giữ khay nghiêng cho rác rơi hết

// ================= THÔNG SỐ SIÊU ÂM HC-SR04 =================
#define US_NUM_SAMPLES         5     // Số mẫu đo để lọc trung vị
#define US_SAMPLE_DELAY_MS     30    // Nghỉ giữa các mẫu đo
#define US_CROSSTALK_DELAY_MS  60    // Nghỉ giữa 2 cảm biến tránh nhiễu sóng chéo
#define US_TIMEOUT_US          25000 // Timeout ~4.2m để tránh treo pulseIn

#define BIN_EMPTY_DIST_CM      35.0f // Khoảng cách từ cảm biến tới đáy thùng trống (cm)
#define BIN_FULL_DIST_CM       8.0f  // Khoảng cách báo đầy (cm, phải >= 5cm để tránh điểm mù)
#define BIN_FULL_PERCENT_TH    85.0f // Ngưỡng % báo đầy
#define BIN_FULL_STREAK_LIMIT  2     // Số lần đo đầy liên tiếp để xác nhận (chống báo giả)

// ================= THỜI GIAN CHU TRÌNH =================
#define SETTLE_DELAY_MS        1500  // Chờ người dùng rút tay khỏi khay
#define SLEEP_CLEAR_TIMEOUT_MS 10000 // Chờ tối đa 10s cảm biến về trạng thái nghỉ trước khi sleep
#define AI_CONFIDENCE_TH       0.70f // Ngưỡng tin cậy tối thiểu