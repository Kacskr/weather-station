#pragma once

#include "I2CWeatherSensor.hpp"

extern "C" {
    #include "bme280.h"
}

namespace ws {
    class BME280 final : public I2CWeatherSensor{
    public:
        BME280() = default;
        virtual ~BME280();

        virtual auto init(ws::I2CBus& i2cBus) -> bool;
        
        virtual auto read_temperature() -> float;
        virtual auto read_pressure() -> float;
        virtual auto read_humidity() -> float; 

    private:
        bme280_handle_t m_handle{};
    };
}