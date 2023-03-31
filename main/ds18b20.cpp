#include <stdio.h>

#include "onewire.h"
#include "ds18b20.h"

namespace ds18b20 {
    void init() {
        onewire::init();
    }

    uint8_t _calculate_crc(uint8_t *data, uint8_t size) {
        uint8_t crc = 0;

        for (uint8_t i = 0; i < size; i++) {
            uint8_t byte = data[i];

            for (uint8_t j = 0; j < 8; j++) {
                uint8_t mix = (crc ^ byte) & 0x01;
                crc >>= 1;

                if (mix) {
                    crc ^= 0x8C;
                }

                byte >>= 1;
            }
        }

        return crc;
    }

    err_t read(float *result) {
        uint16_t raw;
        uint8_t buf[9];

        onewire::reset();
        onewire::write(0xCC);
        onewire::write(0x44);

        while (onewire::read() == 0) {}

        onewire::reset();
        onewire::write(0xCC);
        onewire::write(0xBE);

        for (uint8_t i = 0; i < 9; i++) {
            buf[i] = onewire::read();
        }

        uint8_t crc = _calculate_crc(buf, 8);

        if (crc != buf[8]) {
            return CRC_ERROR;
        }

        raw = buf[0] | (buf[1] << 8);
        *result = (raw >> 11 ? -1 : 1) * (raw & ~(0b11111 << 11)) * 0.0625;

        return OK;
    }
}