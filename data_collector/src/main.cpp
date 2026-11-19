#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"
#include "secrets.h"

const char* SSID = "ssid";
const char* PASS = "pass";
#define LED_PIN 47

WebServer server(80);

void initCamera() {
    camera_config_t config = {};
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer   = LEDC_TIMER_0;
    config.pin_d0 = 11; config.pin_d1 = 9;  config.pin_d2 = 8;  config.pin_d3 = 10;
    config.pin_d4 = 12; config.pin_d5 = 18; config.pin_d6 = 17; config.pin_d7 = 16;
    config.pin_xclk = 15; config.pin_pclk = 13;
    config.pin_vsync = 6; config.pin_href = 7;
    config.pin_sccb_sda = 4; config.pin_sccb_scl = 5;
    config.pin_pwdn = -1; config.pin_reset = -1;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size   = FRAMESIZE_QVGA; // 320x240
    config.jpeg_quality = 10;
    config.fb_count     = 2;
    config.fb_location  = CAMERA_FB_IN_PSRAM;
    config.grab_mode    = CAMERA_GRAB_LATEST;

    if (esp_camera_init(&config) != ESP_OK) {
        Serial.println("Camera init failed!");
        while (1) delay(1000);
    }
}

void handleCapture() {
    digitalWrite(LED_PIN, HIGH);
    delay(150);
    // Bỏ 1 frame cũ trong buffer để lấy đúng ảnh có bật LED
    camera_fb_t* fb = esp_camera_fb_get();
    if (fb) esp_camera_fb_return(fb);
    fb = esp_camera_fb_get();
    digitalWrite(LED_PIN, LOW);

    if (!fb) {
        server.send(500, "text/plain", "Capture Failed");
        return;
    }
    server.sendHeader("Content-Disposition", "inline; filename=cap.jpg");
    server.send_P(200, "image/jpeg", (const char*)fb->buf, fb->len);
    esp_camera_fb_return(fb);
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    initCamera();

    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED) delay(300);
    Serial.print("Capture URL: http://");
    Serial.print(WiFi.localIP());
    Serial.println("/capture");

    server.on("/capture", handleCapture);
    server.begin();
}

void loop() { server.handleClient(); }