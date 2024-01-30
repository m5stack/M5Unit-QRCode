/**
 * @file i2c_mode.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief Unit QRCode I2C Mode Example
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

M5UnitQRCodeI2C qrcode;

#define I2C_AUTO_SCAN_MODE

void setup() {
    M5.begin();

    canvas.setColorDepth(1);  // mono color
    canvas.createSprite(M5.Display.width(), M5.Display.height());
    canvas.setTextSize((float)canvas.width() / 160);
    canvas.setTextScroll(true);

    while (!qrcode.begin(&Wire, UNIT_QRCODE_ADDR, 21, 22, 100000U)) {
        canvas.printf("Unit QRCode Init Fail\r\n", M5.Display.width() / 2,
                      M5.Display.height() / 2);
        Serial.println("Unit QRCode Init Fail");
        canvas.pushSprite(0, 0);
        delay(1000);
    }

#ifdef I2C_AUTO_SCAN_MODE
    canvas.printf("Unit QRCode Init Success\r\n", M5.Display.width() / 2,
                  M5.Display.height() / 2);
    Serial.println("Unit QRCode Init Success");
    canvas.println("Auto Scan Mode");
    canvas.pushSprite(0, 0);
    qrcode.setTriggerMode(AUTO_SCAN_MODE);
#else
    canvas.printf("Unit QRCode Init Success\r\n", M5.Display.width() / 2,
                  M5.Display.height() / 2);
    Serial.println("Unit QRCode Init Success");
    canvas.println("Manual Scan Mode");
    canvas.pushSprite(0, 0);
    qrcode.setTriggerMode(MANUAL_SCAN_MODE);
#endif
}

void loop() {
    if (qrcode.getDecodeReadyStatus() == 1) {
        uint8_t buffer[512] = {0};
        uint16_t length     = qrcode.getDecodeLength();
        Serial.printf("len:%d\r\n", length);
        qrcode.getDecodeData(buffer, length);
        Serial.printf("decode data:");
        for (int i = 0; i < length; i++) {
            Serial.printf("%c", buffer[i]);
            canvas.printf("%c", buffer[i]);
        }
        Serial.println();
        canvas.println();
        canvas.pushSprite(0, 0);
    }
#ifndef I2C_AUTO_SCAN_MODE
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