#include <driver/gpio.h>

#define BUTTON_GPIO GPIO_NUM_34

namespace button {
    void init(void handler(void* args)) {
        gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
        gpio_pulldown_en(BUTTON_GPIO);
        gpio_pullup_dis(BUTTON_GPIO);
        gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_POSEDGE);
        gpio_install_isr_service(0);
        gpio_isr_handler_add(BUTTON_GPIO, handler, NULL);
    }

    bool is_pressed() {
        return gpio_get_level(BUTTON_GPIO) == 1;
    }
}