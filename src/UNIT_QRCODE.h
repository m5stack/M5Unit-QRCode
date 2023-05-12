#ifndef __UNIT_QRCODE_H
#define __UNIT_QRCODE_H

#include "Arduino.h"
#include "Wire.h"

#define UNIT_QRCODE_ADDR 0x21
#define UNIT_QRCODE_TRIGGER_REG 0x0000
#define UNIT_QRCODE_READY_REG 0x0010
#define UNIT_QRCODE_LENGTH_REG 0x0020
#define UNIT_QRCODE_DATA_REG 0x1000

#define JUMP_TO_BOOTLOADER_REG 0x00FD
#define FIRMWARE_VERSION_REG 0x00FE
#define I2C_ADDRESS_REG 0x00FF

class UNIT_QRCODE {
private:
  uint8_t _addr;
  TwoWire *_wire;
  uint8_t _scl;
  uint8_t _sda;
  uint8_t _speed;
  void writeBytes(uint8_t addr, uint16_t reg, uint8_t *buffer, uint8_t length);
  void readBytes(uint8_t addr, uint16_t reg, uint8_t *buffer, uint16_t length);

public:
  bool begin(TwoWire *wire = &Wire, uint8_t addr = UNIT_QRCODE_ADDR,
             uint8_t sda = 21, uint8_t scl = 22, uint32_t speed = 100000L);
  uint8_t getFirmwareVersion(void);
  void jumpBootloader(void);
  uint8_t setI2CAddress(uint8_t addr);
  uint8_t getI2CAddress(void);
  void setDecodeTrigger(bool en);
  uint8_t getDecodeReadyStatus(void);
  uint16_t getDecodeLength(void);
  void getDecodeData(uint8_t *data, uint16_t len);
};

#endif
