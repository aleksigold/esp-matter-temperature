#include <esp_err.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_matter.h>
#include <esp_matter_ota.h>

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;

static const char *TAG = "app_main";

extern "C" void app_main()
{
    nvs_flash_init();

    node::config_t node_config;
    node_t *node = node::create(&node_config, NULL, NULL);
    temperature_sensor::config_t temperature_sensor_config;
    temperature_sensor_config.temperature_measurement.measured_value = 1000;
    endpoint_t *endpoint = temperature_sensor::create(node, &temperature_sensor_config, ENDPOINT_FLAG_NONE, NULL);

    if (!node || !endpoint) {
        ESP_LOGE(TAG, "Matter node creation failed");
    }

    err_t err = esp_matter::start(NULL);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Matter start failed: %d", err);
    }
}
