/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#include "M5UnitQRCodeUART.h"

bool M5UnitQRCodeUART::begin(HardwareSerial *serial, int baud, uint8_t RX, uint8_t TX) {
    _serial = serial;
    _serial->begin(baud, SERIAL_8N1, RX, TX);
    clear();
    uint8_t cmd[] = {0x23, 0x61, 0x41};
    sendCMD(cmd, 3);
    delay(100);
    while (_serial->available()) {
        _rx_buffer[_index] = _serial->read();
        _index++;
    }
    if (_rx_buffer[0] == 0x24 && _rx_buffer[1] == 0x61 && _rx_buffer[2] == 0x41) {
        clear();
        return true;
    }
    clear();
    return false;
}

bool M5UnitQRCodeUART::parseData() {
    if (_rx_buffer[0] == 0x33 && _rx_buffer[1] == 0x75 && _rx_buffer[2] == 0x02) {
        return false;
    }
    if (_rx_buffer[0] == 0x22 && _rx_buffer[1] == 0x61 && _rx_buffer[2] == 0x41) {
        return false;
    }
    String data = String((char *)_rx_buffer);
    if (data.length() > 1) {
        _data = data;
        return true;
    }
    return false;
}

void M5UnitQRCodeUART::clear() {
    _index = 0;
    memset(_rx_buffer, 0, sizeof(_rx_buffer));
}

bool M5UnitQRCodeUART::available() {
    clear();
    while (_serial->available()) {
        _rx_buffer[_index] = _serial->read();
        _index++;
    }
    if (parseData()) {
        return true;
    }
    return false;
}

String M5UnitQRCodeUART::getDecodeData(void) {
    return _data;
}

void M5UnitQRCodeUART::sendCMD(uint8_t *_rx_buffer, size_t size) {
    _serial->write(_rx_buffer, size);
}

void M5UnitQRCodeUART::setDecodeTrigger(bool en) {
    if (en) {
        uint8_t cmd[] = {0x32, 0x75, 0x01};
        sendCMD(cmd, 3);
    } else {
        uint8_t cmd[] = {0x32, 0x75, 0x02};
        sendCMD(cmd, 3);
    }
}

void M5UnitQRCodeUART::setTriggerMode(qrcode_scan_mode_t mode) {
    if (mode == AUTO_SCAN_MODE) {
        uint8_t cmd[] = {0x21, 0x61, 0x41, 0x05};
        sendCMD(cmd, 4);
    } else {
        uint8_t cmd[] = {0x21, 0x61, 0x41, 0x00};
        sendCMD(cmd, 4);
    }
}
