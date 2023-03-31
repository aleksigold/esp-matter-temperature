#include <stdint.h>

namespace onewire {
    void init();
    uint8_t reset();
    void write(uint8_t byte);
    uint8_t read();
}