#pragma once

namespace ws {
    class WeatherSensor {
    public:
        WeatherSensor() = default;
        virtual ~WeatherSensor() = default;
    
        virtual auto read_temperature() -> float = 0;
        virtual auto read_pressure() -> float = 0;
        virtual auto read_humidity() -> float = 0; 
    };
}