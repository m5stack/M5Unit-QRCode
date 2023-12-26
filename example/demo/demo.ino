#include <M5Stack.h>
#include <M5GFX.h>
#include "UNIT_QRCODE.h"

M5GFX display;
M5Canvas canvas(&display);
UNIT_QRCODE qrcode;
uint8_t buffer[512] = {0};
uint16_t length     = 0;

void setup() {
    M5.begin(true, false, true);
    M5.Power.begin();
    display.begin();
    canvas.setColorDepth(8);
    canvas.setFont(&fonts::efontCN_12);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize(2);
    canvas.setTextScroll(true);
    while (!qrcode.begin(&Wire, UNIT_QRCODE_ADDR, 21, 22, 100000U)) {
        Serial.println("qrcode Connect Error");
        M5.Lcd.print("qrcode Connect Error");
        delay(100);
    }
}

void loop() {
    canvas.clear(BLACK);
    canvas.setTextSize(2);
    canvas.setTextColor(ORANGE);
    canvas.drawString("QRCode Scan", 80, 0);
    canvas.setColor(ORANGE);
    canvas.drawFastHLine(0, 30, 320);
    canvas.drawFastHLine(0, 190, 320);
    canvas.setTextColor(WHITE);
    canvas.drawString("[start]", 20, 200);
    canvas.drawString("[stop]", 230, 200);
    canvas.setCursor(0, 30);
    canvas.setTextColor(GREEN);
    for (int i = 0; i < (length < 156 ? length : 156); i++) {
        canvas.printf("%c", buffer[i]);
    }
    canvas.pushSprite(0, 0);
    if (qrcode.getDecodeReadyStatus() == 1) {
        length = qrcode.getDecodeLength();
        Serial.printf("len:%d\r\n", length);
        qrcode.getDecodeData(buffer, length);
        Serial.printf("decode data:");
        canvas.setCursor(15, 30);
        canvas.setTextColor(GREEN);
        for (int i = 0; i < length; i++) {
            Serial.printf("%c", buffer[i]);
            canvas.printf("%c", buffer[i]);
        }
        Serial.println();
    }
    canvas.pushSprite(0, 0);
    M5.update();
    if (M5.BtnA.wasPressed()) {
        qrcode.setDecodeTrigger(1);
    } else if (M5.BtnC.wasPressed()) {
        qrcode.setDecodeTrigger(0);
    }
}