/**
 * @file uart_mode.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief Unit QRCode UART Mode Example
 * @version 0.1
 * @date 2024-01-30
 *
 *
 * @Hardwares: M5Core + Unit Synth
 * @Platform Version: Arduino M5Stack Board Manager v2.1.0
 * @Dependent Library:
 * M5Unified: https://github.com/m5stack/M5Unified
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5UnitQRCode: https://github.com/m5stack/M5Unit-QRCode
 */
#include <M5Unified.h>
#include <M5GFX.h>
#include "M5UnitQRCode.h"

M5Canvas canvas(&M5.Display);

M5UnitQRCodeUART qrcode;

#define UART_AUTO_SCAN_MODE

void setup() {
    M5.begin();

    canvas.setColorDepth(1);  // mono color
    canvas.createSprite(M5.Display.width(), M5.Display.height());
    canvas.setTextSize((float)canvas.width() / 160);
    canvas.setTextScroll(true);

    while (!qrcode.begin(&Serial2, UNIT_QRCODE_UART_BAUD, 16, 17)) {
        canvas.println("Unit QRCode UART Init Fail");
        Serial.println("Unit QRCode UART Init Fail");
        canvas.pushSprite(0, 0);
        delay(1000);
    }
    canvas.println("Unit QRCode UART Init Success");
    Serial.println("Unit QRCode UART Init Success");
#ifdef UART_AUTO_SCAN_MODE
    canvas.println("Auto Scan Mode");
    canvas.pushSprite(0, 0);
    qrcode.setTriggerMode(AUTO_SCAN_MODE);
#else
    canvas.println("Manual Scan Mode");
    canvas.pushSprite(0, 0);
    qrcode.setTriggerMode(MANUAL_SCAN_MODE);
#endif
}

void loop() {
    if (qrcode.available()) {
        String data     = qrcode.getDecodeData();
        uint16_t length = data.length();
        Serial.printf("len:%d\r\n", length);
        Serial.printf("decode data:");
        Serial.println(data);
        canvas.println(data);
        Serial.println();
        canvas.println();
        canvas.pushSprite(0, 0);
    }
#ifndef UART_AUTO_SCAN_MODE
    M5.update();
    if (M5.BtnA.wasPressed()) {
        // start scan
        qrcode.setDecodeTrigger(1);
    }
    if (M5.BtnB.wasPressed()) {
        // stop scan
        qrcode.setDecodeTrigger(0);
    }
#endif
}