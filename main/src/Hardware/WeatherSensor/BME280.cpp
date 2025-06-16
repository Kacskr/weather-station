#include "Hardware/WeatherSensor/BME280.hpp"

#include "esp_log.h"

constexpr int BME280_ADDRESS = 0x76;

namespace ws {
    BME280::~BME280() {
        if(m_handle){
            bme280_delete(&m_handle);
        }
    }

    auto BME280::init(ws::I2CBus& i2cBus) -> bool {
        m_handle = bme280_create(i2cBus.get(), BME280_ADDRESS);
        if (!m_handle) {
            ESP_LOGE("BME280", "Failed to create BME280 handle");
            return 0;
        }

        if (bme280_default_init(m_handle) != ESP_OK) {
            ESP_LOGE("BME280", "BME280 init failed");
            return 0;
        }

        return 1;
    }

    auto BME280::read_temperature() -> float {
        float temperature{}; 
        bme280_read_temperature(m_handle, &temperature);
        return temperature;
    }
    auto BME280::read_pressure() -> float {
        float pressure{};
        bme280_read_pressure(m_handle, &pressure);
        return pressure;
    }
    auto BME280::read_humidity() -> float {
        float humidity{};
        bme280_read_humidity(m_handle, &humidity);
        return humidity;
    }
}