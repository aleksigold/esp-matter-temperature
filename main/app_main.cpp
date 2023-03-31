#include <esp_err.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_matter.h>

#include "ds18b20.h"

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;
using namespace chip::app::Clusters;

#define TEMPERATURE_RATE_S (2 * 60)

static const char *TAG = "app_main";

uint16_t endpoint_id;

void temperature_task(void *args) {
    while (true) {
        float temperature = 0;
        ds18b20::err_t err = ds18b20::read(&temperature);

        if (err) {
            ESP_LOGE("temperature_task", "Error reading temperature");
            vTaskDelay(pdMS_TO_TICKS(500));

            continue;
        }

        esp_matter_attr_val_t val = esp_matter_nullable_uint16(temperature * 100);

        ESP_LOGI("temperature_task", "Temperature is %.2f", temperature);
        attribute::update(endpoint_id, TemperatureMeasurement::Id, TemperatureMeasurement::Attributes::MeasuredValue::Id, &val);
        vTaskDelay(pdMS_TO_TICKS(TEMPERATURE_RATE_S * 1000));
    }
}

extern "C" void app_main()
{
    nvs_flash_init();
    ds18b20::init();

    node::config_t node_config;
    node_t *node = node::create(&node_config, NULL, NULL);
    temperature_sensor::config_t temperature_sensor_config;
    endpoint_t *endpoint = temperature_sensor::create(node, &temperature_sensor_config, ENDPOINT_FLAG_NONE, NULL);
    endpoint_id = endpoint::get_id(endpoint);

    if (!node || !endpoint) {
        ESP_LOGE(TAG, "Matter node creation failed");
    }

    err_t err = esp_matter::start(NULL);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Matter start failed: %d", err);
    }

    xTaskCreate(temperature_task, "temperature_task", 4096, NULL, 0, NULL);
}
