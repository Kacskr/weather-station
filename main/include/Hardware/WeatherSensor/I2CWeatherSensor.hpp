#pragma once

#include "WeatherSensor.hpp"
#include "Hardware/I2CBus.hpp"

namespace ws {
    class I2CWeatherSensor : public WeatherSensor {
    public:
        I2CWeatherSensor() = default;
        virtual ~I2CWeatherSensor() = default;

        virtual auto init(ws::I2CBus& i2cBus) -> bool = 0;
    };
}