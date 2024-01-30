#ifndef _UNIT_QRCODE_UART_H_
#define _UNIT_QRCODE_UART_H_

#include "M5UnitQRCode.h"
#include "Arduino.h"

#define UNIT_QRCODE_UART_BAUD 115200

class M5UnitQRCodeUART {
   private:
    HardwareSerial *_serial;
    void sendCMD(uint8_t *buffer, size_t size);
    char _rx_buffer[256];
    uint8_t _index;
    String _data;

   public:
    bool begin(HardwareSerial *serial = &Serial2,
               int baud = UNIT_QRCODE_UART_BAUD, uint8_t RX = 16,
               uint8_t TX = 17);
    void setDecodeTrigger(bool en);
    void setTriggerMode(qrcode_scan_mode_t mode);
    bool available(void);
    String getDecodeData(void);
    bool parseData(void);
    void clear();
};

#endif