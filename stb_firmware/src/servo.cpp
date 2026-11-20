#include "servo.h"
#include "config.h"
#include <ESP32Servo.h>

static Servo binServo;
static int currentAngle = SERVO_ANGLE_CENTER;

void servoInit() {
    // Dùng Timer 1 trở lên để sau này tránh xung đột LEDC_TIMER_0 của Camera XCLK
    ESP32PWM::allocateTimer(1);
    binServo.setPeriodHertz(50);
    binServo.attach(PIN_SERVO, SERVO_PWM_MIN_US, SERVO_PWM_MAX_US);

    currentAngle = SERVO_ANGLE_CENTER;
    binServo.write(currentAngle);
    delay(300);
}

void servoMoveSmooth(int targetAngle) {
    targetAngle = constrain(targetAngle, 0, 180);
    if (targetAngle == currentAngle) return;

    int step = (targetAngle > currentAngle) ? SERVO_STEP_DEG : -SERVO_STEP_DEG;
    while (currentAngle != targetAngle) {
        if (abs(targetAngle - currentAngle) <= SERVO_STEP_DEG) {
            currentAngle = targetAngle;
        } else {
            currentAngle += step;
        }
        binServo.write(currentAngle);
        delay(SERVO_STEP_DELAY_MS);
    }
}

void servoDumpWaste(WasteType type) {
    if (type == WASTE_EMPTY) {
        Serial.println("[SERVO] Khay trong (kich hoat gia) -> Giu nguyen khay.");
        return;
    }

    int target = (type == WASTE_ELECTRONIC) ? SERVO_ANGLE_EWASTE : SERVO_ANGLE_OTHER;
    Serial.printf("[SERVO] Lat khay toi goc %d do...\n", target);
    servoMoveSmooth(target);

    delay(SERVO_HOLD_DROP_MS);

    Serial.printf("[SERVO] Tra khay ve vi tri can bang (%d do)...\n", SERVO_ANGLE_CENTER);
    servoMoveSmooth(SERVO_ANGLE_CENTER);
}

void servoDetach() {
    if (binServo.attached()) {
        binServo.detach();
    }
}