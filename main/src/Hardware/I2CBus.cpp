#include "Hardware/I2CBus.hpp"

#include "esp_log.h"

namespace ws {
    I2CBus::~I2CBus() {
        if(m_i2cBus){
            i2c_bus_delete(&m_i2cBus);
        }
    }

    auto I2CBus::init(const int sda, const int scl, const int speed) -> bool {
        i2c_config_t conf = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = sda,
            .scl_io_num = scl,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master = {
                .clk_speed = 400000
            },
            .clk_flags = 0
        };
        
        m_i2cBus = i2c_bus_create(I2C_NUM_0, &conf);
        if (!m_i2cBus) {
            ESP_LOGE("I2CBus", "Failed to create I2C bus");
            return 0;
        }

        return 1;
    }
}