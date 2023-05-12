#include "UNIT_QRCODE.h"

void UNIT_QRCODE::writeBytes(uint8_t addr, uint16_t reg, uint8_t *buffer,
                             uint8_t length) {
    uint8_t temp[2];

    temp[0] = (reg & 0x00ff);
    temp[1] = ((reg >> 8) & 0x00ff);

    _wire->beginTransmission(addr);
    _wire->write(temp[0]);
    _wire->write(temp[1]);
    for (int i = 0; i < length; i++) {
        _wire->write(*(buffer + i));
    }
    _wire->endTransmission();
}

void UNIT_QRCODE::readBytes(uint8_t addr, uint16_t reg, uint8_t *buffer,
                            uint16_t length) {
    uint8_t temp[2];

    temp[0] = (reg & 0x00ff);
    temp[1] = ((reg >> 8) & 0x00ff);

    _wire->beginTransmission(addr);
    _wire->write(temp[0]);
    _wire->write(temp[1]);
    _wire->endTransmission(false);
    _wire->requestFrom(addr, length);
    for (uint16_t i = 0; i < length; i++) {
        buffer[i] = _wire->read();
    }
}

bool UNIT_QRCODE::begin(TwoWire *wire, uint8_t addr, uint8_t sda, uint8_t scl,
                        uint32_t speed) {
    _wire  = wire;
    _addr  = addr;
    _sda   = sda;
    _scl   = scl;
    _speed = speed;
    _wire->begin(_sda, _scl, _speed);
    delay(10);
    _wire->beginTransmission(_addr);
    uint8_t error = _wire->endTransmission();
    if (error == 0) {
        return true;
    } else {
        return false;
    }
}

void UNIT_QRCODE::setDecodeTrigger(bool en) {
    writeBytes(_addr, UNIT_QRCODE_TRIGGER_REG, (uint8_t *)&en, 1);
}

uint8_t UNIT_QRCODE::getDecodeReadyStatus(void) {
    uint8_t value = 0;

    readBytes(_addr, UNIT_QRCODE_READY_REG, (uint8_t *)&value, 1);
    return value;
}

uint16_t UNIT_QRCODE::getDecodeLength(void) {
    uint16_t value = 0;

    readBytes(_addr, UNIT_QRCODE_LENGTH_REG, (uint8_t *)&value, 2);
    return value;
}

void UNIT_QRCODE::getDecodeData(uint8_t *data, uint16_t len) {
    readBytes(_addr, UNIT_QRCODE_DATA_REG, data, len);
}

void UNIT_QRCODE::jumpBootloader(void) {
    uint8_t value = 1;

    writeBytes(_addr, JUMP_TO_BOOTLOADER_REG, (uint8_t *)&value, 1);
}

uint8_t UNIT_QRCODE::setI2CAddress(uint8_t addr) {
    uint8_t temp[2] = {0};

    temp[0] = I2C_ADDRESS_REG;
    temp[1] = (I2C_ADDRESS_REG >> 8);

    _wire->beginTransmission(_addr);
    _wire->write(temp[0]);
    _wire->write(temp[1]);
    _wire->write(addr);
    _wire->endTransmission();
    _addr = addr;
    return _addr;
}

uint8_t UNIT_QRCODE::getI2CAddress(void) {
    uint8_t temp[2] = {0};

    temp[0] = I2C_ADDRESS_REG;
    temp[1] = (I2C_ADDRESS_REG >> 8);

    _wire->beginTransmission(_addr);
    _wire->write(temp[0]);
    _wire->write(temp[1]);
    _wire->endTransmission();

    uint8_t RegValue;

    _wire->requestFrom(_addr, 1);
    RegValue = Wire.read();
    return RegValue;
}

uint8_t UNIT_QRCODE::getFirmwareVersion(void) {
    uint8_t temp[2] = {0};

    temp[0] = FIRMWARE_VERSION_REG;
    temp[1] = (FIRMWARE_VERSION_REG >> 8);

    _wire->beginTransmission(_addr);
    _wire->write(temp[0]);
    _wire->write(temp[1]);
    _wire->endTransmission();

    uint8_t RegValue;

    _wire->requestFrom(_addr, 1);
    RegValue = Wire.read();
    return RegValue;
}
