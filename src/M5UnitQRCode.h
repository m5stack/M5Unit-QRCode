/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef _UNIT_QRCODE_H_
#define _UNIT_QRCODE_H_

#include "Arduino.h"
#include "Wire.h"

typedef enum {
    AUTO_SCAN_MODE   = 0,
    MANUAL_SCAN_MODE = 1

} qrcode_scan_mode_t;

#include "M5UnitQRCodeI2C.h"
#include "M5UnitQRCodeUART.h"

#endif
