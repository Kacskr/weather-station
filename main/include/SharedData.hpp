#pragma once

#include "Hardware/WeatherSensor/BME280.hpp"

#include <time.h>
#include <sys/time.h>

namespace ws {
    struct SharedData {
        float temperature{};
        float humidity{};
        float pressure{};
        struct tm timeinfo{};
    };
}