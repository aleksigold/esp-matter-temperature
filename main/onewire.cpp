#include <driver/gpio.h>
#include <rom/ets_sys.h>

#define ONEWIRE_GPIO GPIO_NUM_23

namespace onewire {
    static void _high() {
        gpio_set_direction(ONEWIRE_GPIO, GPIO_MODE_INPUT);
    }

    static void _low() {
        gpio_set_direction(ONEWIRE_GPIO, GPIO_MODE_OUTPUT);
        gpio_set_level(ONEWIRE_GPIO, 0);
    }

    static uint8_t _read() {
        return gpio_get_level(ONEWIRE_GPIO) && 1;
    }

    uint8_t reset() {
        uint8_t result;

        _low();
        ets_delay_us(480);
        _high();
        ets_delay_us(60);

        result = _read();

        ets_delay_us(420);

        return result ^ 1;
    }

    void init() {
        _high();
    }

    void write(uint8_t byte) {
        uint8_t i;

        for (i = 0; i < 8; i++) {
            _low();
            ets_delay_us(2);

            if ((byte >> i) & 1) {
                _high();
                ets_delay_us(58);
            } else {
                ets_delay_us(58);
                _high();
            }

            ets_delay_us(1);
        }
    }

    uint8_t read() {
        uint8_t byte = 0;
        uint8_t i;

        for (i = 0; i < 8; i++) {
            _low();
            ets_delay_us(2);
            _high();
            ets_delay_us(8);

            byte |= _read() << i;

            ets_delay_us(50);
        }

        return byte;
    }
}